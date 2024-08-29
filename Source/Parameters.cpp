#include "Parameters.h"
#include "DSP.h"

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


static float millisecondsFromString(const juce::String &text){
    
    float value = text.getFloatValue();
    
    if(!text.endsWithIgnoreCase("ms"))
        if (text.endsWithIgnoreCase("s") || value <Parameters::minDelayTime)
            return value*1000.0f;
    
    return value;
}


static juce::String stringFromDecibels(float value, int){       //Static menas it can only be used in this file
    
    return juce::String(value, 1) + "dB";
}


static juce::String stringFromPercent(float value, int){
    return juce::String(int(value)) + " %";
}


//Constructor
Parameters::Parameters(juce::AudioProcessorValueTreeState &apvts){
    
    castParameter(apvts, outGainParamID, outGainParam); //Asigna un puntero para ese param
    castParameter(apvts, delayTimeParamID, delayTimeParam);
    castParameter(apvts, drySignalParamID, drySignalParam);
    castParameter(apvts, wetSignalParamID, wetSignalParam);
    castParameter(apvts, feedbackParamID, feedbackParam);
    castParameter(apvts, stereoParamID, stereoParam);

}

juce::AudioProcessorValueTreeState::ParameterLayout
    Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
        
        //out Gain
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            outGainParamID, //ID {string, version (Int)}
            "Output Gain", // User visible text
            juce::NormalisableRange<float> { -12.0f, 12.0f }, //Range of Values
            0.0f, //Default Value
            juce::AudioParameterFloatAttributes() //Changes the way values are displayed, receives a function
                .withStringFromValueFunction(stringFromDecibels)
            ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            drySignalParamID, //ID {string, version (Int)}
            "Dry", // User visible text
            juce::NormalisableRange<float> {0.0f, 100.0f, 1.0f},
                                        // {range min, range max, step size, skew factor}
            100.0f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction(stringFromPercent) //Changes the way values are displayed
        ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            wetSignalParamID, //ID {string, version (Int)}
            "Wet", // User visible text
            juce::NormalisableRange<float> {0.0f, 100.0f, 1.0f},
                                        // {range min, range max, step size, skew factor}
            50.0f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction(stringFromPercent)
        ));
        
        //Delay
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            delayTimeParamID, //ID {string, version (Int)}
            "Delay Time", // User visible text
            juce::NormalisableRange<float> {minDelayTime, maxDelayTime, 0.001f, 0.25f},
                                        // {range min, range max, step size, skew factor}
            100.0f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction(stringFromMilliseconds)
                .withValueFromStringFunction(millisecondsFromString) //Changes the way values are given by user
        ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            feedbackParamID, //ID {string, version (Int)}
            "Feedback", // User visible text
            juce::NormalisableRange<float> {0.0f, 100.0f, 1.0f},
                                        // {range min, range max, step size, skew factor}
            40.0f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction(stringFromPercent)
        ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            stereoParamID, //ID {string, version (Int)}
            "Stereo", // User visible text
            juce::NormalisableRange<float> {0.0f, 100.0f, 1.0f},
                                        // {range min, range max, step size, skew factor}
            0.0f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction(stringFromPercent)
        ));
        
        
        
        return layout;
}

void Parameters::update() noexcept{
    /*
    Obtiene el valor actual de gain y si es distinto al anterior, prepara el
     smoother para actuar y remover el zipper noise; setea el target
    */
    outGainSmoother.setTargetValue(juce::Decibels::decibelsToGain(outGainParam->get()));
    drySignalSmoother.setTargetValue(drySignalParam->get() * 0.01f);
    wetSignalSmoother.setTargetValue(wetSignalParam->get() * 0.01f);
    
    //Delay smoothing and set value
    targetDelayTime = delayTimeParam ->get();
    if(delayTime == 0.0f)
        delayTime = targetDelayTime;
    
    feedbackSmoother.setTargetValue(feedbackParam->get() * 0.01f);
    stereoSmoother.setTargetValue(stereoParam->get() * 0.01f);

    
}

void Parameters::prepareToPlay(double sampleRate) noexcept{
    
    double duration = 0.02;
    outGainSmoother.reset(sampleRate, duration); //le dice al smoother el tiempo de transición entre muestras y lo inicializa
    drySignalSmoother.reset(sampleRate, duration);
    wetSignalSmoother.reset(sampleRate, duration);
    
    delayOnePoleCoeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate))); //Investigar funcionamiento de One pole filters
    //0.2 = despues de 200 ms el onepole alcanzara el target en un 63.2%, esta formula imita la carga de un condensador
    //Entre mas pequeño el coef, mas tardara el filtro en alcanzar el target
    
    feedbackSmoother.reset(sampleRate, duration);
    stereoSmoother.reset(sampleRate, duration);
    

}

void Parameters::reset() noexcept{
    
    outGain = 0.0f;
    drySignal = 1.0f;
    wetSignal = 0.0f;
    
    panL = 0.0f;
    panR = 1.0f;
    
    outGainSmoother.setCurrentAndTargetValue( //Setea Gain a 0 por precaución antes de repr
        juce::Decibels::decibelsToGain(outGainParam->get())
    );
    
    drySignalSmoother.setCurrentAndTargetValue(drySignalParam->get() * 0.01f); // 0.01 para deshacer porcentaje
    wetSignalSmoother.setCurrentAndTargetValue(wetSignalParam->get() * 0.01f); // 0.01 para deshacer porcentaje

    
    delayTime = 0.0f;
    
    feedback = 0.0f;
    feedbackSmoother.setCurrentAndTargetValue(feedbackParam->get() * 0.01f);
    
    stereoSmoother.setCurrentAndTargetValue(stereoParam->get() * 0.01f);
   
    
}

void Parameters:: smoothen() noexcept{
    outGain = outGainSmoother.getNextValue(); //Lee el valor que entrega el smoother
    drySignal = drySignalSmoother.getNextValue();
    wetSignal = wetSignalSmoother.getNextValue();
    
    delayTime += (targetDelayTime - delayTime) * delayOnePoleCoeff;
    feedback = feedbackSmoother.getNextValue();
    
    panningEqualPower(stereoSmoother.getNextValue(), panL, panR);
}
