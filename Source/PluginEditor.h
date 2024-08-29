/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"

//==============================================================================
/**
*/
class DelayRound2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DelayRound2AudioProcessorEditor (DelayRound2AudioProcessor&);
    ~DelayRound2AudioProcessorEditor() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    
    DelayRound2AudioProcessor& audioProcessor; //Debe ir de primero
    
    
    //Objetos/variables
    RotaryKnob outGainKnob{ "Out Gain", audioProcessor.apvts, outGainParamID };
    RotaryKnob delayTimeKnob{ "Delay Time", audioProcessor.apvts, delayTimeParamID };
    RotaryKnob drySignalKnob{ "Dry", audioProcessor.apvts, drySignalParamID };
    RotaryKnob wetSignalKnob{ "Wet", audioProcessor.apvts, wetSignalParamID };
    RotaryKnob feedbackKnob{ "Feedback", audioProcessor.apvts, feedbackParamID}; //That true allows -values
    RotaryKnob stereoKnob{ "Stereo" , audioProcessor.apvts, stereoParamID};
    
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;



    
    /*
    juce::AudioProcessorValueTreeState::SliderAttachment attachment{ //Objeto attachment para enlazar outGain y componente
        audioProcessor.apvts, outGainParamID.getParamID(), outGainKnob.slider}; //Para hacer sin implementarlo en el const de la custom class
    */
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayRound2AudioProcessorEditor) //Maneja automaticamente memory leaks

};
