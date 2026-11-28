#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class AmpRotaryLookAndFeel : public juce::LookAndFeel_V4 {
public:
    AmpRotaryLookAndFeel() {
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFFFF8C1A));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF2E2622));
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider&) override {
        auto bounds = juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y),
                                             static_cast<float>(width), static_cast<float>(height)).reduced(2.0f);
        const float cx = bounds.getCentreX();
        const float cy = bounds.getCentreY();
        const float maxR = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;

        // 1. Exactly 10 evenly spaced, uniform-thickness thin cream tick marks (#D8D3C5, ~1.5px wide)
        const float tickInnerR = maxR - 7.5f;
        const float tickOuterR = maxR - 1.5f;
        g.setColour(juce::Colour(0xFFD8D3C5));
        for (int i = 0; i < 10; ++i) {
            const float norm = static_cast<float>(i) / 9.0f;
            const float angle = rotaryStartAngle + norm * (rotaryEndAngle - rotaryStartAngle);
            const float sinA = std::sin(angle);
            const float cosA = -std::cos(angle);
            g.drawLine(cx + sinA * tickInnerR, cy + cosA * tickInnerR,
                       cx + sinA * tickOuterR, cy + cosA * tickOuterR, 1.5f);
        }

        // 2. Knob body geometry
        const float skirtR = maxR - 11.5f;
        const float capR   = skirtR - 4.5f;

        // Drop shadow
        g.setColour(juce::Colour(0x99000000));
        g.fillEllipse(cx - skirtR + 1.5f, cy - skirtR + 3.0f, skirtR * 2.0f, skirtR * 2.0f);

        // Dark metallic bronze-tinted outer skirt (#2E2622)
        juce::ColourGradient skirtGrad(
            juce::Colour(0xFF423630), cx - skirtR * 0.5f, cy - skirtR * 0.7f,
            juce::Colour(0xFF1E1815), cx + skirtR * 0.6f, cy + skirtR * 0.8f, false);
        g.setGradientFill(skirtGrad);
        g.fillEllipse(cx - skirtR, cy - skirtR, skirtR * 2.0f, skirtR * 2.0f);

        // Knurled grip ridges around skirt perimeter
        const int numKnurls = 28;
        g.setColour(juce::Colour(0x55100D0B));
        for (int k = 0; k < numKnurls; ++k) {
            const float ka = (static_cast<float>(k) / static_cast<float>(numKnurls)) * juce::MathConstants<float>::twoPi;
            const float x1 = cx + std::cos(ka) * (capR + 0.5f);
            const float y1 = cy + std::sin(ka) * (capR + 0.5f);
            const float x2 = cx + std::cos(ka) * (skirtR - 0.5f);
            const float y2 = cy + std::sin(ka) * (skirtR - 0.5f);
            g.drawLine(x1, y1, x2, y2, 1.1f);
        }

        g.setColour(juce::Colour(0xFF14110F));
        g.drawEllipse(cx - skirtR, cy - skirtR, skirtR * 2.0f, skirtR * 2.0f, 1.1f);

        // Matte charcoal/off-black top cap (#1A1A1D)
        juce::ColourGradient capGrad(
            juce::Colour(0xFF242429), cx - capR * 0.4f, cy - capR * 0.6f,
            juce::Colour(0xFF131316), cx + capR * 0.5f, cy + capR * 0.7f, false);
        g.setGradientFill(capGrad);
        g.fillEllipse(cx - capR, cy - capR, capR * 2.0f, capR * 2.0f);

        g.setColour(juce::Colour(0xFF333339));
        g.drawEllipse(cx - capR, cy - capR, capR * 2.0f, capR * 2.0f, 1.0f);

        // 3. Crisp cream-white vertical pill notch (#F2EFE9) indicating position
        const float pointerAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        juce::Graphics::ScopedSaveState state(g);
        g.addTransform(juce::AffineTransform::rotation(pointerAngle, cx, cy));

        const float pillW = 3.0f;
        const float pillH = capR * 0.62f;
        g.setColour(juce::Colour(0xFFF2EFE9));
        g.fillRoundedRectangle(cx - pillW * 0.5f, cy - capR + 2.5f, pillW, pillH, 1.5f);
    }
};

class DigitalReadoutScreen : public juce::Component {
public:
    void setText(const juce::String& newText) {
        if (displayText != newText) {
            displayText = newText;
            repaint();
        }
    }

    void paint(juce::Graphics& g) override {
        auto b = getLocalBounds().toFloat();

        // Outer metallic bezel
        g.setColour(juce::Colour(0xFF28282E));
        g.fillRoundedRectangle(b, 4.0f);

        // Dark recessed screen (#08080A)
        auto inner = b.reduced(1.5f);
        g.setColour(juce::Colour(0xFF08080A));
        g.fillRoundedRectangle(inner, 3.0f);

        g.setColour(juce::Colour(0x66000000));
        g.drawRoundedRectangle(inner, 3.0f, 1.0f);

        // Glowing amber-orange digital readout (#FF8C1A)
        juce::Font f(juce::FontOptions(14.5f));
        f.setBold(true);
        g.setFont(f);

        g.setColour(juce::Colour(0x44FF5E00));
        g.drawText(displayText, inner.translated(0.0f, 0.5f), juce::Justification::centred, false);

        g.setColour(juce::Colour(0xFFFF8C1A));
        g.drawText(displayText, inner, juce::Justification::centred, false);
    }

private:
    juce::String displayText { "0" };
};

class MetallicBypassSwitch : public juce::Component {
public:
    std::function<void()> onToggle;

    void setActive(bool active) {
        isActive = active;
        repaint();
    }

    void mouseUp(const juce::MouseEvent& e) override {
        if (getLocalBounds().contains(e.getPosition()) && onToggle)
            onToggle();
    }

    void paint(juce::Graphics& g) override {
        auto b = getLocalBounds().toFloat();
        const float cx = b.getCentreX();
        const float cy = b.getCentreY();

        g.setColour(juce::Colour(0xFF5A5854));
        g.fillEllipse(cx - 8.5f, cy - 8.5f, 17.0f, 17.0f);
        g.setColour(juce::Colour(0xFF9C9890));
        g.drawEllipse(cx - 8.5f, cy - 8.5f, 17.0f, 17.0f, 1.2f);

        g.setColour(juce::Colour(0xFF1A1A1D));
        g.fillEllipse(cx - 4.5f, cy - 4.5f, 9.0f, 9.0f);

        const float batTipY = isActive ? (cy - 12.0f) : (cy + 12.0f);
        juce::ColourGradient batGrad(
            juce::Colour(0xFFE8E4DC), cx - 2.5f, cy,
            juce::Colour(0xFF7A7670), cx + 2.5f, batTipY, false);
        g.setGradientFill(batGrad);

        juce::Path bat;
        bat.startNewSubPath(cx - 2.0f, cy);
        bat.lineTo(cx - 3.2f, batTipY);
        bat.lineTo(cx + 3.2f, batTipY);
        bat.lineTo(cx + 2.0f, cy);
        bat.closeSubPath();
        g.fillPath(bat);

        g.setColour(juce::Colour(0xFFF5F2EB));
        g.fillEllipse(cx - 3.2f, batTipY - 2.5f, 6.4f, 5.0f);
    }

private:
    bool isActive = true;
};

class ScalePillButton : public juce::TextButton {
public:
    explicit ScalePillButton(const juce::String& name) : juce::TextButton(name) {}

    void setSelected(bool sel) {
        selected = sel;
        repaint();
    }

    void paintButton(juce::Graphics& g, bool highlighted, bool down) override {
        auto b = getLocalBounds().toFloat().reduced(0.5f);
        g.setColour(selected ? juce::Colour(0xFF241A12) : (highlighted ? juce::Colour(0xFF1C1C20) : juce::Colour(0xFF111114)));
        if (down) g.setColour(juce::Colour(0xFF2E2116));
        g.fillRoundedRectangle(b, 4.0f);

        g.setColour(selected ? juce::Colour(0xFFFF8C1A) : juce::Colour(0xFF4A4843));
        g.drawRoundedRectangle(b, 4.0f, selected ? 1.4f : 1.0f);

        juce::Font f(juce::FontOptions(11.5f));
        f.setBold(selected);
        g.setFont(f);
        g.setColour(selected ? juce::Colour(0xFFE6E2D6) : juce::Colour(0xFF8C8982));
        g.drawText(getButtonText(), b, juce::Justification::centred, false);
    }

private:
    bool selected = false;
};

class BacklitPresetButton : public juce::TextButton {
public:
    explicit BacklitPresetButton(const juce::String& name) : juce::TextButton(name) {}

    void setSelected(bool sel) {
        selected = sel;
        repaint();
    }

    void paintButton(juce::Graphics& g, bool highlighted, bool down) override {
        auto b = getLocalBounds().toFloat();

        if (selected) {
            g.setColour(juce::Colour(0x33FF8C1A));
            g.fillRoundedRectangle(b.expanded(2.0f), 7.0f);

            juce::ColourGradient fillGrad(
                juce::Colour(0xFFFF9E2C), b.getCentreX(), b.getY(),
                juce::Colour(0xFFE86A00), b.getCentreX(), b.getBottom(), false);
            g.setGradientFill(fillGrad);
            g.fillRoundedRectangle(b.reduced(0.5f), 5.0f);

            g.setColour(juce::Colour(0xFFFFCFA0));
            g.drawRoundedRectangle(b.reduced(0.5f), 5.0f, 1.3f);
        } else {
            juce::Colour bg = highlighted ? juce::Colour(0xFF1E1E22) : juce::Colour(0xFF151518);
            if (down) bg = juce::Colour(0xFF242429);
            g.setColour(bg);
            g.fillRoundedRectangle(b.reduced(0.5f), 5.0f);

            g.setColour(juce::Colour(0xFF4A4842));
            g.drawRoundedRectangle(b.reduced(0.5f), 5.0f, 1.1f);
        }

        const juce::String text = getButtonText();
        if (text.isEmpty())
            return;

        const juce::Colour textCol = selected ? juce::Colour(0xFF0E0E10) : juce::Colour(0xFFD0CCC0);
        g.setColour(textCol);

        // Bold ONLY the first letter (A, B, C), regular weight for the remaining Title Case letters
        const juce::String firstChar = text.substring(0, 1);
        const juce::String restChars = text.substring(1);

        juce::Font boldFont(juce::FontOptions(14.5f));
        boldFont.setBold(true);
        juce::Font regularFont(juce::FontOptions(13.5f));

        juce::GlyphArrangement gaFirst;
        gaFirst.addLineOfText(boldFont, firstChar, 0.0f, 0.0f);
        const float firstW = gaFirst.getBoundingBox(0, -1, true).getWidth();

        juce::GlyphArrangement gaRest;
        gaRest.addLineOfText(regularFont, restChars, 0.0f, 0.0f);
        const float restW = gaRest.getBoundingBox(0, -1, true).getWidth();

        const float totalW = firstW + restW;
        const float startX = b.getCentreX() - totalW * 0.5f;

        g.setFont(boldFont);
        g.drawText(firstChar,
                   juce::Rectangle<float>(startX, b.getY(), firstW + 2.0f, b.getHeight()),
                   juce::Justification::centredLeft, false);

        g.setFont(regularFont);
        g.drawText(restChars,
                   juce::Rectangle<float>(startX + firstW, b.getY(), restW + 4.0f, b.getHeight()),
                   juce::Justification::centredLeft, false);
    }

private:
    bool selected = false;
};

class TubeCageComponent : public juce::Component {
public:
    void setActivity(float glowLevel, float sniffPhase, float driveNorm) {
        glow = glowLevel;
        phase = sniffPhase;
        drive = driveNorm;
        repaint();
    }

    void paint(juce::Graphics& g) override;

private:
    void drawUprightNeonNose(juce::Graphics& g, float cx, float cy, bool facingRight);
    void drawVacuumTube(juce::Graphics& g, float cx, float cy, float tubeGlow);

    float glow = 0.0f;
    float phase = 0.0f;
    float drive = 0.2f;
};

class KnobControlUnit {
public:
    juce::Slider slider;
    DigitalReadoutScreen readout;
    juce::Label line1Label;
    juce::Label line2Label;
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
    void updateUiState();
    void handleUserSliderChange();

    float getScaleFactor() const {
        if (audioProcessor.uiScaleIndex == 1) return 1.5f;
        if (audioProcessor.uiScaleIndex == 2) return 2.0f;
        return 1.0f;
    }

    TubeyBloomyAudioProcessor& audioProcessor;
    AmpRotaryLookAndFeel ampLookAndFeel;

    juce::Component contentContainer;

    // Top-Right Window Size Bar
    juce::Label windowSizeLabel;
    ScalePillButton scale1xBtn   { "1x" };
    ScalePillButton scale1_5xBtn { "1.5x" };
    ScalePillButton scale2xBtn   { "2x" };

    // Top Vacuum Tube Cage & Dual Upright Neon Noses
    TubeCageComponent tubeCage;

    // Faceplate Header Bar
    MetallicBypassSwitch bypassSwitch;
    juce::Label titleLabel;

    // 5 Knob Control Units (4 in left box, 1 isolated in right box)
    KnobControlUnit sagUnit;
    KnobControlUnit harmonicsUnit;
    KnobControlUnit driveUnit;
    KnobControlUnit tightUnit;
    KnobControlUnit outputUnit;

    // Bottom 3 Preset Strip
    BacklitPresetButton assBtn   { "Ass Sniff" };
    BacklitPresetButton bootyBtn { "Booty Sniff" };
    BacklitPresetButton crackBtn { "Crack Sniff" };

    // Bottom-Right Version / Credit Label
    juce::Label versionLabel;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> driveAttach, sagAttach, harmonicsAttach, tightAttach, outputAttach;

    bool isLoadingSlot = false;
    float smoothedGlow = 0.0f;
    float sniffPhase = 0.0f;

    static constexpr int baseWidth  = 660;
    static constexpr int baseHeight = 442;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TubeyBloomyAudioProcessorEditor)
};