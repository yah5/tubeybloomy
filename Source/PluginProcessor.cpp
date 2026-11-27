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

    g.setColour(juce::Colour(0x28DCE8F5));
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

        const juce::Colour idleGlow(0xFF6B280E);
        const juce::Colour activeGlow(0xFFFFC44D);
        const juce::Colour heaterCol = idleGlow.interpolatedWith(activeGlow, juce::jlimit(0.0f, 1.0f, 0.25f + glow * 0.95f));

        g.setColour(heaterCol);
        g.fillRoundedRectangle(plateX + 3.8f, cy - 25.0f, 3.8f, 5.0f, 1.5f);
        g.fillRoundedRectangle(plateX + 3.8f, cy + 20.0f, 3.8f, 5.0f, 1.5f);

        g.setColour(heaterCol.withAlpha(0.35f + glow * 0.65f));
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

void TubeSnifferCenterpiece::drawRetroCartoonFace(juce::Graphics& g, float cx, float cy, bool lookRight, float swayX, float swayAngle) {
    juce::Graphics::ScopedSaveState state(g);

    const float bounceY = -std::abs(std::sin(phase)) * (glow * 4.5f);
    g.addTransform(juce::AffineTransform::translation(-cx, -cy)
                       .rotated(swayAngle)
                       .translated(cx + swayX, cy + bounceY));

    const float r = 24.0f;

    // Vintage 1930s/50s rubber-hose style round head with chubby cheeks
    juce::Path head;
    head.addEllipse(cx - r, cy - r * 0.92f, r * 2.0f, r * 1.84f);

    // Warm cream/vintage paper fill
    g.setColour(juce::Colour(0xFFF6EFE2));
    g.fillPath(head);

    // Bold black ink outline
    g.setColour(juce::Colour(0xFF141416));
    g.strokePath(head, juce::PathStrokeType(2.2f));

    const float eyeLookOffset = lookRight ? 1.5f : -1.5f;

    // Sleepy half-lidded retro eyes (left and right)
    auto drawSleepyEye = [&](float ex, float ey) {
        const float ew = 9.5f;
        const float eh = 11.0f;

        // Eye white
        g.setColour(juce::Colours::white);
        g.fillEllipse(ex - ew * 0.5f, ey - eh * 0.5f, ew, eh);

        // Relaxed retro pupil looking toward the glowing tube
        g.setColour(juce::Colour(0xFF141416));
        g.fillEllipse(ex - 2.0f + eyeLookOffset, ey - 0.5f, 4.2f, 5.0f);

        // Heavy sleepy upper eyelid covering top half of eye
        g.setColour(juce::Colour(0xFFE6DCCB));
        g.fillRect(ex - ew * 0.5f, ey - eh * 0.5f, ew, eh * 0.48f);

        // Eye outline & horizontal sleepy lid line
        g.setColour(juce::Colour(0xFF141416));
        g.drawEllipse(ex - ew * 0.5f, ey - eh * 0.5f, ew, eh, 1.6f);
        g.drawLine(ex - ew * 0.5f - 0.5f, ey - 0.3f, ex + ew * 0.5f + 0.5f, ey - 0.3f, 1.8f);
    };

    drawSleepyEye(cx - 7.5f, cy - 5.5f);
    drawSleepyEye(cx + 7.5f, cy - 5.5f);

    // Classic vintage cartoon button nose
    g.setColour(juce::Colour(0xFF141416));
    g.fillEllipse(cx - 3.2f + eyeLookOffset * 0.6f, cy + 0.5f, 6.4f, 4.6f);

    // Wide relaxed cartoon smile with cheek dimple creases
    juce::Path smile;
    smile.startNewSubPath(cx - 13.5f, cy + 6.5f);
    smile.quadraticTo(cx, cy + 16.5f, cx + 13.5f, cy + 6.5f);
    g.strokePath(smile, juce::PathStrokeType(2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Cheek dimple arcs at corners of wide smile
    juce::Path leftDimple;
    leftDimple.addArc(cx - 16.5f, cy + 3.5f, 5.0f, 6.0f,
                      juce::MathConstants<float>::pi * 0.7f,
                      juce::MathConstants<float>::pi * 1.4f, true);
    g.strokePath(leftDimple, juce::PathStrokeType(1.6f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path rightDimple;
    rightDimple.addArc(cx + 11.5f, cy + 3.5f, 5.0f, 6.0f,
                       juce::MathConstants<float>::pi * 0.6f,
                       juce::MathConstants<float>::pi * 1.3f, true);
    g.strokePath(rightDimple, juce::PathStrokeType(1.6f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}

void TubeSnifferCenterpiece::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().toFloat();
    const float cx = bounds.getCentreX();
    const float cy = bounds.getCentreY() + 2.0f;

    // Realistic 12AX7 tube (+20% larger)
    drawRealistic12AX7(g, cx, cy, 1.20f);

    // Two vintage retro cartoon faces on either side swaying left & right to the input signal
    const float swayX     = std::sin(phase) * (glow * 11.0f);
    const float swayAngle = std::sin(phase) * (glow * 0.18f);

    drawRetroCartoonFace(g, cx - 88.0f, cy + 2.0f, true,  swayX, swayAngle);
    drawRetroCartoonFace(g, cx + 88.0f, cy + 2.0f, false, swayX, swayAngle);
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

    // Top-right Size controls: Size [1x] [1.5x] [2x]
    sizeLabel.setText("Size", juce::dontSendNotification);
    sizeLabel.setFont(juce::Font(juce::FontOptions(13.0f)));
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

    // Centerpiece: 12AX7 tube + 2 swaying retro cartoon faces
    contentContainer.addAndMakeVisible(centerpiece);

    // 3 A/B/C Custom Memory Buttons: Ass Sniff - Booty Sniff - Crack Sniff
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

void TubeyBloomyAudioProcessorEditor::selectSlot(int slotIndex) {
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
    const float rawLevel = audioProcessor.currentInputLevel.load(std::memory_order_relaxed);
    const float targetGlow = juce::jlimit(0.0f, 1.0f, std::pow(rawLevel * 3.0f, 0.65f));

    if (targetGlow > smoothedGlow)
        smoothedGlow += (targetGlow - smoothedGlow) * 0.55f;
    else
        smoothedGlow += (targetGlow - smoothedGlow) * 0.055f;

    if (smoothedGlow > 0.01f) {
        dancePhase += 0.28f + (smoothedGlow * 0.24f);
        if (dancePhase > juce::MathConstants<float>::twoPi)
            dancePhase -= juce::MathConstants<float>::twoPi;
    } else {
        dancePhase = 0.0f;
    }

    centerpiece.setActivity(smoothedGlow, dancePhase);
}

void TubeyBloomyAudioProcessorEditor::paint(juce::Graphics& g) {
    const float scale = getScaleFactor();
    g.addTransform(juce::AffineTransform::scale(scale));

    g.fillAll(juce::Colour(0xFF161619));

    g.setColour(juce::Colour(0xFF1E1E24));
    g.fillRoundedRectangle(22.0f, 266.0f, static_cast<float>(baseWidth - 44), 232.0f, 12.0f);
    g.setColour(juce::Colour(0xFF2E2E36));
    g.drawRoundedRectangle(22.0f, 266.0f, static_cast<float>(baseWidth - 44), 232.0f, 12.0f, 1.0f);
}

void TubeyBloomyAudioProcessorEditor::resized() {
    contentContainer.setBounds(0, 0, baseWidth, baseHeight);
    auto area = juce::Rectangle<int>(0, 0, baseWidth, baseHeight).reduced(24, 16);

    // Top bar: Title on left, Size [1x] [1.5x] [2x] on top right
    auto topBar = area.removeFromTop(34);
    auto sizeArea = topBar.removeFromRight(150);
    sizeLabel.setBounds(sizeArea.removeFromLeft(34));
    sizeArea.removeFromLeft(4);
    scale1xBtn.setBounds(sizeArea.removeFromLeft(32).reduced(1, 4));
    scale1_5xBtn.setBounds(sizeArea.removeFromLeft(42).reduced(1, 4));
    scale2xBtn.setBounds(sizeArea.removeFromLeft(32).reduced(1, 4));

    titleLabel.setBounds(topBar);

    // 12AX7 tube + 2 swaying retro cartoon faces
    centerpiece.setBounds(area.removeFromTop(152));

    area.removeFromTop(6);

    // 3 A/B/C custom buttons: Ass Sniff - Booty Sniff - Crack Sniff
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
