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

static juce::String stringFromHz(float value, int){
    
    if (value < 1000.0f)
        return juce::String(int(value)) + " Hz";
    
    if (value < 10000.0f)
        return juce::String(value / 1000.0f, 2) + " kHz";
    
    return juce::String(value / 1000.0f, 2) + " kHz";
}

static float hzFromString(const juce::String & str){
    
    float value = str.getFloatValue();
    
    if (value < 20.0f)
        return value * 1000.0f;
    
    return value;
}

//Constructor
Parameters::Parameters(juce::AudioProcessorValueTreeState &apvts){
    
    castParameter(apvts, outGainParamID, outGainParam); //Asigna un puntero para ese param
    castParameter(apvts, delayTimeParamID, delayTimeParam);
    castParameter(apvts, drySignalParamID, drySignalParam);
    castParameter(apvts, wetSignalParamID, wetSignalParam);
    castParameter(apvts, feedbackParamID, feedbackParam);
    castParameter(apvts, stereoParamID, stereoParam);
    castParameter(apvts, lowCutParamID, lowCutParam);
    castParameter(apvts, highCutParamID, highCutParam);
    castParameter(apvts, tempoSyncParamID, tempoSyncParam);
    castParameter(apvts, delayNoteParamID, delayNoteParam);
    castParameter(apvts, bypassParamID, bypassParam);

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
        
        //Feedback
        
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
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            lowCutParamID, //ID {string, version (Int)}
            "Low Cut", // User visible text
            juce::NormalisableRange<float> {20.0f, 20000.0f, 1.0f, 0.3f},
                                        // {range min, range max, step size, skew factor}
            20.0f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction(stringFromHz)
                .withValueFromStringFunction(hzFromString)
        ));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            highCutParamID, //ID {string, version (Int)}
            "High Cut", // User visible text
            juce::NormalisableRange<float> {20.0f, 20000.0f, 1.0f, 0.3f},
                                        // {range min, range max, step size, skew factor}
            20000.0f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction(stringFromHz)
                .withValueFromStringFunction(hzFromString)
        ));
        
        layout.add(std::make_unique<juce::AudioParameterBool>(
            tempoSyncParamID,
            "Tempo Sync",
            false //Default Value
        ));
        
        juce::StringArray noteLengths = { "1/32", "1/16 trip", "1/32 dot", "1/16", "1/8 trip", "1/16 dot",
            "1/8", "1/4 trip", "1/8 dot", "1/4", "1/2 trip", "1/4 dot", "1/2", "1/1 trip", "1/2 dot", "1/1"};
        
        layout.add(std::make_unique<juce::AudioParameterChoice>(
            delayNoteParamID,
            "Delay Note",
            noteLengths,
            9 //La posición 9 es la default (1/4)
        ));
        
        //Bypass
        layout.add(std::make_unique<juce::AudioParameterBool>(
            bypassParamID,
            "Bypass",
            false //Default Value
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
    
    lowCutSmoother.setTargetValue(lowCutParam->get());
    highCutSmoother.setTargetValue(highCutParam->get());
    
    delayNote = delayNoteParam->getIndex();
    tempoSync = tempoSyncParam->get();
    
    bypassed = bypassParam->get();
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
    
    lowCutSmoother.reset(sampleRate, duration);
    highCutSmoother.reset(sampleRate, duration);

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
    
    lowCut = 20.0f;
    highCut = 20000.0f;
    
    lowCutSmoother.setCurrentAndTargetValue(lowCutParam->get());
    highCutSmoother.setCurrentAndTargetValue(highCutParam->get());

}

void Parameters:: smoothen() noexcept{
    outGain = outGainSmoother.getNextValue(); //Lee el valor que entrega el smoother
    drySignal = drySignalSmoother.getNextValue();
    wetSignal = wetSignalSmoother.getNextValue();
    
    //delayTime += (targetDelayTime - delayTime) * delayOnePoleCoeff;
    delayTime = targetDelayTime;
    feedback = feedbackSmoother.getNextValue();
    
    panningEqualPower(stereoSmoother.getNextValue(), panL, panR);
    
    lowCut = lowCutSmoother.getNextValue();
    highCut = highCutSmoother.getNextValue();
}
