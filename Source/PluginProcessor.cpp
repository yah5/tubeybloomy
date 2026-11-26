#include "PluginEditor.h"

void CuteTubeMascot::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().toFloat();
    const float cx = bounds.getCentreX();
    const float cy = bounds.getCentreY() + 6.0f;

    const float bounceY = -std::abs(std::sin(phase)) * (glow * 9.0f);
    const float tiltAngle = std::sin(phase) * (glow * 0.14f);
    const float squashX = 1.0f + std::cos(phase * 2.0f) * (glow * 0.05f);
    const float squashY = 1.0f - std::cos(phase * 2.0f) * (glow * 0.05f);

    juce::Graphics::ScopedSaveState state(g);
    g.addTransform(juce::AffineTransform::translation(-cx, -cy)
                       .scaled(squashX, squashY)
                       .rotated(tiltAngle)
                       .translated(cx, cy + bounceY));

    if (glow > 0.02f) {
        juce::ColourGradient halo(
            juce::Colour(0xFFFF7A1A).withAlpha(glow * 0.42f), cx, cy - 4.0f,
            juce::Colours::transparentBlack, cx + 68.0f, cy - 4.0f, true);
        g.setGradientFill(halo);
        g.fillEllipse(cx - 68.0f, cy - 72.0f, 136.0f, 136.0f);
    }

    const float leftFootY = cy + 43.0f + (std::sin(phase) * glow * 4.0f);
    const float rightFootY = cy + 43.0f - (std::sin(phase) * glow * 4.0f);
    g.setColour(juce::Colour(0xFFFF7A1A));
    g.fillRoundedRectangle(cx - 20.0f, leftFootY, 12.0f, 6.0f, 3.0f);
    g.fillRoundedRectangle(cx + 8.0f, rightFootY, 12.0f, 6.0f, 3.0f);

    g.setColour(juce::Colour(0xFFB8B8C0));
    for (int i = -1; i <= 1; ++i) {
        g.fillRoundedRectangle(cx + (static_cast<float>(i) * 11.0f) - 2.0f, cy + 36.0f, 4.0f, 9.0f, 1.5f);
    }

    g.setColour(juce::Colour(0xFF2B2B30));
    g.fillRoundedRectangle(cx - 25.0f, cy + 22.0f, 50.0f, 16.0f, 5.0f);
    g.setColour(juce::Colour(0xFF45454D));
    g.drawRoundedRectangle(cx - 25.0f, cy + 22.0f, 50.0f, 16.0f, 5.0f, 1.5f);

    juce::Path glass;
    glass.addRoundedRectangle(cx - 23.0f, cy - 42.0f, 46.0f, 66.0f, 21.0f);
    g.setColour(juce::Colour(0x22FFFFFF));
    g.fillPath(glass);

    g.fillRoundedRectangle(cx - 4.5f, cy - 48.0f, 9.0f, 8.0f, 4.0f);

    g.setColour(juce::Colour(0xFF3E3E46));
    g.fillRoundedRectangle(cx - 14.0f, cy - 26.0f, 28.0f, 40.0f, 4.0f);

    const juce::Colour coldFilament(0xFF5A2510);
    const juce::Colour hotFilament(0xFFFFAE2B);
    const juce::Colour currentFilament = coldFilament.interpolatedWith(hotFilament, juce::jlimit(0.0f, 1.0f, glow * 1.25f));

    g.setColour(currentFilament);
    g.fillRoundedRectangle(cx - 9.0f, cy - 20.0f, 18.0f, 28.0f, 4.0f);

    g.setColour(juce::Colour(0xFFFFE699).withAlpha(0.25f + glow * 0.75f));
    g.fillRect(cx - 5.0f, cy - 22.0f, 2.5f, 32.0f);
    g.fillRect(cx + 2.5f, cy - 22.0f, 2.5f, 32.0f);

    g.setColour(juce::Colours::white);
    g.fillEllipse(cx - 11.0f, cy - 11.0f, 7.0f, 7.0f);
    g.fillEllipse(cx + 4.0f, cy - 11.0f, 7.0f, 7.0f);

    g.setColour(juce::Colour(0xFF141416));
    g.fillEllipse(cx - 9.0f, cy - 9.5f, 3.8f, 3.8f);
    g.fillEllipse(cx + 6.0f, cy - 9.5f, 3.8f, 3.8f);

    if (glow > 0.1f) {
        g.setColour(juce::Colour(0xFFFF5533).withAlpha(glow * 0.65f));
        g.fillEllipse(cx - 15.0f, cy - 4.0f, 6.0f, 3.5f);
        g.fillEllipse(cx + 9.0f, cy - 4.0f, 6.0f, 3.5f);
    }

    g.setColour(juce::Colours::white);
    juce::Path smile;
    if (glow > 0.35f) {
        smile.addPieSegment(cx - 5.0f, cy - 5.0f, 10.0f, 9.0f, 0.0f, juce::MathConstants<float>::pi, 0.0f);
        g.fillPath(smile);
    } else {
        smile.addArc(cx - 4.5f, cy - 5.0f, 9.0f, 6.0f, 0.2f, juce::MathConstants<float>::pi - 0.2f, true);
        g.strokePath(smile, juce::PathStrokeType(1.8f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    g.setColour(juce::Colour(0x88FFFFFF));
    g.drawRoundedRectangle(cx - 23.0f, cy - 42.0f, 46.0f, 66.0f, 21.0f, 1.8f);

    g.setColour(juce::Colour(0x55FFFFFF));
    g.fillRoundedRectangle(cx - 17.5f, cy - 33.0f, 3.5f, 22.0f, 1.75f);
}

TubeyBloomyAudioProcessorEditor::TubeyBloomyAudioProcessorEditor(TubeyBloomyAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    setSize(440, 490);
    setLookAndFeel(&orangeLookAndFeel);

    titleLabel.setText("tubeybloomy", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(juce::FontOptions(28.0f)));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF8C24));
    addAndMakeVisible(titleLabel);

    addAndMakeVisible(tubeMascot);

    auto setupButton = [this](juce::TextButton& btn, int modeIndex) {
        btn.onClick = [this, modeIndex] { applyStrengthPreset(modeIndex); };
        addAndMakeVisible(btn);
    };
    setupButton(lightBtn, 0);
    setupButton(mediumBtn, 1);
    setupButton(heavyBtn, 2);
    setupButton(customBtn, 3);

    auto setupSlider = [this](juce::Slider& s, juce::Label& l, const juce::String& name, const juce::String& suffix) {
        s.setSliderStyle(juce::Slider::LinearHorizontal);
        s.setTextBoxStyle(juce::Slider::TextBoxRight, false, 58, 24);
        s.setTextValueSuffix(suffix);
        s.setNumDecimalPlacesToDisplay(0);
        s.onValueChange = [this] { handleUserSliderChange(); };
        addAndMakeVisible(s);

        l.setText(name, juce::dontSendNotification);
        l.setFont(juce::Font(juce::FontOptions(14.0f)));
        l.setColour(juce::Label::textColourId, juce::Colour(0xFFE2E2E8));
        addAndMakeVisible(l);
    };

    setupSlider(driveSlider,  driveLabel,  "Tube Drive",         " dB");
    setupSlider(sagSlider,    sagLabel,    "Dynamic Sag / Feel", "");
    setupSlider(tightSlider,  tightLabel,  "Low-End Tighten",    " Hz");
    setupSlider(outputSlider, outputLabel, "Output Level",       " dB");

    driveAttach  = std::make_unique<Attachment>(audioProcessor.apvts, "drive",  driveSlider);
    sagAttach    = std::make_unique<Attachment>(audioProcessor.apvts, "sag",    sagSlider);
    tightAttach  = std::make_unique<Attachment>(audioProcessor.apvts, "tight",  tightSlider);
    outputAttach = std::make_unique<Attachment>(audioProcessor.apvts, "output", outputSlider);

    updateButtonStyles();
    startTimerHz(30);
}

TubeyBloomyAudioProcessorEditor::~TubeyBloomyAudioProcessorEditor() {
    stopTimer();
    setLookAndFeel(nullptr);
}

void TubeyBloomyAudioProcessorEditor::applyStrengthPreset(int mode) {
    isApplyingPreset = true;
    audioProcessor.activeStrengthMode = mode;

    auto setParam = [this](const juce::String& id, float value) {
        if (auto* param = audioProcessor.apvts.getParameter(id)) {
            param->beginChangeGesture();
            param->setValueNotifyingHost(param->convertTo0to1(value));
            param->endChangeGesture();
        }
    };

    if (mode == 0) {
        setParam("drive", 1.0f);
        setParam("sag", 3.0f);
        setParam("tight", 65.0f);
        setParam("output", 0.0f);
    } else if (mode == 1) {
        setParam("drive", 6.0f);
        setParam("sag", 5.0f);
        setParam("tight", 85.0f);
        setParam("output", 0.0f);
    } else if (mode == 2) {
        setParam("drive", 12.0f);
        setParam("sag", 8.0f);
        setParam("tight", 110.0f);
        setParam("output", 0.0f);
    } else if (mode == 3) {
        setParam("drive", audioProcessor.customDrive);
        setParam("sag", audioProcessor.customSag);
        setParam("tight", audioProcessor.customTight);
        setParam("output", audioProcessor.customOutput);
    }

    isApplyingPreset = false;
    updateButtonStyles();
}

void TubeyBloomyAudioProcessorEditor::handleUserSliderChange() {
    if (isApplyingPreset)
        return;

    audioProcessor.activeStrengthMode = 3;
    audioProcessor.customDrive  = static_cast<float>(driveSlider.getValue());
    audioProcessor.customSag    = static_cast<float>(sagSlider.getValue());
    audioProcessor.customTight  = static_cast<float>(tightSlider.getValue());
    audioProcessor.customOutput = static_cast<float>(outputSlider.getValue());
    updateButtonStyles();
}

void TubeyBloomyAudioProcessorEditor::updateButtonStyles() {
    const int mode = audioProcessor.activeStrengthMode;
    juce::TextButton* buttons[] = { &lightBtn, &mediumBtn, &heavyBtn, &customBtn };

    for (int i = 0; i < 4; ++i) {
        const bool isActive = (i == mode);
        buttons[i]->setColour(juce::TextButton::buttonColourId,
                              isActive ? juce::Colour(0xFFFF7A1A) : juce::Colour(0xFF242429));
        buttons[i]->setColour(juce::TextButton::textColourOffId,
                              isActive ? juce::Colour(0xFF141416) : juce::Colour(0xFFD0D0D8));
    }
    repaint();
}

void TubeyBloomyAudioProcessorEditor::timerCallback() {
    const float rawLevel = audioProcessor.currentInputLevel.load(std::memory_order_relaxed);
    const float targetGlow = juce::jlimit(0.0f, 1.0f, std::pow(rawLevel * 2.8f, 0.7f));

    if (targetGlow > smoothedGlow)
        smoothedGlow += (targetGlow - smoothedGlow) * 0.35f;
    else
        smoothedGlow += (targetGlow - smoothedGlow) * 0.12f;

    if (smoothedGlow > 0.01f) {
        dancePhase += 0.24f + (smoothedGlow * 0.22f);
        if (dancePhase > juce::MathConstants<float>::twoPi)
            dancePhase -= juce::MathConstants<float>::twoPi;
    } else {
        dancePhase = 0.0f;
    }

    tubeMascot.setActivity(smoothedGlow, dancePhase);
}

void TubeyBloomyAudioProcessorEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colour(0xFF161619));

    g.setColour(juce::Colour(0xFF1E1E23));
    g.fillRoundedRectangle(20.0f, 240.0f, static_cast<float>(getWidth() - 40), 228.0f, 12.0f);
    g.setColour(juce::Colour(0xFF2D2D35));
    g.drawRoundedRectangle(20.0f, 240.0f, static_cast<float>(getWidth() - 40), 228.0f, 12.0f, 1.0f);
}

void TubeyBloomyAudioProcessorEditor::resized() {
    auto area = getLocalBounds().reduced(24, 16);

    titleLabel.setBounds(area.removeFromTop(36));
    tubeMascot.setBounds(area.removeFromTop(126));

    area.removeFromTop(8);

    auto buttonRow = area.removeFromTop(32);
    const int gap = 8;
    const int btnWidth = (buttonRow.getWidth() - (gap * 3)) / 4;
    lightBtn.setBounds(buttonRow.removeFromLeft(btnWidth));
    buttonRow.removeFromLeft(gap);
    mediumBtn.setBounds(buttonRow.removeFromLeft(btnWidth));
    buttonRow.removeFromLeft(gap);
    heavyBtn.setBounds(buttonRow.removeFromLeft(btnWidth));
    buttonRow.removeFromLeft(gap);
    customBtn.setBounds(buttonRow);

    area.removeFromTop(24);

    auto sliderArea = area.reduced(10, 0);
    auto placeRow = [&](juce::Label& lbl, juce::Slider& sld) {
        auto row = sliderArea.removeFromTop(50);
        lbl.setBounds(row.removeFromTop(20));
        sld.setBounds(row.removeFromTop(26));
    };

    placeRow(driveLabel, driveSlider);
    placeRow(sagLabel, sagSlider);
    placeRow(tightLabel, tightSlider);
    placeRow(outputLabel, outputSlider);
}
