#include "PluginEditor.h"

void TubeCageComponent::drawUprightNeonNose(juce::Graphics& g, float cx, float cy) {
    juce::Graphics::ScopedSaveState state(g);

    const float sniffScale = 1.0f + (std::sin(phase) * glow * 0.065f);
    const float sniffTilt  = std::sin(phase) * glow * 0.07f;
    const float sniffY     = -std::abs(std::sin(phase)) * glow * 2.5f;

    g.addTransform(juce::AffineTransform::translation(-cx, -cy)
                       .scaled(sniffScale, sniffScale)
                       .rotated(sniffTilt)
                       .translated(cx, cy + sniffY));

    const float glowIntensity = juce::jlimit(0.18f, 1.0f, 0.22f + glow * 0.88f);
    const float haloR = 46.0f + glow * 16.0f;
    juce::ColourGradient noseHalo(
        juce::Colour(0xFFFF5E00).withAlpha(glowIntensity * 0.55f), cx + 4.0f, cy + 4.0f,
        juce::Colours::transparentBlack, cx + haloR, cy + 4.0f, true);
    g.setGradientFill(noseHalo);
    g.fillEllipse(cx - haloR + 4.0f, cy - haloR + 4.0f, haloR * 2.0f, haloR * 2.0f);

    juce::Path noseOutline;
    noseOutline.startNewSubPath(cx - 7.0f, cy - 30.0f);
    noseOutline.lineTo(cx + 3.0f, cy - 10.0f);
    noseOutline.cubicTo(cx + 15.0f, cy + 4.0f,
                        cx + 18.0f, cy + 15.0f,
                        cx + 8.0f,  cy + 21.0f);
    noseOutline.cubicTo(cx + 1.0f,  cy + 25.0f,
                        cx - 7.0f,  cy + 22.0f,
                        cx - 12.0f, cy + 17.0f);

    juce::Path nostrilCurve;
    nostrilCurve.startNewSubPath(cx - 5.0f, cy + 17.5f);
    nostrilCurve.quadraticTo(cx - 1.0f, cy + 13.5f, cx + 3.5f, cy + 17.5f);

    g.setColour(juce::Colour(0xFFFF5E00).withAlpha(glowIntensity * 0.45f));
    g.strokePath(noseOutline, juce::PathStrokeType(7.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.strokePath(nostrilCurve, juce::PathStrokeType(5.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    g.setColour(juce::Colour(0xFFFF8C1A).withAlpha(glowIntensity * 0.85f));
    g.strokePath(noseOutline, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.strokePath(nostrilCurve, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    g.setColour(juce::Colour(0xFFFFE0B2).withAlpha(juce::jlimit(0.45f, 1.0f, glowIntensity)));
    g.strokePath(noseOutline, juce::PathStrokeType(1.8f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.strokePath(nostrilCurve, juce::PathStrokeType(1.4f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    if (glow > 0.08f) {
        g.setColour(juce::Colour(0xFFFF8C1A).withAlpha(glow * 0.75f));
        g.drawLine(cx + 19.0f, cy + 8.0f,  cx + 26.0f, cy + 5.0f,  1.5f);
        g.drawLine(cx + 20.0f, cy + 14.0f, cx + 28.0f, cy + 14.0f, 1.5f);
        g.drawLine(cx + 17.0f, cy + 20.0f, cx + 24.0f, cy + 23.0f, 1.5f);
    }
}

void TubeCageComponent::drawVacuumTube(juce::Graphics& g, float cx, float cy, float tubeGlow) {
    const float bloomR = 34.0f + tubeGlow * 14.0f;
    juce::ColourGradient halo(
        juce::Colour(0xFFFF5E00).withAlpha(juce::jlimit(0.15f, 0.85f, 0.20f + tubeGlow * 0.65f)), cx, cy,
        juce::Colours::transparentBlack, cx + bloomR, cy, true);
    g.setGradientFill(halo);
    g.fillEllipse(cx - bloomR, cy - bloomR, bloomR * 2.0f, bloomR * 2.0f);

    g.setColour(juce::Colour(0xFF282624));
    g.fillRoundedRectangle(cx - 13.0f, cy + 25.0f, 26.0f, 9.0f, 2.0f);
    g.setColour(juce::Colour(0xFF4A4640));
    g.drawRoundedRectangle(cx - 13.0f, cy + 25.0f, 26.0f, 9.0f, 2.0f, 1.0f);

    juce::Path glass;
    glass.startNewSubPath(cx - 12.0f, cy + 25.0f);
    glass.lineTo(cx - 12.0f, cy - 22.0f);
    glass.cubicTo(cx - 12.0f, cy - 30.0f, cx - 4.0f, cy - 32.0f, cx - 2.5f, cy - 34.0f);
    glass.lineTo(cx - 2.0f, cy - 37.0f);
    glass.lineTo(cx + 2.0f, cy - 37.0f);
    glass.lineTo(cx + 2.5f, cy - 34.0f);
    glass.cubicTo(cx + 4.0f, cy - 32.0f, cx + 12.0f, cy - 30.0f, cx + 12.0f, cy - 22.0f);
    glass.lineTo(cx + 12.0f, cy + 25.0f);
    glass.closeSubPath();

    g.setColour(juce::Colour(0x22DCE6F2).interpolatedWith(juce::Colour(0x55FF8C1A), tubeGlow));
    g.fillPath(glass);

    {
        juce::Graphics::ScopedSaveState clip(g);
        g.reduceClipRegion(glass);
        juce::ColourGradient getter(
            juce::Colour(0xBB989EA8), cx, cy - 37.0f,
            juce::Colour(0x00989EA8), cx, cy - 18.0f, false);
        g.setGradientFill(getter);
        g.fillRect(cx - 13.0f, cy - 38.0f, 26.0f, 22.0f);
    }

    g.setColour(juce::Colour(0xAA888C96));
    g.fillRect(cx - 11.0f, cy - 18.0f, 22.0f, 2.2f);
    g.fillRect(cx - 11.0f, cy + 17.0f, 22.0f, 2.2f);

    const juce::Colour idleFilament(0xFF7A2808);
    const juce::Colour hotFilament(0xFFFFD27D);
    const juce::Colour filCol = idleFilament.interpolatedWith(hotFilament, juce::jlimit(0.0f, 1.0f, 0.25f + tubeGlow * 0.75f));

    for (float offset : { -7.5f, 1.0f }) {
        g.setColour(juce::Colour(0xFF2D2E34));
        g.fillRoundedRectangle(cx + offset, cy - 14.0f, 6.5f, 28.0f, 1.2f);

        g.setColour(filCol);
        g.fillRect(cx + offset + 2.2f, cy - 17.0f, 2.0f, 3.5f);
        g.fillRect(cx + offset + 2.2f, cy + 13.5f, 2.0f, 3.5f);

        g.setColour(filCol.withAlpha(0.55f + tubeGlow * 0.45f));
        g.fillRect(cx + offset + 2.5f, cy - 10.0f, 1.4f, 20.0f);
    }

    g.setColour(juce::Colour(0x55FFFFFF));
    g.fillRect(cx - 9.5f, cy - 20.0f, 1.8f, 42.0f);

    g.setColour(juce::Colour(0x88CCCCCC));
    g.strokePath(glass, juce::PathStrokeType(1.2f));
}

void TubeCageComponent::paint(juce::Graphics& g) {
    auto b = getLocalBounds().toFloat();

    g.setColour(juce::Colour(0xFF070709));
    g.fillRoundedRectangle(b, 6.0f);

    const float effectiveGlow = juce::jlimit(0.15f, 1.0f, glow * 0.85f + drive * 0.25f);
    juce::ColourGradient backGlow(
        juce::Colour(0xFFFF6A00).withAlpha(effectiveGlow * 0.22f), b.getCentreX() + 20.0f, b.getCentreY(),
        juce::Colours::transparentBlack, b.getRight(), b.getCentreY(), true);
    g.setGradientFill(backGlow);
    g.fillRoundedRectangle(b, 6.0f);

    const float cy = b.getCentreY() + 2.0f;

    drawUprightNeonNose(g, b.getCentreX() - 96.0f, cy);

    const float startTubeX = b.getCentreX() - 24.0f;
    const float spacing = 46.0f;
    for (int t = 0; t < 4; ++t) {
        const float tubeVariation = 1.0f - (static_cast<float>(t) * 0.04f);
        drawVacuumTube(g, startTubeX + static_cast<float>(t) * spacing, cy, effectiveGlow * tubeVariation);
    }

    {
        juce::Graphics::ScopedSaveState clip(g);
        g.reduceClipRegion(b.toNearestInt());

        g.setColour(juce::Colour(0x58141418));
        const float step = 6.0f;
        for (float d = -b.getHeight(); d < b.getWidth() + b.getHeight(); d += step) {
            g.drawLine(b.getX() + d, b.getY(), b.getX() + d - b.getHeight(), b.getBottom(), 1.0f);
            g.drawLine(b.getX() + d - b.getHeight(), b.getY(), b.getX() + d, b.getBottom(), 1.0f);
        }

        g.setColour(juce::Colour(0xAA000000));
        g.drawRoundedRectangle(b.reduced(0.5f), 6.0f, 3.0f);
    }

    g.setColour(juce::Colour(0xFF34312C));
    g.drawRoundedRectangle(b, 6.0f, 1.6f);
}

// ==============================================================================

TubeyBloomyAudioProcessorEditor::TubeyBloomyAudioProcessorEditor(TubeyBloomyAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    setLookAndFeel(&ampLookAndFeel);
    addAndMakeVisible(contentContainer);

    contentContainer.addAndMakeVisible(tubeCage);

    bypassSwitch.onToggle = [this] { toggleMasterPower(); };
    contentContainer.addAndMakeVisible(bypassSwitch);

    titleLabel.setText("TUBE SNIFFER", juce::dontSendNotification);
    juce::Font titleFont(juce::FontOptions(20.0f));
    titleFont.setBold(true);
    titleLabel.setFont(titleFont);
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFE6E2D6));
    contentContainer.addAndMakeVisible(titleLabel);

    windowSizeLabel.setText("WINDOW SIZE", juce::dontSendNotification);
    juce::Font wsFont(juce::FontOptions(11.0f));
    wsFont.setBold(true);
    windowSizeLabel.setFont(wsFont);
    windowSizeLabel.setJustificationType(juce::Justification::centredRight);
    windowSizeLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFE6E2D6));
    contentContainer.addAndMakeVisible(windowSizeLabel);

    auto setupScaleBtn = [this](ScalePillButton& btn, int idx) {
        btn.onClick = [this, idx] { setUiScaleIndex(idx); };
        contentContainer.addAndMakeVisible(btn);
    };
    setupScaleBtn(scale1xBtn,   0);
    setupScaleBtn(scale1_5xBtn, 1);
    setupScaleBtn(scale2xBtn,   2);

    auto setupKnobUnit = [this](KnobControlUnit& unit, const juce::String& line1, const juce::String& line2) {
        unit.slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        unit.slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        unit.slider.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                                        juce::MathConstants<float>::pi * 2.75f, true);
        unit.slider.onValueChange = [this] { handleUserSliderChange(); };
        contentContainer.addAndMakeVisible(unit.slider);

        contentContainer.addAndMakeVisible(unit.readout);

        juce::Font lblFont(juce::FontOptions(11.5f));
        lblFont.setBold(true);

        unit.line1Label.setText(line1, juce::dontSendNotification);
        unit.line1Label.setFont(lblFont);
        unit.line1Label.setJustificationType(juce::Justification::centred);
        unit.line1Label.setColour(juce::Label::textColourId, juce::Colour(0xFFE6E2D6));
        contentContainer.addAndMakeVisible(unit.line1Label);

        unit.line2Label.setText(line2, juce::dontSendNotification);
        unit.line2Label.setFont(lblFont);
        unit.line2Label.setJustificationType(juce::Justification::centred);
        unit.line2Label.setColour(juce::Label::textColourId, juce::Colour(0xFFE6E2D6));
        contentContainer.addAndMakeVisible(unit.line2Label);
    };

    setupKnobUnit(driveUnit,     "TUBE",        "DRIVE");
    setupKnobUnit(sagUnit,       "DYNAMIC SAG", "/ FEEL");
    setupKnobUnit(harmonicsUnit, "HARMONIC",    "RICHNESS");
    setupKnobUnit(tightUnit,     "LOW-END",     "TIGHTEN");
    setupKnobUnit(outputUnit,    "OUTPUT",      "LEVEL");

    auto setupSlotBtn = [this](BacklitPresetButton& btn, int idx) {
        btn.onClick = [this, idx] { selectSlot(idx); };
        contentContainer.addAndMakeVisible(btn);
    };
    setupSlotBtn(assBtn,   0);
    setupSlotBtn(bootyBtn, 1);
    setupSlotBtn(crackBtn, 2);

    driveAttach     = std::make_unique<Attachment>(audioProcessor.apvts, "drive",     driveUnit.slider);
    sagAttach       = std::make_unique<Attachment>(audioProcessor.apvts, "sag",       sagUnit.slider);
    harmonicsAttach = std::make_unique<Attachment>(audioProcessor.apvts, "harmonics", harmonicsUnit.slider);
    tightAttach     = std::make_unique<Attachment>(audioProcessor.apvts, "tight",     tightUnit.slider);
    outputAttach    = std::make_unique<Attachment>(audioProcessor.apvts, "output",    outputUnit.slider);

    updateUiState();
    setUiScaleIndex(audioProcessor.uiScaleIndex);
    startTimerHz(30);
}

TubeyBloomyAudioProcessorEditor::~TubeyBloomyAudioProcessorEditor() {
    stopTimer();
    setLookAndFeel(nullptr);
}

void TubeyBloomyAudioProcessorEditor::setUiScaleIndex(int scaleIndex) {
    audioProcessor.uiScaleIndex = juce::jlimit(0, 2, scaleIndex);
    const float scale = getScaleFactor();

    contentContainer.setBounds(0, 0, baseWidth, baseHeight);
    contentContainer.setTransform(juce::AffineTransform::scale(scale));
    setSize(juce::roundToInt(static_cast<float>(baseWidth) * scale),
            juce::roundToInt(static_cast<float>(baseHeight) * scale));

    updateUiState();
}

void TubeyBloomyAudioProcessorEditor::toggleMasterPower() {
    if (auto*
