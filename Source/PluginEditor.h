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
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class DelayRound2AudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         private juce::AudioProcessorParameter::Listener
{
public:
    DelayRound2AudioProcessorEditor (DelayRound2AudioProcessor&);
    ~DelayRound2AudioProcessorEditor() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    //Funciones
    void parameterValueChanged(int, float) override;
    
    void parameterGestureChanged(int, bool) override{} //Asi se escribe una función sin definición ({} y sin ;)
    
    void updateDelayKnobs(bool tempoSyncActive);
    
    //AudioProcessor
    DelayRound2AudioProcessor& audioProcessor; //Debe ir de primero
    
    
    //Objetos/variables
    RotaryKnob outGainKnob{ "Out Gain", audioProcessor.apvts, outGainParamID };
    RotaryKnob delayTimeKnob{ "Delay Time", audioProcessor.apvts, delayTimeParamID };
    RotaryKnob drySignalKnob{ "Dry", audioProcessor.apvts, drySignalParamID };
    RotaryKnob wetSignalKnob{ "Wet", audioProcessor.apvts, wetSignalParamID };
    RotaryKnob feedbackKnob{ "Feedback", audioProcessor.apvts, feedbackParamID}; 
    RotaryKnob stereoKnob{ "Stereo" , audioProcessor.apvts, stereoParamID};
    RotaryKnob lowCutKnob{ "Low Cut", audioProcessor.apvts, lowCutParamID};
    RotaryKnob highCutKnob{ "High Cut", audioProcessor.apvts, highCutParamID};
    
    RotaryKnob delayNoteKnob{"Note", audioProcessor.apvts, delayNoteParamID};
    
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;
    
    juce::TextButton tempoSyncButton;
    
    juce::AudioProcessorValueTreeState::ButtonAttachment tempoSyncAttachment{
        audioProcessor.apvts, tempoSyncParamID.getParamID(), tempoSyncButton
    };

    /*
    juce::AudioProcessorValueTreeState::SliderAttachment attachment{ //Objeto attachment para enlazar parametro y componente
        audioProcessor.apvts, outGainParamID.getParamID(), outGainKnob.slider}; //Para hacer sin implementarlo en el const de la custom class
    */
    
    MainLookAndFeel mainLF;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayRound2AudioProcessorEditor) //Maneja automaticamente memory leaks

};
