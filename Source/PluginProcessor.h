#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>

class TubeyBloomyAudioProcessor : public juce::AudioProcessor {
public:
    struct SlotSettings {
        float drive;
        float sag;
        float harmonics;
        float tight;
    };

    TubeyBloomyAudioProcessor();
    ~TubeyBloomyAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    juce::AudioProcessorParameter* getBypassParameter() const override {
        return apvts.getParameter("bypass");
    }

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

    std::atomic<float> currentInputLevel { 0.0f };

    // 0 = Ass Sniff, 1 = Booty Sniff, 2 = Crack Sniff (Output Level is independent)
    int activeSlot = 0;
    std::array<SlotSettings, 3> slotMemory {{
        { 2.0f,  3.0f, 1.0f, 70.0f  }, // Ass Sniff
        { 6.0f,  7.0f, 3.0f, 90.0f  }, // Booty Sniff
        { 10.0f, 11.0f, 6.0f, 115.0f } // Crack Sniff
    }};

    float lastNonZeroOutput = 1.0f;

    // UI scale index: 0 = 1x (100%), 1 = 1.5x (150%), 2 = 2x (200%)
    int uiScaleIndex = 0;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> preHighPass;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> postLowPass;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> harmonicDcBlock;

    float envelope = 0.0f;
    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TubeyBloomyAudioProcessor)
};
