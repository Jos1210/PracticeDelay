#pragma once

//Includes
#include <JuceHeader.h>

//Consts
const juce::ParameterID outGainParamID { "outGain", 1 };
const juce::ParameterID delayTimeParamID { "delayTime", 1 };
const juce::ParameterID drySignalParamID {"Dry", 1};
const juce::ParameterID wetSignalParamID {"Wet", 1};


class Parameters
{
public:
    //Varialbes / objetos
    float outGain = 0.0f;
    float delayTime = 0.0f;
    float drySignal = 1.0; //Will reference amount of dry in %
    float wetSignal = 0.5f; //Will reference amount of wet in %
    
    static constexpr float minDelayTime = 5.0f; //t en ms
    static constexpr float maxDelayTime = 5000.0f;
    
    //Funciones
    Parameters(juce::AudioProcessorValueTreeState &apvts); //Const
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update() noexcept;
    void prepareToPlay (double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    
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
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameters)
};
