/*
  ==============================================================================

    RotaryKnob.h
    Created: 26 Aug 2024 8:53:15pm
    Author:  Jose Gonzalez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RotaryKnob  : public juce::Component
{
public:
    RotaryKnob(const juce::String &text,
               juce::AudioProcessorValueTreeState &apvts,
               const juce::ParameterID &parameterID);
    
    ~RotaryKnob() override;
    
    juce::Slider slider;
    juce::Label label;
    //Los attachments deben ir debajo de los components
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;

    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
