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

    // Ambient warm amber glow behind upright nose
    const float glowIntensity = juce::jlimit(0.18f, 1.0f, 0.22f + glow * 0.88f);
    const float haloR = 46.0f + glow * 16.0f;
    juce::ColourGradient noseHalo(
        juce::Colour(0xFFFF5E00).withAlpha(glowIntensity * 0.55f), cx + 4.0f, cy + 4.0f,
        juce::Colours::transparentBlack, cx + haloR, cy + 4.0f, true);
    g.setGradientFill(noseHalo);
    g.fillEllipse(cx - haloR + 4.0f, cy - haloR + 4.0f, haloR * 2.0f, haloR * 2.0f);

    // Upright vertically oriented profile outline of a nose (facing right toward the tubes)
    juce::Path noseOutline;
    noseOutline.startNewSubPath(cx - 7.0f, cy - 30.0f);
    noseOutline.lineTo(cx + 3.0f, cy - 10.0f);
    noseOutline.cubicTo(cx + 15.0f, cy + 4.0f,
                        cx + 18.0f, cy + 15.0f,
                        cx + 8.0f,  cy + 21.0f);
    noseOutline.cubicTo(cx + 1.0f,  cy + 25.0f,
                        cx - 7.0f,  cy + 22.0f,
                        cx - 12.0f, cy + 17.0f);

    // Nostril curve
    juce::Path nostrilCurve;
    nostrilCurve.startNewSubPath(cx - 5.0f, cy + 17.5f);
    nostrilCurve.quadraticTo(cx - 1.0f, cy + 13.5f, cx + 3.5f, cy + 17.5f);

    // Multi-pass neon tube stroke (#FF5E00 outer bloom -> #FF8C1A mid -> #FFD699 core)
    g.setColour(juce::Colour(0xFFFF5E00).withAlpha(glowIntensity * 0.45f));
    g.strokePath(noseOutline, juce::PathStrokeType(7.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.strokePath(nostrilCurve, juce::PathStrokeType(5.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    g.setColour(juce::Colour(0xFFFF8C1A).withAlpha(glowIntensity * 0.85f));
    g.strokePath(noseOutline, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.strokePath(nostrilCurve, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    g.setColour(juce::Colour(0xFFFFE0B2).withAlpha(juce::jlimit(0.45f, 1.0f, glowIntensity)));
    g.strokePath(noseOutline, juce::PathStrokeType(1.8f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.strokePath(nostrilCurve, juce::PathStrokeType(1.4f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Subtle animated sniff air-wave wisps between nose tip and tubes
    if (glow > 0.08f) {
        g.setColour(juce::Colour(0xFFFF8C1A).withAlpha(glow * 0.75f));
        g.drawLine(cx + 19.0f, cy + 8.0f,  cx + 26.0f, cy + 5.0f,  1.5f);
        g.drawLine(cx + 20.0f, cy + 14.0f, cx + 28.0f, cy + 14.0f, 1.5f);
        g.drawLine(cx + 17.0f, cy + 20.0f, cx + 24.0f, cy + 23.0f, 1.5f);
    }
}

void TubeCageComponent::drawVacuumTube(juce::Graphics& g, float cx, float cy, float tubeGlow) {
    // Filament radial bloom behind tube
    const float bloomR = 34.0f + tubeGlow * 14.0f;
    juce::ColourGradient halo(
        juce::Colour(0xFFFF5E00).withAlpha(juce::jlimit(0.15f, 0.85f, 0.20f + tubeGlow * 0.65f)), cx, cy,
        juce::Colours::transparentBlack, cx + bloomR, cy, true);
    g.setGradientFill(halo);
    g.fillEllipse(cx - bloomR, cy - bloomR, bloomR * 2.0f, bloomR * 2.0f);

    // Ceramic socket base
    g.setColour(juce::Colour(0xFF282624));
    g.fillRoundedRectangle(cx - 13.0f, cy + 25.0f, 26.0f, 9.0f, 2.0f);
    g.setColour(juce::Colour(0xFF4A4640));
    g.drawRoundedRectangle(cx - 13.0f, cy + 25.0f, 26.0f, 9.0f, 2.0f, 1.0f);

    // Glass envelope
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

    // Top silver getter flash
    {
        juce::Graphics::ScopedSaveState clip(g);
        g.reduceClipRegion(glass);
        juce::ColourGradient getter(
            juce::Colour(0xBB989EA8), cx, cy - 37.0f,
            juce::Colour(0x00989EA8), cx, cy - 18.0f, false);
        g.setGradientFill(getter);
        g.fillRect(cx - 13.0f, cy - 38.0f, 26.0f, 22.0f);
    }

    // Mica discs
    g.setColour(juce::Colour(0xAA888C96));
    g.fillRect(cx - 11.0f, cy - 18.0f, 22.0f, 2.2f);
    g.fillRect(cx - 11.0f, cy + 17.0f, 22.0f, 2.2f);

    // Dual triode plates & glowing orange heater filaments (#FF5E00 -> #FF8C1A)
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

    // Specular glass edge highlight
    g.setColour(juce::Colour(0x55FFFFFF));
    g.fillRect(cx - 9.5f, cy - 20.0f, 1.8f, 42.0f);

    g.setColour(juce::Colour(0x88CCCCCC));
    g.strokePath(glass, juce::PathStrokeType(1.2f));
}

void TubeCageComponent::paint(juce::Graphics& g) {
    auto b = getLocalBounds().toFloat();

    // Recessed dark cage interior (#070709)
    g.setColour(juce::Colour(0xFF070709));
    g.fillRoundedRectangle(b, 6.0f);

    // Subtle warm interior backplate wash
    const float effectiveGlow = juce::jlimit(0.15f, 1.0f, glow * 0.85f + drive * 0.25f);
    juce::ColourGradient backGlow(
        juce::Colour(0xFFFF6A00).withAlpha(effectiveGlow * 0.22f), b.getCentreX() + 20.0f, b.getCentreY(),
        juce::Colours::transparentBlack, b.getRight(), b.getCentreY(), true);
    g.setGradientFill(backGlow);
    g.fillRoundedRectangle(b, 6.0f);

    const float cy = b.getCentreY() + 2.0f;

    // Center-Left: Upright glowing neon amber-orange nose
    drawUprightNeonNose(g, b.getCentreX() - 96.0f, cy);

    // Center-Right: Row of 4 glowing 12AX7 vacuum tubes immediately right of the nose
    const float startTubeX = b.getCentreX() - 24.0f;
    const float spacing = 46.0f;
    for (int t = 0; t < 4; ++t) {
        const float tubeVariation = 1.0f - (static_cast<float>(t) * 0.04f);
        drawVacuumTube(g, startTubeX + static_cast<float>(t) * spacing, cy, effectiveGlow * tubeVariation);
    }

    // Dark metallic wire-mesh grille across the cage window
    {
        juce::Graphics::ScopedSaveState clip(g);
        g.reduceClipRegion(b.toNearestInt());

        g.setColour(juce::Colour(0x58141418));
        const float step = 6.0f;
        for (float d = -b.getHeight(); d < b.getWidth() + b.getHeight(); d += step) {
            g.drawLine(b.getX() + d, b.getY(), b.getX() + d - b.getHeight(), b.getBottom(), 1.0f);
            g.drawLine(b.getX() + d - b.getHeight(), b.getY(), b.getX() + d, b.getBottom(), 1.0f);
        }

        // Inner cage shadow vignette
        g.setColour(juce::Colour(0xAA000000));
        g.drawRoundedRectangle(b.reduced(0.5f), 6.0f, 3.0f);
    }

    // Metallic grille bezel border
    g.setColour(juce::Colour(0xFF34312C));
    g.drawRoundedRectangle(b, 6.0f, 1.6f);
}

// ==============================================================================

TubeyBloomyAudioProcessorEditor::TubeyBloomyAudioProcessorEditor(TubeyBloomyAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    setLookAndFeel(&ampLookAndFeel);
    addAndMakeVisible(contentContainer);

    // Top Vacuum Tube Cage & Upright Neon Nose
    contentContainer.addAndMakeVisible(tubeCage);

    // Faceplate Header Bar
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

    // Helper to configure each rotary knob unit
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

    // Bottom 3 Preset Strip
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

    // Update 7-segment style digital readouts
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

    // Preset strip states
    const int slot = audioProcessor.activeSlot;
    assBtn.setSelected(slot == 0);
    bootyBtn.setSelected(slot == 1);
    crackBtn.setSelected(slot == 2);

    // Window size pill states
    const int idx = audioProcessor.uiScaleIndex;
    scale1xBtn.setSelected(idx == 0);
    scale1_5xBtn.setSelected(idx == 1);
    scale2xBtn.setSelected(idx == 2);

    repaint();
}

void TubeyBloomyAudioProcessorEditor::timerCallback() {
    const bool isMasterOn = audioProcessor.apvts.getRawParameterValue("bypass")->load() < 0.5f;
    const float rawLevel  = isMasterOn ? audioProcessor.currentInputLevel.load(std::memory_order_relaxed) : 0.0f;
    const float driveNorm = isMasterOn ? (static_cast<float>(driveUnit.slider.getValue()) / 24.0f) : 0.0f;

    const float targetGlow = isMasterOn
        ? juce::jlimit(0.0f, 1.0f, std::pow(rawLevel * 18.0f, 0.38f))
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

    // Subtle pebbled tolex texture across outer cabinet
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
    const juce::Rectangle<float> faceplate(14.0f, 126.0f, w - 28.0f, h - 140.0f);
    juce::ColourGradient fpGrad(
        juce::Colour(0xFF141416), faceplate.getX(), faceplate.getY(),
        juce::Colour(0xFF0D0D0F), faceplate.getX(), faceplate.getBottom(), false);
    g.setGradientFill(fpGrad);
    g.fillRoundedRectangle(faceplate, 6.0f);

    // Subtle horizontal brushed metal grain lines
    g.setColour(juce::Colour(0x07FFFFFF));
    for (float gy = faceplate.getY() + 3.0f; gy < faceplate.getBottom() - 2.0f; gy += 3.0f) {
        g.drawHorizontalLine(static_cast<int>(gy), faceplate.getX() + 4.0f, faceplate.getRight() - 4.0f);
    }

    // Faceplate outer metallic trim
    g.setColour(juce::Colour(0xFF2E2C28));
    g.drawRoundedRectangle(faceplate, 6.0f, 1.5f);

    // 3. Glowing Amber Power/Active LED next to Bypass Switch (#FF8C1A active / #2A1210 bypassed)
    const bool isMasterOn = audioProcessor.apvts.getRawParameterValue("bypass")->load() < 0.5f;
    const float ledX = 56.0f;
    const float ledY = 149.0f;
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

    // 4. Two Separate Side-by-Side Silkscreen Containers (cornerSize = 8.0f, 2px solid #E6E2D6 outline)
    const juce::Rectangle<float> leftBox(28.0f, 174.0f, 466.0f, 184.0f);
    const juce::Rectangle<float> rightBox(506.0f, 174.0f, 126.0f, 184.0f);

    g.setColour(juce::Colour(0xFF101013));
    g.fillRoundedRectangle(leftBox, 8.0f);
    g.fillRoundedRectangle(rightBox, 8.0f);

    g.setColour(juce::Colour(0xFFE6E2D6));
    g.drawRoundedRectangle(leftBox, 8.0f, 2.0f);
    g.drawRoundedRectangle(rightBox, 8.0f, 2.0f);
}

void TubeyBloomyAudioProcessorEditor::resized() {
    contentContainer.setBounds(0, 0, baseWidth, baseHeight);

    // Top Vacuum Tube Cage & Upright
