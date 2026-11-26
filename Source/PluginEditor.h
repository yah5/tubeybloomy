#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class TubeyBloomyLookAndFeel : public juce::LookAndFeel_V4 {
public:
    TubeyBloomyLookAndFeel() {
        setColour(juce::Slider::thumbColourId, juce::Colour(0xFFFF7A1A));
        setColour(juce::Slider::trackColourId, juce::Colour(0xFFFF7A1A));
        setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF2A2A2E));
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle style, juce::Slider& slider) override {
        juce::ignoreUnused(minSliderPos, maxSliderPos, style, slider);

        const float trackHeight = 6.0f;
        const float centerY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
        const float trackY = centerY - trackHeight * 0.5f;

        g.setColour(juce::Colour(0xFF2A2A2F));
        g.fillRoundedRectangle(static_cast<float>(x), trackY, static_cast<float>(width), trackHeight, 3.0f);

        const float filledWidth = juce::jlimit(0.0f, static_cast<float>(width), sliderPos - static_cast<float>(x));
        g.setColour(juce::Colour(0xFFFF7A1A));
        g.fillRoundedRectangle(static_cast<float>(x), trackY, filledWidth, trackHeight, 3.0f);

        const float thumbRadius = 8.5f;
        g.setColour(juce::Colour(0x44FF7A1A));
        g.fillEllipse(sliderPos - thumbRadius - 3.0f, centerY - thumbRadius - 3.0f,
                      (thumbRadius + 3.0f) * 2.0f, (thumbRadius + 3.0f) * 2.0f);

        g.setColour(juce::Colour(0xFFFF8C24));
        g.fillEllipse(sliderPos - thumbRadius, centerY - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f);

        g.setColour(juce::Colour(0xFFFFD1A6));
        g.fillEllipse(sliderPos - 3.0f, centerY - 3.0f, 6.0f, 6.0f);
    }
};

class CuteTubeMascot : public juce::Component {
public:
    void setActivity(float glowLevel, float dancePhase) {
        glow = glowLevel;
        phase = dancePhase;
        repaint();
    }

    void paint(juce::Graphics& g) override;

private:
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
    void applyStrengthPreset(int mode);
    void updateButtonStyles();
    void handleUserSliderChange();

    TubeyBloomyAudioProcessor& audioProcessor;
    TubeyBloomyLookAndFeel orangeLookAndFeel;

    juce::Label titleLabel;
    CuteTubeMascot tubeMascot;

    juce::TextButton lightBtn  { "Light" };
    juce::TextButton mediumBtn { "Medium" };
    juce::TextButton heavyBtn  { "Heavy" };
    juce::TextButton customBtn { "Custom" };

    juce::Slider driveSlider, sagSlider, tightSlider, outputSlider;
    juce::Label driveLabel, sagLabel, tightLabel, outputLabel;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> driveAttach, sagAttach, tightAttach, outputAttach;

    bool isApplyingPreset = false;
    float smoothedGlow = 0.0f;
    float dancePhase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TubeyBloomyAudioProcessorEditor)
};
