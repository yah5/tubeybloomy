#include "PluginEditor.h"

void TubeCageComponent::drawUprightNeonNose(juce::Graphics& g, float cx, float cy, bool facingRight) {
    juce::Graphics::ScopedSaveState state(g);

    const float dir        = facingRight ? 1.0f : -1.0f;
    const float sniffScale = 1.0f + (std::sin(phase) * glow * 0.075f);
    const float sniffTilt  = std::sin(phase) * glow * 0.08f * dir;
    const float sniffY     = -std::abs(std::sin(phase)) * glow * 2.8f;

    g.addTransform(juce::AffineTransform::translation(-cx, -cy)
                       .scaled(dir * sniffScale, sniffScale)
                       .rotated(sniffTilt)
                       .translated(cx, cy + sniffY));

    // Larger, prominent warm amber glow behind upright nose
    const float glowIntensity = juce::jlimit(0.24f, 1.0f, 0.28f + glow * 0.90f);
    const float haloR = 58.0f + glow * 22.0f;
    juce::ColourGradient noseHalo(
        juce::Colour(0xFFFF5E00).withAlpha(glowIntensity * 0.65f), cx + 4.0f, cy + 4.0f,
        juce::Colours::transparentBlack, cx + haloR, cy + 4.0f, true);
    g.setGradientFill(noseHalo);
    g.fillEllipse(cx - haloR + 4.0f, cy - haloR + 4.0f, haloR * 2.0f, haloR * 2.0f);

    // Upright vertically oriented profile outline of a nose pointing toward center tube
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

    g.setColour(juce::Colour(0xFFFF5E00).withAlpha(glowIntensity * 0.52f));
    g.strokePath(noseOutline, juce::PathStrokeType(8.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.strokePath(nostrilCurve, juce::PathStrokeType(6.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    g.setColour(juce::Colour(0xFFFF8C1A).withAlpha(glowIntensity * 0.90f));
    g.strokePath(noseOutline, juce::PathStrokeType(4.2f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.strokePath(nostrilCurve, juce::PathStrokeType(3.2f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    g.setColour(juce::Colour(0xFFFFE5BD).withAlpha(juce::jlimit(0.50f, 1.0f, glowIntensity)));
    g.strokePath(noseOutline, juce::PathStrokeType(1.9f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.strokePath(nostrilCurve, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    if (glow > 0.06f) {
        g.setColour(juce::Colour(0xFFFF8C1A).withAlpha(glow * 0.85f));
        g.drawLine(cx + 19.0f, cy + 8.0f,  cx + 27.0f, cy + 5.0f,  1.6f);
        g.drawLine(cx + 20.0f, cy + 14.0f, cx + 29.0f, cy + 14.0f, 1.6f);
        g.drawLine(cx + 17.0f, cy + 20.0f, cx + 25.0f, cy + 23.0f, 1.6f);
    }
}

void TubeCageComponent::drawVacuumTube(juce::Graphics& g, float cx, float cy, float tubeGlow) {
    // Larger, prominent warm amber-orange radial bloom behind the single 12AX7 tube
    const float outerBloomR = 86.0f + tubeGlow * 32.0f;
    juce::ColourGradient outerHalo(
        juce::Colour(0xFFFF5E00).withAlpha(juce::jlimit(0.22f, 0.88f, 0.28f + tubeGlow * 0.62f)), cx, cy - 2.0f,
        juce::Colours::transparentBlack, cx + outerBloomR, cy - 2.0f, true);
    g.setGradientFill(outerHalo);
    g.fillEllipse(cx - outerBloomR, cy - 2.0f - outerBloomR, outerBloomR * 2.0f, outerBloomR * 2.0f);

    const float coreBloomR = 44.0f + tubeGlow * 16.0f;
    juce::ColourGradient coreHalo(
        juce::Colour(0xFFFFB84D).withAlpha(juce::jlimit(0.25f, 0.96f, 0.30f + tubeGlow * 0.68f)), cx, cy - 2.0f,
        juce::Colours::transparentBlack, cx + coreBloomR, cy - 2.0f, true);
    g.setGradientFill(coreHalo);
    g.fillEllipse(cx - coreBloomR, cy - 2.0f - coreBloomR, coreBloomR * 2.0f, coreBloomR * 2.0f);

    // 7 Golden Pins protruding from bottom of tube (no dark square socket)
    const float pinOffsets[] = { -11.5f, -7.7f, -3.8f, 0.0f, 3.8f, 7.7f, 11.5f };
    for (float px : pinOffsets) {
        // Warm golden pin body
        g.setColour(juce::Colour(0xFFC89B2B));
        g.fillRoundedRectangle(cx + px - 1.1f, cy + 29.0f, 2.2f, 11.0f, 0.9f);
        // Bright gold specular highlight along each pin
        g.setColour(juce::Colour(0xFFFFE58F));
        g.fillRect(cx + px - 0.4f, cy + 29.0f, 0.8f, 10.0f);
    }

    // Translucent glass button base ridge
    g.setColour(juce::Colour(0x66C8D6E6));
    g.fillRoundedRectangle(cx - 15.5f, cy + 24.5f, 31.0f, 5.5f, 2.5f);
    g.setColour(juce::Colour(0x99FFFFFF));
    g.drawRoundedRectangle(cx - 15.5f, cy + 24.5f, 31.0f, 5.5f, 2.5f, 1.0f);

    // Single 12AX7 glass envelope
    juce::Path glass;
    glass.startNewSubPath(cx - 15.0f, cy + 25.0f);
    glass.lineTo(cx - 15.0f, cy - 24.0f);
    glass.cubicTo(cx - 15.0f, cy - 33.0f, cx - 5.5f, cy - 36.0f, cx - 3.0f, cy - 38.0f);
    glass.lineTo(cx - 2.4f, cy - 42.0f);
    glass.lineTo(cx + 2.4f, cy - 42.0f);
    glass.lineTo(cx + 3.0f, cy - 38.0f);
    glass.cubicTo(cx + 5.5f, cy - 36.0f, cx + 15.0f, cy - 33.0f, cx + 15.0f, cy - 24.0f);
    glass.lineTo(cx + 15.0f, cy + 25.0f);
    glass.closeSubPath();

    g.setColour(juce::Colour(0x28DCE6F2).interpolatedWith(juce::Colour(0x66FF9824), tubeGlow));
    g.fillPath(glass);

    // Top silver getter flash
    {
        juce::Graphics::ScopedSaveState clip(g);
        g.reduceClipRegion(glass);
        juce::ColourGradient getter(
            juce::Colour(0xCC9FA6B2), cx, cy - 42.0f,
            juce::Colour(0x009FA6B2), cx, cy - 20.0f, false);
        g.setGradientFill(getter);
        g.fillRect(cx - 16.0f, cy - 43.0f, 32.0f, 24.0f);
    }

    // Internal support rods & mica spacers
    g.setColour(juce::Colour(0xFF9E9EA8));
    g.fillRect(cx - 10.5f, cy - 22.0f, 1.2f, 46.0f);
    g.fillRect(cx + 9.3f,  cy - 22.0f, 1.2f, 46.0f);

    g.setColour(juce::Colour(0xBB8E929E));
    g.fillRect(cx - 13.8f, cy - 20.0f, 27.6f, 2.5f);
    g.fillRect(cx - 13.8f, cy + 17.5f, 27.6f, 2.5f);

    // Dual triode plates & bright glowing orange heater filaments
    const juce::Colour idleFilament(0xFF8A300A);
    const juce::Colour hotFilament(0xFFFFE082);
    const juce::Colour filCol = idleFilament.interpolatedWith(hotFilament, juce::jlimit(0.0f, 1.0f, 0.30f + tubeGlow * 0.70f));

    for (float offset : { -9.5f, 1.5f }) {
        g.setColour(juce::Colour(0xFF2D2E34));
        g.fillRoundedRectangle(cx + offset, cy - 15.5f, 8.0f, 30.0f, 1.4f);

        g.setColour(filCol);
        g.fillRect(cx + offset + 2.8f, cy - 19.0f, 2.4f, 4.0f);
        g.fillRect(cx + offset + 2.8f, cy + 14.5f, 2.4f, 4.0f);

        g.setColour(filCol.withAlpha(0.65f + tubeGlow * 0.35f));
        g.fillRect(cx + offset + 3.1f, cy - 11.5f, 1.8f, 22.0f);
    }

    // Specular glass edge highlight
    g.setColour(juce::Colour(0x66FFFFFF));
    g.fillRect(cx - 12.0f, cy - 22.0f, 2.2f, 44.0f);

    g.setColour(juce::Colour(0x99D6D6DE));
    g.strokePath(glass, juce::PathStrokeType(1.3f));
}

void TubeCageComponent::paint(juce::Graphics& g) {
    auto b = getLocalBounds().toFloat();

    // Clean recessed dark interior (#070709) — wire-mesh grille removed
    g.setColour(juce::Colour(0xFF070709));
    g.fillRoundedRectangle(b, 6.0f);

    // Prominent warm interior backplate wash
    const float effectiveGlow = juce::jlimit(0.20f, 1.0f, glow * 0.90f + drive * 0.25f);
    juce::ColourGradient backGlow(
        juce::Colour(0xFFFF6A00).withAlpha(effectiveGlow * 0.34f), b.getCentreX(), b.getCentreY(),
        juce::Colours::transparentBlack, b.getCentreX() + 180.0f, b.getCentreY(), true);
    g.setGradientFill(backGlow);
    g.fillRoundedRectangle(b, 6.0f);

    const float cx = b.getCentreX();
    const float cy = b.getCentreY() + 1.0f;

    // Single centered 12AX7 vacuum tube with golden pins
    drawVacuumTube(g, cx, cy, effectiveGlow);

    // Two upright glowing neon noses on left and right pointing inward toward the tube
    drawUprightNeonNose(g, cx - 76.0f, cy, true);
    drawUprightNeonNose(g, cx + 76.0f, cy, false);

    // Subtle recessed window shadow & border
    g.setColour(juce::Colour(0x88000000));
    g.drawRoundedRectangle(b.reduced(0.5f), 6.0f, 2.5f);

    g.setColour(juce::Colour(0xFF34312C));
    g.drawRoundedRectangle(b, 6.0f, 1.5f);
}

// ==============================================================================

TubeyBloomyAudioProcessorEditor::TubeyBloomyAudioProcessorEditor(TubeyBloomyAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    setLookAndFeel(&ampLookAndFeel);
    addAndMakeVisible(contentContainer);

    // Top-Right Window Size Bar
    windowSizeLabel.setText("Window Size", juce::dontSendNotification);
    juce::Font wsFont(juce::FontOptions(11.5f));
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

    // Top Vacuum Tube Cage & Dual Upright Neon Noses
    contentContainer.addAndMakeVisible(tubeCage);

    // Faceplate Header Bar: Bypass Switch + Orange "Tube Sniffer" Title
    bypassSwitch.onToggle = [this] { toggleMasterPower(); };
    contentContainer.addAndMakeVisible(bypassSwitch);

    titleLabel.setText("Tube Sniffer", juce::dontSendNotification);
    juce::Font titleFont(juce::FontOptions(22.0f));
    titleFont.setBold(true);
    titleLabel.setFont(titleFont);
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF8C1A));
    contentContainer.addAndMakeVisible(titleLabel);

    // Helper to configure each rotary knob unit in Title Case
    auto setupKnobUnit = [this](KnobControlUnit& unit, const juce::String& line1, const juce::String& line2) {
        unit.slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        unit.slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        unit.slider.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                                        juce::MathConstants<float>::pi * 2.75f, true);
        unit.slider.onValueChange = [this] { handleUserSliderChange(); };
        contentContainer.addAndMakeVisible(unit.slider);

        contentContainer.addAndMakeVisible(unit.readout);

        juce::Font lblFont(juce::FontOptions(12.0f));
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

    // Order: Dynamic Sag / Feel -> Harmonic Richness -> Swamp Ass Drive -> Low-End Tighten | Output Level
    setupKnobUnit(sagUnit,       "Dynamic Sag", "/ Feel");
    setupKnobUnit(harmonicsUnit, "Harmonic",    "Richness");
    setupKnobUnit(driveUnit,     "Swamp Ass",   "Drive");
    setupKnobUnit(tightUnit,     "Low-End",     "Tighten");
    setupKnobUnit(outputUnit,    "Output",      "Level");

    // Bottom 3 Preset Strip (Title Case with Bold A - B - C first letters)
    auto setupSlotBtn = [this](BacklitPresetButton& btn, int idx) {
        btn.onClick = [this, idx] { selectSlot(idx); };
        contentContainer.addAndMakeVisible(btn);
    };
    setupSlotBtn(assBtn,   0);
    setupSlotBtn(bootyBtn, 1);
    setupSlotBtn(crackBtn, 2);

    // Bottom-Right Credit / Version Text
    versionLabel.setText("v.alpha - By: Yousef AbdulHusain", juce::dontSendNotification);
    versionLabel.setFont(juce::Font(juce::FontOptions(10.0f)));
    versionLabel.setJustificationType(juce::Justification::centredRight);
    versionLabel.setColour(juce::Label::textColourId, juce::Colour(0xFF8C887E));
    contentContainer.addAndMakeVisible(versionLabel);

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
    if (auto* param = audioProcessor.apvts.getParameter("bypass")) {
        const bool currentlyBypassed = param->getValue() > 0.5f;
        param->beginChangeGesture();
        param->setValueNotifyingHost(currentlyBypassed ? 0.0f : 1.0f);
        param->endChangeGesture();
    }
    updateUiState();
}

void TubeyBloomyAudioProcessorEditor::selectSlot(int slotIndex) {
    const int prevSlot = audioProcessor.activeSlot;
    if (prevSlot >= 0 && prevSlot < 3) {
        audioProcessor.slotMemory[static_cast<size_t>(prevSlot)] = {
            static_cast<float>(driveUnit.slider.getValue()),
            static_cast<float>(sagUnit.slider.getValue()),
            static_cast<float>(harmonicsUnit.slider.getValue()),
            static_cast<float>(tightUnit.slider.getValue())
        };
    }

    audioProcessor.activeSlot = slotIndex;
    isLoadingSlot = true;

    const auto& target = audioProcessor.slotMemory[static_cast<size_t>(slotIndex)];
    auto setParam = [this](const juce::String& id, float value) {
        if (auto* param = audioProcessor.apvts.getParameter(id)) {
            param->beginChangeGesture();
            param->setValueNotifyingHost(param->convertTo0to1(value));
            param->endChangeGesture();
        }
    };

    setParam("drive",     target.drive);
    setParam("sag",       target.sag);
    setParam("harmonics", target.harmonics);
    setParam("tight",     target.tight);

    isLoadingSlot = false;
    updateUiState();
}

void TubeyBloomyAudioProcessorEditor::handleUserSliderChange() {
    const float currentOut = static_cast<float>(outputUnit.slider.getValue());
    if (currentOut >= 0.5f)
        audioProcessor.lastNonZeroOutput = currentOut;

    if (!isLoadingSlot) {
        const int slot = audioProcessor.activeSlot;
        if (slot >= 0 && slot < 3) {
            audioProcessor.slotMemory[static_cast<size_t>(slot)] = {
                static_cast<float>(driveUnit.slider.getValue()),
                static_cast<float>(sagUnit.slider.getValue()),
                static_cast<float>(harmonicsUnit.slider.getValue()),
                static_cast<float>(tightUnit.slider.getValue())
            };
        }
    }

    updateUiState();
}

void TubeyBloomyAudioProcessorEditor::updateUiState() {
    const bool isMasterOn = audioProcessor.apvts.getRawParameterValue("bypass")->load() < 0.5f;
    bypassSwitch.setActive(isMasterOn);

    const int driveVal = juce::roundToInt(driveUnit.slider.getValue());
    const int sagVal   = juce::roundToInt(sagUnit.slider.getValue());
    const int harmVal  = juce::roundToInt(harmonicsUnit.slider.getValue());
    const int tightVal = juce::roundToInt(tightUnit.slider.getValue());
    const int outVal   = juce::roundToInt(outputUnit.slider.getValue());

    driveUnit.readout.setText(juce::String(driveVal) + " dB");
    sagUnit.readout.setText(juce::String(sagVal));
    harmonicsUnit.readout.setText(juce::String(harmVal));
    tightUnit.readout.setText(juce::String(tightVal));
    outputUnit.readout.setText(outVal == 0 ? "OFF" : (juce::String(outVal) + " dB"));

    const int slot = audioProcessor.activeSlot;
    assBtn.setSelected(slot == 0);
    bootyBtn.setSelected(slot == 1);
    crackBtn.setSelected(slot == 2);

    const int idx = audioProcessor.uiScaleIndex;
    scale1xBtn.setSelected(idx == 0);
    scale1_5xBtn.setSelected(idx == 1);
    scale2xBtn.setSelected(idx == 2);

    repaint();
}

void TubeyBloomyAudioProcessorEditor::timerCallback() {
    const bool isMasterOn = audioProcessor.apvts.getRawParameterValue("bypass")->load() < 0.5f;
    const float rawLevel  = isMasterOn ? audioProcessor.currentInputLevel.load(std::memory_order_relaxed) : 0.0f;
    const float driveNorm = isMasterOn ? (static_cast<float>(driveUnit.slider.getValue()) / 12.0f) : 0.0f;

    const float targetGlow = isMasterOn
        ? juce::jlimit(0.0f, 1.0f, std::pow(rawLevel * 20.0f, 0.36f))
        : 0.0f;

    if (targetGlow > smoothedGlow)
        smoothedGlow += (targetGlow - smoothedGlow) * 0.65f;
    else
        smoothedGlow += (targetGlow - smoothedGlow) * 0.06f;

    if (smoothedGlow > 0.01f) {
        sniffPhase += 0.28f + (smoothedGlow * 0.24f);
        if (sniffPhase > juce::MathConstants<float>::twoPi)
            sniffPhase -= juce::MathConstants<float>::twoPi;
    } else {
        sniffPhase = 0.0f;
    }

    tubeCage.setActivity(smoothedGlow, sniffPhase, driveNorm);
}

void TubeyBloomyAudioProcessorEditor::paint(juce::Graphics& g) {
    const float scale = getScaleFactor();
    g.addTransform(juce::AffineTransform::scale(scale));

    const float w = static_cast<float>(baseWidth);
    const float h = static_cast<float>(baseHeight);

    // 1. Outer Textured Black Tolex Cabinet Housing
    g.fillAll(juce::Colour(0xFF0A0A0C));

    g.setColour(juce::Colour(0x0DFFFFFF));
    for (int py = 4; py < baseHeight; py += 6) {
        for (int px = (py % 12 == 0 ? 4 : 7); px < baseWidth; px += 8) {
            g.fillRect(px, py, 2, 2);
        }
    }

    // Dark bronze cabinet corner protectors
    auto drawCornerProtector = [&](float cx, float cy, bool flipX, bool flipY) {
        juce::Graphics::ScopedSaveState s(g);
        g.addTransform(juce::AffineTransform::translation(-cx, -cy)
                           .scaled(flipX ? -1.0f : 1.0f, flipY ? -1.0f : 1.0f)
                           .translated(cx, cy));
        juce::Path corner;
        corner.startNewSubPath(cx, cy);
        corner.lineTo(cx + 28.0f, cy);
        corner.lineTo(cx + 22.0f, cy + 8.0f);
        corner.lineTo(cx + 8.0f,  cy + 8.0f);
        corner.lineTo(cx + 8.0f,  cy + 22.0f);
        corner.lineTo(cx,         cy + 28.0f);
        corner.closeSubPath();

        g.setColour(juce::Colour(0xFF2B231E));
        g.fillPath(corner);
        g.setColour(juce::Colour(0xFF4A3D34));
        g.strokePath(corner, juce::PathStrokeType(1.0f));
    };
    drawCornerProtector(0.0f, 0.0f, false, false);
    drawCornerProtector(w,    0.0f, true,  false);
    drawCornerProtector(0.0f, h,    false, true);
    drawCornerProtector(w,    h,    true,  true);

    // 2. Brushed Matte Black Aluminum Faceplate Chassis (#0D0D0F to #141416)
    const juce::Rectangle<float> faceplate(14.0f, 146.0f, w - 28.0f, h - 160.0f);
    juce::ColourGradient fpGrad(
        juce::Colour(0xFF141416), faceplate.getX(), faceplate.getY(),
        juce::Colour(0xFF0D0D0F), faceplate.getX(), faceplate.getBottom(), false);
    g.setGradientFill(fpGrad);
    g.fillRoundedRectangle(faceplate, 6.0f);

    g.setColour(juce::Colour(0x07FFFFFF));
    for (float gy = faceplate.getY() + 3.0f; gy < faceplate.getBottom() - 2.0f; gy += 3.0f) {
        g.drawHorizontalLine(static_cast<int>(gy), faceplate.getX() + 4.0f, faceplate.getRight() - 4.0f);
    }

    g.setColour(juce::Colour(0xFF2E2C28));
    g.drawRoundedRectangle(faceplate, 6.0f, 1.5f);

    // 3. Glowing Amber Power/Active LED next to Bypass Switch
    const bool isMasterOn = audioProcessor.apvts.getRawParameterValue("bypass")->load() < 0.5f;
    const float ledX = 56.0f;
    const float ledY = 169.0f;
    if (isMasterOn) {
        juce::ColourGradient ledBloom(
            juce::Colour(0xAAFF8C1A), ledX, ledY,
            juce::Colours::transparentBlack, ledX + 14.0f, ledY, true);
        g.setGradientFill(ledBloom);
        g.fillEllipse(ledX - 14.0f, ledY - 14.0f, 28.0f, 28.0f);

        g.setColour(juce::Colour(0xFFFF8C1A));
        g.fillEllipse(ledX - 4.5f, ledY - 4.5f, 9.0f, 9.0f);
        g.setColour(juce::Colour(0xFFFFE2B8));
        g.fillEllipse(ledX - 2.0f, ledY - 2.0f, 4.0f, 4.0f);
    } else {
        g.setColour(juce::Colour(0xFF2A1210));
        g.fillEllipse(ledX - 4.5f, ledY - 4.5f, 9.0f, 9.0f);
        g.setColour(juce::Colour(0xFF4A2824));
        g.drawEllipse(ledX - 4.5f, ledY - 4.5f, 9.0f, 9.0f, 1.0f);
    }

    // 4. Subtle dark recessed knob panels (white outlines completely removed)
    const juce::Rectangle<float> leftBox(28.0f, 194.0f, 466.0f, 174.0f);
    const juce::Rectangle<float> rightBox(506.0f, 194.0f, 126.0f, 174.0f);

    g.setColour(juce::Colour(0xFF101013));
    g.fillRoundedRectangle(leftBox, 8.0f);
    g.fillRoundedRectangle(rightBox, 8.0f);
}

void TubeyBloomyAudioProcessorEditor::resized() {
    contentContainer.setBounds(0, 0, baseWidth, baseHeight);

    // Top-Right Window Size controls at top right of the window
    windowSizeLabel.setBounds(416, 8, 92, 18);
    scale1xBtn.setBounds(514, 8, 32, 18);
    scale1_5xBtn.setBounds(550, 8, 42, 18);
    scale2xBtn.setBounds(596, 8, 32, 18);

    // Top Vacuum Tube Cage & Dual Upright Noses Window
    tubeCage.setBounds(18, 32, baseWidth - 36, 106);

    // Faceplate Header Bar
    bypassSwitch.setBounds(24, 154, 24, 30);
    titleLabel.setBounds(72, 154, 260, 30);

    // Helper to lay out a vertical knob stack inside a column
    auto layoutKnobColumn = [](KnobControlUnit& unit, juce::Rectangle<int> col) {
        const int readoutW = 56;
        const int readoutH = 26;
        unit.readout.setBounds(col.withSizeKeepingCentre(readoutW, readoutH).withY(col.getY() + 6));

        const int knobSize = 84;
        unit.slider.setBounds(col.withSizeKeepingCentre(knobSize, knobSize).withY(col.getY() + 36));

        unit.line1Label.setBounds(col.getX(), col.getY() + 126, col.getWidth(), 16);
        unit.line2Label.setBounds(col.getX(), col.getY() + 142, col.getWidth(), 16);
    };

    // Left Container: Dynamic Sag / Feel -> Harmonic Richness -> Swamp Ass Drive -> Low-End Tighten
    juce::Rectangle<int> leftInner(34, 198, 454, 166);
    const int colW = leftInner.getWidth() / 4;
    layoutKnobColumn(sagUnit,       leftInner.removeFromLeft(colW));
    layoutKnobColumn(harmonicsUnit, leftInner.removeFromLeft(colW));
    layoutKnobColumn(driveUnit,     leftInner.removeFromLeft(colW));
    layoutKnobColumn(tightUnit,     leftInner);

    // Right Container: Output Level
    juce::Rectangle<int> rightInner(512, 198, 114, 166);
    layoutKnobColumn(outputUnit, rightInner);

    // Bottom Preset Strip (Centered along bottom faceplate margin)
    const int btnW = 122;
    const int btnH = 28;
    const int gap = 14;
    const int totalStripW = (btnW * 3) + (gap * 2);
    const int startX = (baseWidth - totalStripW) / 2;
    const int stripY = 380;

    assBtn.setBounds(startX, stripY, btnW, btnH);
    bootyBtn.setBounds(startX + btnW + gap, stripY, btnW, btnH);
    crackBtn.setBounds(startX + (btnW + gap) * 2, stripY, btnW, btnH);

    // Bottom-Right Version / Credit Text
    versionLabel.setBounds(baseWidth - 220, baseHeight - 16, 204, 14);
}