#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

inline juce::Font makeDmSansFont(float size, bool bold = false) {
    auto opts = juce::FontOptions(size);
    opts = opts.withName("DM Sans");
    juce::Font f(opts);
    f.setBold(bold);
    return f;
}

class OutputLevelSlider : public juce::Slider {
public:
    juce::String getTextFromValue(double value) override {
        if (value < 0.5)
            return "OFF";
        return juce::String(juce::roundToInt(value)) + " dB";
    }
};

class TubeSnifferLookAndFeel : public juce::LookAndFeel_V4 {
public:
    TubeSnifferLookAndFeel() {
        setDefaultSansSerifTypefaceName("DM Sans");
        setColour(juce::Slider::thumbColourId, juce::Colours::white);
        setColour(juce::Slider::trackColourId, juce::Colour(0xFFFF7A1A));
        setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF2A2A2E));
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    }

    juce::Font getLabelFont(juce::Label& label) override {
        return makeDmSansFont(label.getFont().getHeight(), label.getFont().isBold());
    }

    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override {
        return makeDmSansFont(juce::jmin(14.0f, static_cast<float>(buttonHeight) * 0.52f), true);
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle style, juce::Slider& slider) override {
        juce::ignoreUnused(minSliderPos, maxSliderPos, style, slider);

        const float trackHeight = 6.0f;
        const float centerY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
        const float trackY = centerY - trackHeight * 0.5f;

        g.setColour(juce::Colour(0xFF2B2B31));
        g.fillRoundedRectangle(static_cast<float>(x), trackY, static_cast<float>(width), trackHeight, 3.0f);

        const float filledWidth = juce::jlimit(0.0f, static_cast<float>(width), sliderPos - static_cast<float>(x));
        g.setColour(juce::Colour(0xFFFF7A1A));
        g.fillRoundedRectangle(static_cast<float>(x), trackY, filledWidth, trackHeight, 3.0f);

        // White tube-shaped slider thumb (5% larger)
        const float thumbW = 13.0f;
        const float thumbH = 22.0f;
        const float tx = sliderPos - thumbW * 0.5f;
        const float ty = centerY - thumbH * 0.5f;

        g.setColour(juce::Colour(0x55000000));
        g.fillRoundedRectangle(tx + 1.0f, ty + 1.5f, thumbW, thumbH, 5.5f);

        g.setColour(juce::Colour(0xFFF6F6F8));
        g.fillRoundedRectangle(tx, ty, thumbW, thumbH, 5.5f);

        g.setColour(juce::Colour(0xFFD0D0D8));
        g.fillRoundedRectangle(tx + 1.5f, ty + thumbH - 4.5f, thumbW - 3.0f, 3.0f, 1.5f);

        g.setColour(juce::Colour(0xFFFF8C24));
        g.fillRoundedRectangle(tx + thumbW * 0.5f - 1.2f, ty + 4.5f, 2.4f, 10.0f, 1.2f);
    }
};

class BoldFirstLetterButton : public juce::TextButton {
public:
    explicit BoldFirstLetterButton(const juce::String& name) : juce::TextButton(name) {}

    void setIsSelected(bool selected) {
        isSelected = selected;
        repaint();
    }

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        auto bounds = getLocalBounds().toFloat();

        juce::Colour bg = isSelected ? juce::Colour(0xFFFF7A1A) : juce::Colour(0xFF24242A);
        if (shouldDrawButtonAsDown)
            bg = bg.darker(0.15f);
        else if (shouldDrawButtonAsHighlighted)
            bg = bg.brighter(0.1f);

        g.setColour(bg);
        g.fillRoundedRectangle(bounds, 7.0f);

        g.setColour(isSelected ? juce::Colour(0xFFFFB273) : juce::Colour(0xFF383842));
        g.drawRoundedRectangle(bounds.reduced(0.5f), 7.0f, 1.0f);

        const juce::String text = getButtonText();
        if (text.isEmpty())
            return;

        const juce::Colour textCol = isSelected ? juce::Colour(0xFF121214) : juce::Colour(0xFFE4E4EC);
        g.setColour(textCol);

        const juce::String firstChar = text.substring(0, 1);
        const juce::String restChars = text.substring(1);

        juce::Font boldFont    = makeDmSansFont(15.5f, true);
        juce::Font regularFont = makeDmSansFont(14.5f, false);

        juce::GlyphArrangement ga;
        ga.addLineOfText(boldFont, firstChar, 0.0f, 0.0f);
        const float firstW = ga.getBoundingBox(0, -1, true).getWidth();

        juce::GlyphArrangement gaRest;
        gaRest.addLineOfText(regularFont, restChars, 0.0f, 0.0f);
        const float restW = gaRest.getBoundingBox(0, -1, true).getWidth();

        const float totalW = firstW + restW;
        const float startX = bounds.getCentreX() - totalW * 0.5f;

        g.setFont(boldFont);
        g.drawText(firstChar,
                   juce::Rectangle<float>(startX, bounds.getY(), firstW + 2.0f, bounds.getHeight()),
                   juce::Justification::centredLeft, false);

        g.setFont(regularFont);
        g.drawText(restChars,
                   juce::Rectangle<float>(startX + firstW, bounds.getY(), restW + 4.0f, bounds.getHeight()),
                   juce::Justification::centredLeft, false);
    }

private:
    bool isSelected = false;
};

class TubeSnifferCenterpiece : public juce::Component {
public:
    void setActivity(float glowLevel, float sniffPhase) {
        glow = glowLevel;
        phase = sniffPhase;
        repaint();
    }

    void paint(juce::Graphics& g) override;

private:
    void drawRealistic12AX7(juce::Graphics& g, float cx, float cy, float scale);
    void drawClipArtNose(juce::Graphics& g, float cx, float cy, bool facingRight, float swayX, float swayAngle);

    float glow = 0.0f;
    float phase = 0.0f;
};

class TubeyBloomyAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        private juce::Timer {
public:
    explicit TubeyBloomyAudioProcessorEditor(TubeyBloomyAudioProcessor&);
    ~TubeyBloomyAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void selectSlot(int slotIndex);
    void setUiScaleIndex(int scaleIndex);
    void toggleMasterPower();
    void toggleOutputPower();
    void updateButtonStates();
    void handleUserSliderChange();

    float getScaleFactor() const {
        if (audioProcessor.uiScaleIndex == 1) return 1.5f;
        if (audioProcessor.uiScaleIndex == 2) return 2.0f;
        return 1.0f;
    }

    TubeyBloomyAudioProcessor& audioProcessor;
    TubeSnifferLookAndFeel customLookAndFeel;

    juce::Component contentContainer;

    juce::Label titleLabel;
    juce::Label sizeLabel;
    juce::TextButton scale1xBtn   { "1x" };
    juce::TextButton scale1_5xBtn { "1.5x" };
    juce::TextButton scale2xBtn   { "2x" };

    TubeSnifferCenterpiece centerpiece;

    // Centered Master ON/OFF toggle button below tube
    juce::TextButton masterPowerBtn { "ON" };

    // 5 Sliders
    juce::Slider driveSlider, sagSlider, harmonicsSlider, tightSlider;
    OutputLevelSlider outputSlider;
    juce::Label driveLabel, sagLabel, harmonicsLabel, tightLabel, outputLabel;

    // Toggle button next to Output Level label
    juce::TextButton outputPowerBtn { "OFF" };

    // 3 A/B/C Custom Memory Buttons below sliders
    BoldFirstLetterButton assBtn   { "Ass Sniff" };
    BoldFirstLetterButton bootyBtn { "Booty Sniff" };
    BoldFirstLetterButton crackBtn { "Crack Sniff" };

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> driveAttach, sagAttach, harmonicsAttach, tightAttach, outputAttach;

    bool isLoadingSlot = false;
    float smoothedGlow = 0.0f;
    float sniffPhase = 0.0f;

    static constexpr int baseWidth  = 470;
    static constexpr int baseHeight = 588;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TubeyBloomyAudioProcessorEditor)
};
