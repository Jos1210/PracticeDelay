#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState &apvts,
                          const juce::ParameterID &id, T &destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination); //Checks if param exists or is wrong
}

Parameters::Parameters(juce::AudioProcessorValueTreeState &apvts){
    
    castParameter(apvts, gainParamID, gainParam);
    
}

juce::AudioProcessorValueTreeState::ParameterLayout
    Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            gainParamID, //ID {string, version (Int)}
            "Output Gain", // User visible text
            juce::NormalisableRange<float> { -12.0f, 12.0f }, //Range of Values
            0.0f) //Default Value
        );
        
        return layout;
}

void Parameters::update() noexcept{
    /*
    Obtiene el valor actual de gain y si es distinto al anterior, prepara el
     smoother para actuar y remover el zipper noise; setea el target
    */
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
}

void Parameters::prepareToPlay(double sampleRate) noexcept{
    
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration); //le dice al smoother el tiempo de transición entre muestras y lo inicializa
}

void Parameters::reset() noexcept{
    
    gain = 0.0f;
    
    gainSmoother.setCurrentAndTargetValue( //Setea Gain a 0 por precaución antes de repr
        juce::Decibels::decibelsToGain(gainParam->get()));
}

void Parameters:: smoothen() noexcept{
    gain = gainSmoother.getNextValue(); //Lee el valor que entrega el smoother
}
