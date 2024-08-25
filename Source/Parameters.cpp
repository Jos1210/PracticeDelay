#include "Parameters.h"

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts){
    
    auto* param = apvts.getParameter(gainParamID.getParamID());
    gainParam = dynamic_cast<juce::AudioParameterFloat*>(param);
    
}

juce::AudioProcessorValueTreeState::ParameterLayout
    Parameters::createParameterLayout()
{
        juce::AudioProcessorValueTreeState::ParameterLayout layout;
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID, // Identificador {id(string), versión (int)}
        "Output Gain", // String que ve el usuario
        juce::NormalisableRange<float> { -12.0f, 12.0f }, //Rango de valores
        0.0f)); // Valor default
        
        return layout;
}
