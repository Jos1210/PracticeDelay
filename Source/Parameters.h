#pragma once

//Includes
#include <JuceHeader.h>

//Consts
const juce::ParameterID gainParamID { "gain", 1 };

class Parameters
{
public:
    //Varialbes / objetos
    juce::AudioParameterFloat* gainParam;

    
    
    //Funciones
    Parameters(juce::AudioProcessorValueTreeState& apvts); //Const
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
};
