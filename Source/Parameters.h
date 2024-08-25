#pragma once

//Includes
#include <JuceHeader.h>

//Consts
const juce::ParameterID gainParamID { "gain", 1 };

class Parameters
{
public:
    //Varialbes / objetos
    float gain = 0.0f;
    
    
    //Funciones
    Parameters(juce::AudioProcessorValueTreeState &apvts); //Const
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update() noexcept;
    void prepareToPlay (double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    
private:
    
    //Varialbes
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
};
