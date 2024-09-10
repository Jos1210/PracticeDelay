#pragma once

#include <JuceHeader.h>

namespace Colors
{
    const juce::Colour background { 74, 150, 113 };
    const juce::Colour header { 0, 0, 0 };
    namespace Knob
    {
        const juce::Colour trackBackground { 0, 50, 0 }; const juce::Colour trackActive { 124, 200, 163 }; const juce::Colour outline { 255, 250, 245 };
        const juce::Colour gradientTop { 100, 130, 100 }; const juce::Colour gradientBottom { 100, 130, 100 };
        const juce::Colour dial { 255, 255, 255 };
        const juce::Colour dropShadow { 40, 40, 40 };
        const juce::Colour label { 255, 255, 255 };
        const juce::Colour textBoxBackground { 255, 255, 255 }; const juce::Colour value { 255, 255, 255 };
        const juce::Colour caret { 255, 255, 255 };
    }
    namespace Group
    {
    const juce::Colour label { 255, 255, 255 };
    const juce::Colour outline { 255, 255, 255 };
    }

    namespace LevelMeter{
        const juce::Colour background { 74, 150, 113 };
        const juce::Colour tickLine { 0, 0, 0 };
        const juce::Colour tickLabel { 255, 255, 255 };
        const juce::Colour tooLoud { 226, 74, 81 };
        const juce::Colour levelOK { 65, 206, 88 };
    }
}

class RotaryKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
        
    //Functions
    RotaryKnobLookAndFeel();
    
    static RotaryKnobLookAndFeel* get() {
        static RotaryKnobLookAndFeel instance;
        return &instance;
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override;

private:
    juce::DropShadow dropShadow { Colors::Knob::dropShadow, 6, { 0, 3 }};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnobLookAndFeel)
};

class MainLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MainLookAndFeel();
    private: JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLookAndFeel)
};
