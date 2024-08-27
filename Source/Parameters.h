#pragma once

//Includes
#include <JuceHeader.h>

//Consts
const juce::ParameterID gainParamID { "gain", 1 };
const juce::ParameterID delayTimeParamID { "delayTime", 1 };
const juce::ParameterID mixParamID { "mix", 1 };



class Parameters
{
public:
    //Varialbes / objetos
    float gain = 0.0f;
    float delayTime = 0.0f;
    float mix = 1.0f;
    
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
    juce::AudioParameterFloat *gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
    
    juce::AudioParameterFloat *delayTimeParam;
    float targetDelayTime = 0.0f; //The one pole filter will try to reach this value
    float delayOnePoleCoeff = 0.0f; // Determines how fast the smoothing will happen
    
    juce::AudioParameterFloat *mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;
    
};
