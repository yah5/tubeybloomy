#include "PluginEditor.h"

void TubeSnifferCenterpiece::drawRealistic12AX7(juce::Graphics& g, float cx, float cy, float scale) {
    juce::Graphics::ScopedSaveState state(g);
    g.addTransform(juce::AffineTransform::translation(-cx, -cy)
                       .scaled(scale, scale)
                       .translated(cx, cy));

    // Warm orange radial glow radiating out behind the 12AX7 tube
    if (glow > 0.005f) {
        const float outerRadius = 76.0f + (glow * 24.0f);
        juce::ColourGradient outerHalo(
            juce::Colour(0xFFFF7A1A).withAlpha(glow * 0.55f), cx, cy - 4.0f,
            juce::Colours::transparentBlack, cx + outerRadius, cy - 4.0f, true);
        g.setGradientFill(outerHalo);
        g.fillEllipse(cx - outerRadius, cy - 4.0f - outerRadius, outerRadius * 2.0f, outerRadius * 2.0f);

        const float innerRadius = 38.0f + (glow * 10.0f);
        juce::ColourGradient innerCore(
            juce::Colour(0xFFFFB84D).withAlpha(glow * 0.65f), cx, cy - 2.0f,
            juce::Colours::transparentBlack, cx + innerRadius, cy - 2.0f, true);
        g.setGradientFill(innerCore);
        g.fillEllipse(cx - innerRadius, cy - 2.0f - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
    }

    // 9-pin miniature base pins (steel/nickel)
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

    // Outer clear glass envelope (classic 12AX7 tall cylindrical silhouette)
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

    // Subtle smoky glass interior tint
    g.setColour(juce::Colour(0x28DCE8F5));
    g.fillPath(glassBody);

    // Silver getter flash coating at top dome of 12AX7
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

    // Top and bottom mica insulator discs (serrated translucent wafers)
    g.setColour(juce::Colour(0xCC8E929E));
    g.fillRoundedRectangle(cx - 19.5f, cy - 28.0f, 39.0f, 3.5f, 1.5f);
    g.fillRoundedRectangle(cx - 19.5f, cy + 25.0f, 39.0f, 3.5f, 1.5f);

    // Getter ring halo at top
    g.setColour(juce::Colour(0xFFB0B4BE));
    g.drawEllipse(cx - 9.0f, cy - 41.0f, 18.0f, 5.0f, 1.5f);

    // Dual-triode 12AX7 gray anode plates (left & right sections)
    auto drawTriodeSection = [&](float plateX) {
        // Plate metal body
        g.setColour(juce::Colour(0xFF3A3C44));
        g.fillRoundedRectangle(plateX, cy - 21.0f, 11.5f, 42.0f, 2.0f);
        g.setColour(juce::Colour(0xFF5A5D68));
        g.drawRoundedRectangle(plateX, cy - 21.0f, 11.5f, 42.0f, 2.0f, 1.0f);

        // Plate cooling ribs
        g.setColour(juce::Colour(0xFF282A30));
        g.fillRect(plateX + 2.5f, cy - 16.0f, 6.5f, 32.0f);

        // Glowing cathode & heater tips emerging top and bottom of each triode
        const juce::Colour idleGlow(0xFF6B280E);
        const juce::Colour activeGlow(0xFFFFC44D);
        const juce::Colour heaterCol = idleGlow.interpolatedWith(activeGlow, juce::jlimit(0.0f, 1.0f, 0.25f + glow * 0.95f));

        g.setColour(heaterCol);
        g.fillRoundedRectangle(plateX + 3.8f, cy - 25.0f, 3.8f, 5.0f, 1.5f);
        g.fillRoundedRectangle(plateX + 3.8f, cy + 20.0f, 3.8f, 5.0f, 1.5f);

        // Internal heater glow visible through plate slot
        g.setColour(heaterCol.withAlpha(0.35f + glow * 0.65f));
        g.fillRect(plateX + 4.8f, cy - 14.0f, 2.0f, 28.0f);
    };

    drawTriodeSection(cx - 13.5f);
    drawTriodeSection(cx + 2.0f);

    // Glass specular highlights & crisp outline
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

    // Cartoon/clip-art side-profile nose facing right toward the tube
    juce::Path nosePath;
    nosePath.startNewSubPath(cx - 14.0f, cy - 24.0f); // Bridge top
    nosePath.lineTo(cx - 2.0f, cy - 8.0f);           // Bridge slope
    nosePath.cubicTo(cx + 14.0f, cy + 4.0f,
                     cx + 18.0f, cy + 16.0f,
                     cx + 6.0f,  cy + 21.0f);        // Rounded nose tip
    nosePath.cubicTo(cx - 2.0f,  cy + 24.0f,
                     cx - 10.0f, cy + 21.0f,
                     cx - 15.0f, cy + 17.0f);        // Base & nostril curve
    nosePath.closeSubPath();

    // Warm skin-tone clip-art fill + shading
    g.setColour(juce::Colour(0xFFF3B38B));
    g.fillPath(nosePath);

    // Soft highlight on bridge
    g.setColour(juce::Colour(0xFFFFD6BA));
    g.fillEllipse(cx - 4.0f, cy - 2.0f, 9.0f, 13.0f);

    // Nostril
    g.setColour(juce::Colour(0xFF6E3B26));
    g.fillEllipse(cx - 4.0f, cy + 13.0f, 7.5f, 4.5f);

    // Bold black clip-art outline
    g.setColour(juce::Colour(0xFF121214));
    g.strokePath(nosePath, juce::PathStrokeType(2.4f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Little sniff motion lines when active
    if (glow > 0.08f) {
        g.setColour(juce::Colour(0xFFFF8C24).withAlpha(glow * 0.85f));
        g.drawLine(cx + 16.0f, cy + 4.0f,  cx + 23.0f, cy + 1.0f,  1.8f);
        g.drawLine(cx + 18.0f, cy + 11.0f, cx + 25.0f, cy + 11.0f, 1.8f);
        g.drawLine(cx + 15.0f, cy + 18.0f, cx + 22.0f, cy + 21.0f, 1.8f);
    }
}

void TubeSnifferCenterpiece::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().toFloat();
    const float cx = bounds.getCentreX();
    const float cy = bounds.getCentreY() + 2.0f;

    // 1. Realistic 12AX7 tube scaled 20% larger (1.20f)
    drawRealistic12AX7(g, cx, cy, 1.20f);

    // 2. Two clip-art noses on either side swaying left/right like sniffing the tube
    const float swayOffset = std::sin(phase) * (glow * 9.0f);
    const float swayTilt   = std::sin(phase) * (glow * 0.16f);

    drawClipArtNose(g, cx - 72.0f, cy + 4.0f, true,  swayOffset,  swayTilt);
    drawClipArtNose(g, cx + 72.0f, cy + 4.0f, false, -swayOffset, -swayTilt);
}

// ==============================================================================

TubeyBloomyAudioProcessorEditor::TubeyBloomyAudioProcessorEditor(TubeyBloomyAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(contentContainer);

    // Title: "Tube Sniffer"
    titleLabel.setText("Tube Sniffer", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(juce::FontOptions(26.0f)));
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF8C24));
    contentContainer.addAndMakeVisible(titleLabel);

    // Top-right Size controls: Size [1x] [2x] [3x]
    sizeLabel.setText("Size", juce::dontSendNotification);
    sizeLabel.setFont(juce::Font(juce::FontOptions(13.0f)));
    sizeLabel.setJustificationType(juce::Justification::centredRight);
    sizeLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFB8B8C2));
    contentContainer.addAndMakeVisible(sizeLabel);

    auto setupScaleBtn = [this](juce::TextButton& btn, int mult) {
        btn.onClick = [this, mult] { setUiScale(mult); };
        contentContainer.addAndMakeVisible(btn);
    };
    setupScaleBtn(scale1xBtn, 1);
    setupScaleBtn(scale2xBtn, 2);
    setupScaleBtn(scale3xBtn, 3);

    // Centerpiece: 12AX7 tube + 2 sniffing clip-art noses
    contentContainer.addAndMakeVisible(centerpiece);

    // 3 Custom Memory Buttons: Ass - Booty - Crack
    auto setupSlotBtn = [this](BoldFirstLetterButton& btn, int idx) {
        btn.onClick = [this, idx] { selectSlot(idx); };
        contentContainer.addAndMakeVisible(btn);
    };
    setupSlotBtn(assBtn,   0);
    setupSlotBtn(bootyBtn, 1);
    setupSlotBtn(crackBtn, 2);

    // Sliders
    auto setupSlider = [this](juce::Slider& s, juce::Label& l, const juce::String& name, const juce::String& suffix) {
        s.setSliderStyle(juce::Slider::LinearHorizontal);
        s.setTextBoxStyle(juce::Slider::TextBoxRight, false, 58, 24);
        s.setTextValueSuffix(suffix);
        s.setNumDecimalPlacesToDisplay(0);
        s.onValueChange = [this] { handleUserSliderChange(); };
        contentContainer.addAndMakeVisible(s);

        l.setText(name, juce::dontSendNotification);
        l.setFont(juce::Font(juce::FontOptions(14.0f)));
        l.setColour(juce::Label::textColourId, juce::Colour(0xFFE4E4EC));
        contentContainer.addAndMakeVisible(l);
    };

    setupSlider(driveSlider,  driveLabel,  "Tube Drive",         " dB");
    setupSlider(sagSlider,    sagLabel,    "Dynamic Sag / Feel", "");
    setupSlider(tightSlider,  tightLabel,  "Low-End Tighten",    " Hz");
    setupSlider(outputSlider, outputLabel, "Output Level",       " dB");

    driveAttach  = std::make_unique<Attachment>(audioProcessor.apvts, "drive",  driveSlider);
    sagAttach    = std::make_unique<Attachment>(audioProcessor.apvts, "sag",    sagSlider);
    tightAttach  = std::make_unique<Attachment>(audioProcessor.apvts, "tight",  tightSlider);
    outputAttach = std::make_unique<Attachment>(audioProcessor.apvts, "output", outputSlider);

    updateButtonStates();
    setUiScale(audioProcessor.uiScaleMultiplier);
    startTimerHz(30);
}

TubeyBloomyAudioProcessorEditor::~TubeyBloomyAudioProcessorEditor() {
    stopTimer();
    setLookAndFeel(nullptr);
}

void TubeyBloomyAudioProcessorEditor::setUiScale(int multiplier) {
    audioProcessor.uiScaleMultiplier = juce::jlimit(1, 3, multiplier);
    const float scale = static_cast<float>(audioProcessor.uiScaleMultiplier);

    contentContainer.setBounds(0, 0, baseWidth, baseHeight);
    contentContainer.setTransform(juce::AffineTransform::scale(scale));
    setSize(baseWidth * audioProcessor.uiScaleMultiplier,
            baseHeight * audioProcessor.uiScaleMultiplier);

    updateButtonStates();
}

void TubeyBloomyAudioProcessorEditor::selectSlot(int slotIndex) {
    // Save current slider values to the previously active slot before switching
    const int prevSlot = audioProcessor.activeSlot;
    if (prevSlot >= 0 && prevSlot < 3) {
        audioProcessor.slotMemory[static_cast<size_t>(prevSlot)] = {
            static_cast<float>(driveSlider.getValue()),
            static_cast<float>(sagSlider.getValue()),
            static_cast<float>(tightSlider.getValue()),
            static_cast<float>(outputSlider.getValue())
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

    setParam("drive",  target.drive);
    setParam("sag",    target.sag);
    setParam("tight",  target.tight);
    setParam("output", target.output);

    isLoadingSlot = false;
    updateButtonStates();
}

void TubeyBloomyAudioProcessorEditor::handleUserSliderChange() {
    if (isLoadingSlot)
        return;

    const int slot = audioProcessor.activeSlot;
    if (slot >= 0 && slot < 3) {
        audioProcessor.slotMemory[static_cast<size_t>(slot)] = {
            static_cast<float>(driveSlider.getValue()),
            static_cast<float>(sagSlider.getValue()),
            static_cast<float>(tightSlider.getValue()),
            static_cast<float>(outputSlider.getValue())
        };
    }
}

void TubeyBloomyAudioProcessorEditor::updateButtonStates() {
    const int slot = audioProcessor.activeSlot;
    assBtn.setIsSelected(slot == 0);
    bootyBtn.setIsSelected(slot == 1);
    crackBtn.setIsSelected(slot == 2);

    const int mult = audioProcessor.uiScaleMultiplier;
    juce::TextButton* scaleBtns[] = { &scale1xBtn, &scale2xBtn, &scale3xBtn };
    for (int i = 0; i < 3; ++i) {
        const bool active = ((i + 1) == mult);
        scaleBtns[i]->setColour(juce::TextButton::buttonColourId,
                                active ? juce::Colour(0xFFFF7A1A) : juce::Colour(0xFF24242A));
        scaleBtns[i]->setColour(juce::TextButton::textColourOffId,
                                active ? juce::Colour(0xFF121214) : juce::Colour(0xFFD0D0D8));
    }
}

void TubeyBloomyAudioProcessorEditor::timerCallback() {
    const float rawLevel = audioProcessor.currentInputLevel.load(std::memory_order_relaxed);
    const float targetGlow = juce::jlimit(0.0f, 1.0f, std::pow(rawLevel * 3.0f, 0.65f));

    // Light up quickly when input signal arrives; fade out slowly when silent
    if (targetGlow > smoothedGlow)
        smoothedGlow += (targetGlow - smoothedGlow) * 0.55f; // Fast attack
    else
        smoothedGlow += (targetGlow - smoothedGlow) * 0.055f; // Slow, warm tube fade-out

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
    const float scale = static_cast<float>(audioProcessor.uiScaleMultiplier);
    g.addTransform(juce::AffineTransform::scale(scale));

    // Off-black background
    g.fillAll(juce::Colour(0xFF161619));

    // Slider panel card
    g.setColour(juce::Colour(0xFF1E1E24));
    g.fillRoundedRectangle(22.0f, 266.0f, static_cast<float>(baseWidth - 44), 232.0f, 12.0f);
    g.setColour(juce::Colour(0xFF2E2E36));
    g.drawRoundedRectangle(22.0f, 266.0f, static_cast<float>(baseWidth - 44), 232.0f, 12.0f, 1.0f);
}

void TubeyBloomyAudioProcessorEditor::resized() {
    contentContainer.setBounds(0, 0, baseWidth, baseHeight);
    auto area = juce::Rectangle<int>(0, 0, baseWidth, baseHeight).reduced(24, 16);

    // Top bar: Title on left, Size [1x] [2x] [3x] on top right
    auto topBar = area.removeFromTop(34);
    auto sizeArea = topBar.removeFromRight(142);
    sizeLabel.setBounds(sizeArea.removeFromLeft(36));
    sizeArea.removeFromLeft(4);
    scale1xBtn.setBounds(sizeArea.removeFromLeft(32).reduced(1, 4));
    scale2xBtn.setBounds(sizeArea.removeFromLeft(32).reduced(1, 4));
    scale3xBtn.setBounds(sizeArea.removeFromLeft(32).reduced(1, 4));

    titleLabel.setBounds(topBar);

    // 12AX7 tube + 2 sniffing noses centerpiece (+20% larger)
    centerpiece.setBounds(area.removeFromTop(152));

    area.removeFromTop(6);

    // 3 A/B/C custom buttons: Ass - Booty - Crack
    auto buttonRow = area.removeFromTop(34);
    const int gap = 10;
    const int btnWidth = (buttonRow.getWidth() - (gap * 2)) / 3;
    assBtn.setBounds(buttonRow.removeFromLeft(btnWidth));
    buttonRow.removeFromLeft(gap);
    bootyBtn.setBounds(buttonRow.removeFromLeft(btnWidth));
    buttonRow.removeFromLeft(gap);
    crackBtn.setBounds(buttonRow);

    area.removeFromTop(24);

    // Sliders
    auto sliderArea = area.reduced(10, 0);
    auto placeRow = [&](juce::Label& lbl, juce::Slider& sld) {
        auto row = sliderArea.removeFromTop(52);
        lbl.setBounds(row.removeFromTop(20));
        sld.setBounds(row.removeFromTop(28));
    };

    placeRow(driveLabel, driveSlider);
    placeRow(sagLabel, sagSlider);
    placeRow(tightLabel, tightSlider);
    placeRow(outputLabel, outputSlider);
}
