#include "Parameters.h"

//Helper functions

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState &apvts,
                          const juce::ParameterID &id, T &destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination); //Checks if param exists or is wrong
}

static juce::String stringFromMilliseconds(float value, int){
    if (value < 10.0f){
        return juce::String(value, 2) + " ms"; //(valor, n decimales)
    }else if (value < 100.0f){
        return juce::String(value, 1) + " ms";
    }else if(value < 1000.0f){
        return juce::String(int(value)) + "ms";
    }else{
        return juce::String(value * 0.001f, 2) + " s";
    }
}

static juce::String stringFromDecibels(float value, int){       //Static menas it can only be used in this file
    
    return juce::String(value, 1) + "dB";
}

static juce::String stringFromPercent(float value, int){
    return juce::String(int(value)) + " %";
}

//Constructor
Parameters::Parameters(juce::AudioProcessorValueTreeState &apvts){
    
    castParameter(apvts, gainParamID, gainParam); //Asigna un puntero para ese param
    castParameter(apvts, delayTimeParamID, delayTimeParam);
    castParameter(apvts, mixParamID, mixParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout
    Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            gainParamID, //ID {string, version (Int)}
            "Output Gain", // User visible text
            juce::NormalisableRange<float> { -12.0f, 12.0f }, //Range of Values
            0.0f, //Default Value
            juce::AudioParameterFloatAttributes() //Changes the way values are displayed, receives a function
                .withStringFromValueFunction(stringFromDecibels)
            ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            delayTimeParamID, //ID {string, version (Int)}
            "Delay Time", // User visible text
            juce::NormalisableRange<float> {minDelayTime, maxDelayTime, 0.001f, 0.25f},
                                        // {range min, range max, step size, skew factor}
            100.0f,
            juce::AudioParameterFloatAttributes() //Changes the way values are displayed
                .withStringFromValueFunction(stringFromMilliseconds)
        ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            mixParamID, //ID {string, version (Int)}
            "Mix", // User visible text
            juce::NormalisableRange<float> {0.0f, 100.0f, 1.0f},
                                        // {range min, range max, step size, skew factor}
            50.0f,
            juce::AudioParameterFloatAttributes() //Changes the way values are displayed
                .withStringFromValueFunction(stringFromPercent)
        ));
        
        return layout;
}

void Parameters::update() noexcept{
    /*
    Obtiene el valor actual de gain y si es distinto al anterior, prepara el
     smoother para actuar y remover el zipper noise; setea el target
    */
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    //Delay smoothing and set value
    targetDelayTime = delayTimeParam ->get();
    if(delayTime == 0.0f)
        delayTime = targetDelayTime;
    
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    
}

void Parameters::prepareToPlay(double sampleRate) noexcept{
    
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration); //le dice al smoother el tiempo de transición entre muestras y lo inicializa
    
    delayOnePoleCoeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate))); //Investigar funcionamiento de One pole filters
    //0.2 = despues de 200 ms el onepole alcanzara el target en un 63.2%, esta formula imita la carga de un condensador
    //Entre mas pequeño el coef, mas tardara el filtro en alcanzar el target
    
    mixSmoother.reset(sampleRate, duration);

}

void Parameters::reset() noexcept{
    
    gain = 0.0f;
    gainSmoother.setCurrentAndTargetValue( //Setea Gain a 0 por precaución antes de repr
        juce::Decibels::decibelsToGain(gainParam->get())
    );
    
    delayTime = 0.0f;
    
    mix = 5.0f;
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);
    
}

void Parameters:: smoothen() noexcept{
    gain = gainSmoother.getNextValue(); //Lee el valor que entrega el smoother
    
    delayTime += (targetDelayTime - delayTime) * delayOnePoleCoeff;
    
    mix = mixSmoother.getNextValue();
}
