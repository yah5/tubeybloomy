#include "PluginEditor.h"

void TubeSnifferCenterpiece::drawRealistic12AX7(juce::Graphics& g, float cx, float cy, float scale) {
    juce::Graphics::ScopedSaveState state(g);
    g.addTransform(juce::AffineTransform::translation(-cx, -cy)
                       .scaled(scale, scale)
                       .translated(cx, cy));

    // Prominent warm orange radial glow radiating behind & around the 12AX7 tube
    if (glow > 0.005f) {
        const float ambientRadius = 112.0f + (glow * 38.0f);
        juce::ColourGradient ambientHalo(
            juce::Colour(0xFFFF6A00).withAlpha(juce::jlimit(0.0f, 1.0f, glow * 0.72f)), cx, cy - 4.0f,
            juce::Colours::transparentBlack, cx + ambientRadius, cy - 4.0f, true);
        g.setGradientFill(ambientHalo);
        g.fillEllipse(cx - ambientRadius, cy - 4.0f - ambientRadius, ambientRadius * 2.0f, ambientRadius * 2.0f);

        const float midRadius = 70.0f + (glow * 24.0f);
        juce::ColourGradient midHalo(
            juce::Colour(0xFFFF9824).withAlpha(juce::jlimit(0.0f, 1.0f, glow * 0.90f)), cx, cy - 3.0f,
            juce::Colours::transparentBlack, cx + midRadius, cy - 3.0f, true);
        g.setGradientFill(midHalo);
        g.fillEllipse(cx - midRadius, cy - 3.0f - midRadius, midRadius * 2.0f, midRadius * 2.0f);

        const float innerRadius = 42.0f + (glow * 14.0f);
        juce::ColourGradient innerCore(
            juce::Colour(0xFFFFE082).withAlpha(juce::jlimit(0.0f, 1.0f, glow * 0.98f)), cx, cy - 2.0f,
            juce::Colours::transparentBlack, cx + innerRadius, cy - 2.0f, true);
        g.setGradientFill(innerCore);
        g.fillEllipse(cx - innerRadius, cy - 2.0f - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
    }

    // 9-pin miniature base pins
    const float pinOffsets[] = { -16.0f, -10.5f, -5.0f, 0.0f, 5.0f, 10.5f, 16.0f };
    for (float px : pinOffsets) {
        g.setColour(juce::Colour(0xFF8C8C96));
        g.fillRoundedRectangle(cx + px - 1.3f, cy + 44.0f, 2.6f, 12.0f, 1.0f);
        g.setColour(juce::Colour(0xFFD6D6E0));
        g.fillRect(cx + px - 0.5f, cy + 44.0f, 1.0f, 11.0f);
    }

    // Glass button base ridge
    g.setColour(juce::Colour(0x66B8C8D8));
    g.fillRoundedRectangle(cx - 22.0f, cy + 37.0f, 44.0f, 8.0f, 3.0f);
    g.setColour(juce::Colour(0x99FFFFFF));
    g.drawRoundedRectangle(cx - 22.0f, cy + 37.0f, 44.0f, 8.0f, 3.0f, 1.0f);

    // Clear glass 12AX7 envelope
    juce::Path glassBody;
    glassBody.startNewSubPath(cx - 21.0f, cy + 38.0f);
    glassBody.lineTo(cx - 21.0f, cy - 34.0f);
    glassBody.cubicTo(cx - 21.0f, cy - 46.0f, cx - 8.0f, cy - 50.0f, cx - 4.0f, cy - 52.0f);
    glassBody.lineTo(cx - 3.5f, cy - 57.0f);
    glassBody.lineTo(cx + 3.5f, cy - 57.0f);
    glassBody.lineTo(cx + 4.0f, cy - 52.0f);
    glassBody.cubicTo(cx + 8.0f, cy - 50.0f, cx + 21.0f, cy - 46.0f, cx + 21.0f, cy - 34.0f);
    glassBody.lineTo(cx + 21.0f, cy + 38.0f);
    glassBody.closeSubPath();

    // Warm inner glass illumination tint when signal arrives
    const juce::Colour coldGlass(0x28DCE8F5);
    const juce::Colour warmGlass(0x66FF9E2C);
    g.setColour(coldGlass.interpolatedWith(warmGlass, glow));
    g.fillPath(glassBody);

    // Silver getter flash at top dome
    {
        juce::Graphics::ScopedSaveState getterClip(g);
        g.reduceClipRegion(glassBody);
        juce::ColourGradient getterGrad(
            juce::Colour(0xCC9FA6B2), cx, cy - 56.0f,
            juce::Colour(0x009FA6B2), cx, cy - 28.0f, false);
        g.setGradientFill(getterGrad);
        g.fillRect(cx - 22.0f, cy - 58.0f, 44.0f, 32.0f);
    }

    // Internal vertical support rods
    g.setColour(juce::Colour(0xFF9A9AA4));
    g.fillRect(cx - 14.5f, cy - 32.0f, 1.6f, 68.0f);
    g.fillRect(cx + 12.9f, cy - 32.0f, 1.6f, 68.0f);
    g.fillRect(cx - 0.8f,  cy - 30.0f, 1.6f, 66.0f);

    // Mica spacers
    g.setColour(juce::Colour(0xCC8E929E));
    g.fillRoundedRectangle(cx - 19.5f, cy - 28.0f, 39.0f, 3.5f, 1.5f);
    g.fillRoundedRectangle(cx - 19.5f, cy + 25.0f, 39.0f, 3.5f, 1.5f);

    // Getter ring
    g.setColour(juce::Colour(0xFFB0B4BE));
    g.drawEllipse(cx - 9.0f, cy - 41.0f, 18.0f, 5.0f, 1.5f);

    // Dual-triode 12AX7 gray anode plates
    auto drawTriodeSection = [&](float plateX) {
        g.setColour(juce::Colour(0xFF3A3C44));
        g.fillRoundedRectangle(plateX, cy - 21.0f, 11.5f, 42.0f, 2.0f);
        g.setColour(juce::Colour(0xFF5A5D68));
        g.drawRoundedRectangle(plateX, cy - 21.0f, 11.5f, 42.0f, 2.0f, 1.0f);

        g.setColour(juce::Colour(0xFF282A30));
        g.fillRect(plateX + 2.5f, cy - 16.0f, 6.5f, 32.0f);

        const juce::Colour idleGlow(0xFF7A2D10);
        const juce::Colour activeGlow(0xFFFFE57F);
        const juce::Colour heaterCol = idleGlow.interpolatedWith(activeGlow, juce::jlimit(0.0f, 1.0f, 0.30f + glow * 0.95f));

        g.setColour(heaterCol);
        g.fillRoundedRectangle(plateX + 3.8f, cy - 25.0f, 3.8f, 5.0f, 1.5f);
        g.fillRoundedRectangle(plateX + 3.8f, cy + 20.0f, 3.8f, 5.0f, 1.5f);

        g.setColour(heaterCol.withAlpha(0.50f + glow * 0.50f));
        g.fillRect(plateX + 4.8f, cy - 14.0f, 2.0f, 28.0f);
    };

    drawTriodeSection(cx - 13.5f);
    drawTriodeSection(cx + 2.0f);

    // Glass highlights & outline
    g.setColour(juce::Colour(0x66FFFFFF));
    g.fillRoundedRectangle(cx - 17.0f, cy - 32.0f, 3.0f, 64.0f, 1.5f);
    g.setColour(juce::Colour(0x33FFFFFF));
    g.fillRoundedRectangle(cx + 13.5f, cy - 28.0f, 2.0f, 58.0f, 1.0f);

    g.setColour(juce::Colour(0x99FFFFFF));
    g.strokePath(glassBody, juce::PathStrokeType(1.6f));
}

void TubeSnifferCenterpiece::drawClipArtNose(juce::Graphics& g, float cx, float cy, bool facingRight, float swayX, float swayAngle) {
    juce::Graphics::ScopedSaveState state(g);

    const float dir = facingRight ? 1.0f : -1.0f;
    g.addTransform(juce::AffineTransform::translation(-cx, -cy)
                       .scaled(dir, 1.0f)
                       .rotated(swayAngle * dir)
                       .translated(cx + swayX, cy));

    juce::Path nosePath;
    nosePath.startNewSubPath(cx - 14.0f, cy - 24.0f);
    nosePath.lineTo(cx - 2.0f, cy - 8.0f);
    nosePath.cubicTo(cx + 14.0f, cy + 4.0f,
                     cx + 18.0f, cy + 16.0f,
                     cx + 6.0f,  cy + 21.0f);
    nosePath.cubicTo(cx - 2.0f,  cy + 24.0f,
                     cx - 10.0f, cy + 21.0f,
                     cx - 15.0f, cy + 17.0f);
    nosePath.closeSubPath();

    g.setColour(juce::Colour(0xFFF3B38B));
    g.fillPath(nosePath);

    g.setColour(juce::Colour(0xFFFFD6BA));
    g.fillEllipse(cx - 4.0f, cy - 2.0f, 9.0f, 13.0f);

    g.setColour(juce::Colour(0xFF6E3B26));
    g.fillEllipse(cx - 4.0f, cy + 13.0f, 7.5f, 4.5f);

    g.setColour(juce::Colour(0xFF121214));
    g.strokePath(nosePath, juce::PathStrokeType(2.4f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    if (glow > 0.06f) {
        g.setColour(juce::Colour(0xFFFF8C24).withAlpha(glow * 0.90f));
        g.drawLine(cx + 16.0f, cy + 4.0f,  cx + 23.0f, cy + 1.0f,  1.8f);
        g.drawLine(cx + 18.0f, cy + 11.0f, cx + 25.0f, cy + 11.0f, 1.8f);
        g.drawLine(cx + 15.0f, cy + 18.0f, cx + 22.0f, cy + 21.0f, 1.8f);
    }
}

void TubeSnifferCenterpiece::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().toFloat();
    const float cx = bounds.getCentreX();
    const float cy = bounds.getCentreY() + 2.0f;

    drawRealistic12AX7(g, cx, cy, 1.20f);

    const float swayOffset = std::sin(phase) * (glow * 10.0f);
    const float swayTilt   = std::sin(phase) * (glow * 0.18f);

    drawClipArtNose(g, cx - 76.0f, cy + 4.0f, true,  swayOffset,  swayTilt);
    drawClipArtNose(g, cx + 76.0f, cy + 4.0f, false, -swayOffset, -swayTilt);
}

// ==============================================================================

TubeyBloomyAudioProcessorEditor::TubeyBloomyAudioProcessorEditor(TubeyBloomyAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(contentContainer);

    // Title: "Tube Sniffer" (default JUCE font)
    titleLabel.setText("Tube Sniffer", juce::dontSendNotification);
    juce::Font titleFont(juce::FontOptions(25.0f));
    titleFont.setBold(true);
    titleLabel.setFont(titleFont);
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF8C24));
    contentContainer.addAndMakeVisible(titleLabel);

    // Top-right Window Size controls matching compact label height
    sizeLabel.setText("Window Size", juce::dontSendNotification);
    sizeLabel.setFont(juce::Font(juce::FontOptions(12.0f)));
    sizeLabel.setJustificationType(juce::Justification::centredRight);
    sizeLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFB8B8C2));
    contentContainer.addAndMakeVisible(sizeLabel);

    auto setupScaleBtn = [this](juce::TextButton& btn, int idx) {
        btn.onClick = [this, idx] { setUiScaleIndex(idx); };
        contentContainer.addAndMakeVisible(btn);
    };
    setupScaleBtn(scale1xBtn,   0);
    setupScaleBtn(scale1_5xBtn, 1);
    setupScaleBtn(scale2xBtn,   2);

    contentContainer.addAndMakeVisible(centerpiece);

    masterPowerBtn.onClick = [this] { toggleMasterPower(); };
    contentContainer.addAndMakeVisible(masterPowerBtn);

    auto setupSlider = [this](juce::Slider& s, juce::Label& l, const juce::String& name, const juce::String& suffix) {
        s.setSliderStyle(juce::Slider::LinearHorizontal);
        s.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 24);
        s.setTextValueSuffix(suffix);
        s.setNumDecimalPlacesToDisplay(0);
        s.onValueChange = [this] { handleUserSliderChange(); };
        contentContainer.addAndMakeVisible(s);

        l.setText(name, juce::dontSendNotification);
        juce::Font lblFont(juce::FontOptions(14.0f));
        lblFont.setBold(true);
        l.setFont(lblFont);
        l.setColour(juce::Label::textColourId, juce::Colour(0xFFE4E4EC));
        contentContainer.addAndMakeVisible(l);
    };

    setupSlider(driveSlider,     driveLabel,     "Tube Drive",         " dB");
    setupSlider(sagSlider,       sagLabel,       "Dynamic Sag / Feel", "");
    setupSlider(harmonicsSlider, harmonicsLabel, "Harmonic Richness",  "");
    setupSlider(tightSlider,     tightLabel,     "Low-End Tighten",    " Hz");
    setupSlider(outputSlider,    outputLabel,    "Output Level",       "");

    outputPowerBtn.onClick = [this] { toggleOutputPower(); };
    contentContainer.addAndMakeVisible(outputPowerBtn);

    auto setupSlotBtn = [this](BoldFirstLetterButton& btn, int idx) {
        btn.onClick = [this, idx] { selectSlot(idx); };
        contentContainer.addAndMakeVisible(btn);
    };
    setupSlotBtn(assBtn,   0);
    setupSlotBtn(bootyBtn, 1);
    setupSlotBtn(crackBtn, 2);

    driveAttach     = std::make_unique<Attachment>(audioProcessor.apvts, "drive",     driveSlider);
    sagAttach       = std::make_unique<Attachment>(audioProcessor.apvts, "sag",       sagSlider);
    harmonicsAttach = std::make_unique<Attachment>(audioProcessor.apvts, "harmonics", harmonicsSlider);
    tightAttach     = std::make_unique<Attachment>(audioProcessor.apvts, "tight",     tightSlider);
    outputAttach    = std::make_unique<Attachment>(audioProcessor.apvts, "output",    outputSlider);

    updateButtonStates();
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

    updateButtonStates();
}

void TubeyBloomyAudioProcessorEditor::toggleMasterPower() {
    if (auto* param = audioProcessor.apvts.getParameter("bypass")) {
        const bool currentlyBypassed = param->getValue() > 0.5f;
        param->beginChangeGesture();
        param->setValueNotifyingHost(currentlyBypassed ? 0.0f : 1.0f);
        param->endChangeGesture();
    }
    updateButtonStates();
}

void TubeyBloomyAudioProcessorEditor::toggleOutputPower() {
    if (auto* param = audioProcessor.apvts.getParameter("output")) {
        const float currentOut = static_cast<float>(outputSlider.getValue());
        param->beginChangeGesture();
        if (currentOut < 0.5f) {
            const float restoreVal = juce::jlimit(1.0f, 12.0f, audioProcessor.lastNonZeroOutput);
            param->setValueNotifyingHost(param->convertTo0to1(restoreVal));
        } else {
            audioProcessor.lastNonZeroOutput = currentOut;
            param->setValueNotifyingHost(param->convertTo0to1(0.0f));
        }
        param->endChangeGesture();
    }
    updateButtonStates();
}

void TubeyBloomyAudioProcessorEditor::selectSlot(int slotIndex) {
    // Save current Drive, Sag, Harmonic Richness, and Tightness (leave Output Level untouched)
    const int prevSlot = audioProcessor.activeSlot;
    if (prevSlot >= 0 && prevSlot < 3) {
        audioProcessor.slotMemory[static_cast<size_t>(prevSlot)] = {
            static_cast<float>(driveSlider.getValue()),
            static_cast<float>(sagSlider.getValue()),
            static_cast<float>(harmonicsSlider.getValue()),
            static_cast<float>(tightSlider.getValue())
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
    updateButtonStates();
}

void TubeyBloomyAudioProcessorEditor::handleUserSliderChange() {
    const float currentOut = static_cast<float>(outputSlider.getValue());
    if (currentOut >= 0.5f)
        audioProcessor.lastNonZeroOutput = currentOut;

    if (!isLoadingSlot) {
        const int slot = audioProcessor.activeSlot;
        if (slot >= 0 && slot < 3) {
            audioProcessor.slotMemory[static_cast<size_t>(slot)] = {
                static_cast<float>(driveSlider.getValue()),
                static_cast<float>(sagSlider.getValue()),
                static_cast<float>(harmonicsSlider.getValue()),
                static_cast<float>(tightSlider.getValue())
            };
        }
    }

    updateButtonStates();
}

void TubeyBloomyAudioProcessorEditor::updateButtonStates() {
    const bool isMasterOn = audioProcessor.apvts.getRawParameterValue("bypass")->load() < 0.5f;
    masterPowerBtn.setButtonText(isMasterOn ? "ON" : "OFF");
    masterPowerBtn.setColour(juce::TextButton::buttonColourId,
                             isMasterOn ? juce::Colour(0xFFFF7A1A) : juce::Colour(0xFF24242A));
    masterPowerBtn.setColour(juce::TextButton::textColourOffId,
                             isMasterOn ? juce::Colour(0xFF121214) : juce::Colour(0xFFA0A0AC));

    const bool isOutputOn = outputSlider.getValue() >= 0.5;
    outputPowerBtn.setButtonText(isOutputOn ? "ON" : "OFF");
    outputPowerBtn.setColour(juce::TextButton::buttonColourId,
                             isOutputOn ? juce::Colour(0xFFFF7A1A) : juce::Colour(0xFF2A2A32));
    outputPowerBtn.setColour(juce::TextButton::textColourOffId,
                             isOutputOn ? juce::Colour(0xFF121214) : juce::Colour(0xFFA0A0AC));

    const int slot = audioProcessor.activeSlot;
    assBtn.setIsSelected(slot == 0);
    bootyBtn.setIsSelected(slot == 1);
    crackBtn.setIsSelected(slot == 2);

    const int idx = audioProcessor.uiScaleIndex;
    juce::TextButton* scaleBtns[] = { &scale1xBtn, &scale1_5xBtn, &scale2xBtn };
    for (int i = 0; i < 3; ++i) {
        const bool active = (i == idx);
        scaleBtns[i]->setColour(juce::TextButton::buttonColourId,
                                active ? juce::Colour(0xFFFF7A1A) : juce::Colour(0xFF24242A));
        scaleBtns[i]->setColour(juce::TextButton::textColourOffId,
                                active ? juce::Colour(0xFF121214) : juce::Colour(0xFFD0D0D8));
    }
}

void TubeyBloomyAudioProcessorEditor::timerCallback() {
    const bool isMasterOn = audioProcessor.apvts.getRawParameterValue("bypass")->load() < 0.5f;
    const float rawLevel  = isMasterOn ? audioProcessor.currentInputLevel.load(std::memory_order_relaxed) : 0.0f;

    // Very responsive input curve for a prominent warm glow
    const float targetGlow = juce::jlimit(0.0f, 1.0f, std::pow(rawLevel * 18.0f, 0.38f));

    if (targetGlow > smoothedGlow)
        smoothedGlow += (targetGlow - smoothedGlow) * 0.65f; // Fast attack
    else
        smoothedGlow += (targetGlow - smoothedGlow) * 0.05f; // Slow warm fade-out

    if (smoothedGlow > 0.01f) {
        sniffPhase += 0.28f + (smoothedGlow * 0.24f);
        if (sniffPhase > juce::MathConstants<float>::twoPi)
            sniffPhase -= juce::MathConstants<float>::twoPi;
    } else {
        sniffPhase = 0.0f;
    }

    centerpiece.setActivity(smoothedGlow, sniffPhase);
}

void TubeyBloomyAudioProcessorEditor::paint(juce::Graphics& g) {
    const float scale = getScaleFactor();
    g.addTransform(juce::AffineTransform::scale(scale));

    g.fillAll(juce::Colour(0xFF161619));

    g.setColour(juce::Colour(0xFF1E1E24));
    g.fillRoundedRectangle(22.0f, 248.0f, static_cast<float>(baseWidth - 44), 278.0f, 12.0f);
    g.setColour(juce::Colour(0xFF2E2E36));
    g.drawRoundedRectangle(22.0f, 248.0f, static_cast<float>(baseWidth - 44), 278.0f, 12.0f, 1.0f);
}

void TubeyBloomyAudioProcessorEditor::resized() {
    contentContainer.setBounds(0, 0, baseWidth, baseHeight);
    auto area = juce::Rectangle<int>(0, 0, baseWidth, baseHeight).reduced(24, 14);

    // Top bar: Title on left, compact Window Size [1x] [1.5x] [2x] matching label height
    auto topBar = area.removeFromTop(34);
    auto sizeArea = topBar.removeFromRight(186).withSizeKeepingCentre(186, 20);
    sizeLabel.setBounds(sizeArea.removeFromLeft(76));
    sizeArea.removeFromLeft(5);
    scale1xBtn.setBounds(sizeArea.removeFromLeft(30));
    sizeArea.removeFromLeft(3);
    scale1_5xBtn.setBounds(sizeArea.removeFromLeft(38));
    sizeArea.removeFromLeft(3);
    scale2xBtn.setBounds(sizeArea.removeFromLeft(30));

    titleLabel.setBounds(topBar);

    centerpiece.setBounds(area.removeFromTop(148));

    area.removeFromTop(4);

    auto powerRow = area.removeFromTop(30);
    masterPowerBtn.setBounds(powerRow.withSizeKeepingCentre(76, 28));

    area.removeFromTop(14);

    auto sliderArea = area.removeFromTop(268).reduced(10, 0);
    auto placeRow = [&](juce::Label& lbl, juce::Slider& sld, juce::TextButton* extraBtn = nullptr) {
        auto row = sliderArea.removeFromTop(52);
        auto labelRow = row.removeFromTop(22);
        if (extraBtn != nullptr) {
            lbl.setBounds(labelRow.removeFromLeft(96));
            extraBtn->setBounds(labelRow.removeFromLeft(44).reduced(0, 1));
        } else {
            lbl.setBounds(labelRow);
        }
        sld.setBounds(row.removeFromTop(28));
    };

    placeRow(driveLabel,     driveSlider);
    placeRow(sagLabel,       sagSlider);
    placeRow(harmonicsLabel, harmonicsSlider);
    placeRow(tightLabel,     tightSlider);
    placeRow(outputLabel,    outputSlider, &outputPowerBtn);

    area.removeFromTop(12);

    auto buttonRow = area.removeFromTop(34);
    const int gap = 10;
    const int btnWidth = (buttonRow.getWidth() - (gap * 2)) / 3;
    assBtn.setBounds(buttonRow.removeFromLeft(btnWidth));
    buttonRow.removeFromLeft(gap);
    bootyBtn.setBounds(buttonRow.removeFromLeft(btnWidth));
    buttonRow.removeFromLeft(gap);
    crackBtn.setBounds(buttonRow);
}
