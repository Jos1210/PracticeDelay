#pragma once

//Includes
#include <JuceHeader.h>

//Consts
const juce::ParameterID outGainParamID { "outGain", 1 };
const juce::ParameterID delayTimeParamID { "delayTime", 1 };
const juce::ParameterID drySignalParamID {"Dry", 1 };
const juce::ParameterID wetSignalParamID {"Wet", 1 };
const juce::ParameterID feedbackParamID {"Feedback", 1 };
const juce::ParameterID stereoParamID{ "Stereo", 1 };
const juce::ParameterID lowCutParamID { "lowCut", 1 };
const juce::ParameterID highCutParamID{ "highCut", 1 };
const juce::ParameterID tempoSyncParamID{ "TempoSync", 1 };
const juce::ParameterID delayNoteParamID{ "delayNote", 1 };
const juce::ParameterID bypassParamID { "Bypass", 1 };

class Parameters
{
public:
    
    //Funciones
    Parameters(juce::AudioProcessorValueTreeState &apvts); //Const
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update() noexcept;
    void prepareToPlay (double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    
    //Varialbes / objetos
    float delayTime = 0.0f;
    float feedback = 0.0f;
    
    float outGain = 0.0f;
    float drySignal = 1.0; //Will reference amount of dry in %
    float wetSignal = 0.5f; //Will reference amount of wet in %
    
    float panL = 0.0f;
    float panR = 1.0f;
    
    float lowCut = 20.0f;
    float highCut = 20000.0f;
    
    int delayNote = 0;
    bool tempoSync = false;
    
    juce::AudioParameterBool *tempoSyncParam; //Usado para parametros prendido/apagado

    
    static constexpr float minDelayTime = 5.0f; //t en ms
    static constexpr float maxDelayTime = 5000.0f;  //here static means it can be called as a variable instead of class member
    
    bool bypassed = false;
    juce::AudioParameterBool *bypassParam;
        
private:
    
    //Varialbes
    juce::AudioParameterFloat *outGainParam;
    juce::LinearSmoothedValue<float> outGainSmoother;
    
    juce::AudioParameterFloat *drySignalParam;
    juce::LinearSmoothedValue<float> drySignalSmoother;
    
    juce::AudioParameterFloat *wetSignalParam;
    juce::LinearSmoothedValue<float> wetSignalSmoother;
    
    juce::AudioParameterFloat *delayTimeParam;
    float targetDelayTime = 0.0f; //The one pole filter will try to reach this value
    float delayOnePoleCoeff = 0.0f; // Determines how fast the smoothing will happen
    
    juce::AudioParameterFloat *feedbackParam;
    juce::LinearSmoothedValue<float> feedbackSmoother;
    
    juce::AudioParameterFloat *stereoParam;
    juce::LinearSmoothedValue<float> stereoSmoother;
    
    juce::AudioParameterFloat *lowCutParam;
    juce::LinearSmoothedValue<float> lowCutSmoother;
    
    juce::AudioParameterFloat *highCutParam;
    juce::LinearSmoothedValue<float> highCutSmoother;
    
    juce::AudioParameterChoice *delayNoteParam; //Usado para parametros con opciones limitadas
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameters)
};
