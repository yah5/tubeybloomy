#include "PluginProcessor.h"
#include "PluginEditor.h"

TubeyBloomyAudioProcessor::TubeyBloomyAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout TubeyBloomyAudioProcessor::createParameterLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Master Bypass (false = Plugin ON, true = Plugin OFF / Bypassed)
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{"bypass", 1}, "Master Bypass", false));

    // Tube Drive: 0 to 24 (step 1)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"drive", 1}, "Tube Drive (dB)",
        juce::NormalisableRange<float>(0.0f, 24.0f, 1.0f), 3.0f));

    // Dynamic Sag / Feel: 0 to 10 (step 1)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"sag", 1}, "Dynamic Sag / Feel",
        juce::NormalisableRange<float>(0.0f, 10.0f, 1.0f), 3.0f));

    // Harmonic Richness: 0 to 5 (step 1)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"harmonics", 1}, "Harmonic Richness",
        juce::NormalisableRange<float>(0.0f, 5.0f, 1.0f), 1.0f));

    // Low-End Tighten: 30 to 180 Hz in increments of 5
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"tight", 1}, "Low-End Tighten (Hz)",
        juce::NormalisableRange<float>(30.0f, 180.0f, 5.0f), 70.0f));

    // Output Level: 0 (OFF / Bypassed) to 12 dB (step 1)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"output", 1}, "Output Level (dB)",
        juce::NormalisableRange<float>(0.0f, 12.0f, 1.0f), 0.0f));

    return { params.begin(), params.end() };
}

bool TubeyBloomyAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    return layouts.getMainOutputChannelSet() == layouts.getMainInputChannelSet()
           && !layouts.getMainOutputChannelSet().isDisabled();
}

void TubeyBloomyAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    const int numChannels = getTotalNumOutputChannels();
    const double osRate = sampleRate * 4.0;

    oversampler = std::make_unique<juce::dsp::Oversampling<float>>(
        numChannels, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, true);
    oversampler->initProcessing(static_cast<size_t>(samplesPerBlock));

    juce::dsp::ProcessSpec spec{ osRate, static_cast<juce::uint32>(samplesPerBlock * 4), static_cast<juce::uint32>(numChannels) };
    preHighPass.prepare(spec);
    postLowPass.prepare(spec);
    harmonicDcBlock.prepare(spec);

    *postLowPass.state     = *juce::dsp::IIR::Coefficients<float>::makeLowPass(osRate, 9200.0f);
    *harmonicDcBlock.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(osRate, 20.0f);

    attackCoeff  = std::exp(-1.0f / (static_cast<float>(osRate) * 0.0025f));
    releaseCoeff = std::exp(-1.0f / (static_cast<float>(osRate) * 0.110f));
    envelope = 0.0f;
}

void TubeyBloomyAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::ScopedNoDenormals noDenormals;

    float blockInputPeak = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        blockInputPeak = juce::jmax(blockInputPeak, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
    }
    currentInputLevel.store(blockInputPeak, std::memory_order_relaxed);

    // Master ON/OFF bypass check
    const bool isBypassed = apvts.getRawParameterValue("bypass")->load() > 0.5f;
    if (isBypassed) {
        return; // Clean true-bypass pass-through
    }

    const float outputDb = apvts.getRawParameterValue("output")->load();
    // Output Level = 0 means OFF (completely bypassed / muted)
    if (outputDb < 0.5f) {
        buffer.clear();
        return;
    }

    const float driveDb      = apvts.getRawParameterValue("drive")->load();
    const float sag          = apvts.getRawParameterValue("sag")->load() * 0.1f;
    const float harmonicsAmt = apvts.getRawParameterValue("harmonics")->load() / 5.0f;
    const float tightHz      = apvts.getRawParameterValue("tight")->load();

    const float inputGain  = juce::Decibels::decibelsToGain(driveDb);
    const float outputGain = juce::Decibels::decibelsToGain(outputDb);

    const float transformerCutoff = 9200.0f - (harmonicsAmt * 1700.0f);
    *postLowPass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate() * 4.0, transformerCutoff);
    *preHighPass.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate() * 4.0, tightHz);

    juce::dsp::AudioBlock<float> block(buffer);
    auto osBlock = oversampler->processSamplesUp(block);

    juce::dsp::ProcessContextReplacing<float> context(osBlock);
    preHighPass.process(context);

    const size_t numChannels = osBlock.getNumChannels();
    const size_t numSamples  = osBlock.getNumSamples();

    for (size_t i = 0; i < numSamples; ++i) {
        float peak = 0.0f;
        for (size_t ch = 0; ch < numChannels; ++ch)
            peak = juce::jmax(peak, std::abs(osBlock.getSample(ch, i) * inputGain));

        envelope = (peak > envelope)
            ? attackCoeff * envelope + (1.0f - attackCoeff) * peak
            : releaseCoeff * envelope + (1.0f - releaseCoeff) * peak;

        const float dynamicBias    = 0.12f + (envelope * sag * 0.25f);
        const float sagCompression = 1.0f / (1.0f + (envelope * sag * 0.85f));
        const float dcOffset       = std::tanh(dynamicBias * 1.15f);

        for (size_t ch = 0; ch < numChannels; ++ch) {
            const float x      = osBlock.getSample(ch, i) * inputGain * sagCompression;
            const float biased = x + dynamicBias;
            const float triode = (biased >= 0.0f) ? std::tanh(biased * 1.15f) : std::tanh(biased * 0.85f);
            float outSample    = triode - dcOffset;

            if (harmonicsAmt > 0.001f) {
                const float clamped   = std::tanh(outSample * 1.25f);
                const float even2nd   = (clamped * clamped) * 0.42f;
                const float odd3rd    = (clamped * clamped * clamped - 0.75f * clamped) * 0.34f;
                const float overtones = std::tanh((even2nd + odd3rd) * (1.0f + harmonicsAmt * 0.8f));

                outSample += overtones * (harmonicsAmt * 0.55f);
            }

            osBlock.setSample(ch, i, outSample);
        }
    }

    harmonicDcBlock.process(context);
    postLowPass.process(context);
    oversampler->processSamplesDown(block);
    buffer.applyGain(outputGain);
}

juce::AudioProcessorEditor* TubeyBloomyAudioProcessor::createEditor() {
    return new TubeyBloomyAudioProcessorEditor(*this);
}

void TubeyBloomyAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    auto state = apvts.copyState();
    state.setProperty("activeSlot", activeSlot, nullptr);
    state.setProperty("uiScaleIndex", uiScaleIndex, nullptr);
    state.setProperty("lastNonZeroOutput", lastNonZeroOutput, nullptr);

    for (size_t i = 0; i < 3; ++i) {
        const juce::String prefix = "slot" + juce::String(static_cast<int>(i)) + "_";
        state.setProperty(prefix + "drive",     slotMemory[i].drive, nullptr);
        state.setProperty(prefix + "sag",       slotMemory[i].sag, nullptr);
        state.setProperty(prefix + "harmonics", slotMemory[i].harmonics, nullptr);
        state.setProperty(prefix + "tight",     slotMemory[i].tight, nullptr);
        state.setProperty(prefix + "output",    slotMemory[i].output, nullptr);
    }

    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TubeyBloomyAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState && xmlState->hasTagName(apvts.state.getType())) {
        auto tree = juce::ValueTree::fromXml(*xmlState);
        activeSlot        = static_cast<int>(tree.getProperty("activeSlot", 0));
        uiScaleIndex      = juce::jlimit(0, 2, static_cast<int>(tree.getProperty("uiScaleIndex", 0)));
        lastNonZeroOutput = static_cast<float>(tree.getProperty("lastNonZeroOutput", 6.0f));

        for (size_t i = 0; i < 3; ++i) {
            const juce::String prefix = "slot" + juce::String(static_cast<int>(i)) + "_";
            slotMemory[i].drive     = static_cast<float>(tree.getProperty(prefix + "drive",     slotMemory[i].drive));
            slotMemory[i].sag       = static_cast<float>(tree.getProperty(prefix + "sag",       slotMemory[i].sag));
            slotMemory[i].harmonics = static_cast<float>(tree.getProperty(prefix + "harmonics", slotMemory[i].harmonics));
            slotMemory[i].tight     = static_cast<float>(tree.getProperty(prefix + "tight",     slotMemory[i].tight));
            slotMemory[i].output    = static_cast<float>(tree.getProperty(prefix + "output",    slotMemory[i].output));
        }
        apvts.replaceState(tree);
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new TubeyBloomyAudioProcessor();
}
