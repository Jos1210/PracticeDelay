/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"
#include "Tempo.h"
#include "DelayLine.h"

//constantes


//==============================================================================
/**
*/
class DelayRound2AudioProcessor  : public juce::AudioProcessor
{
public:
    
    //==============================================================================
    DelayRound2AudioProcessor();
    ~DelayRound2AudioProcessor() override;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const juce::String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //User Defined
    
    //Functions
    juce::AudioProcessorParameter* getBypassParameter() const override;
    
    //Variables
    juce::AudioProcessorValueTreeState apvts{
        *this, nullptr, "Parameters", Parameters::createParameterLayout()
    };
    
    Parameters params; // debe ir debajo de la inicialización del apvts
    
    //std::atomic<float> levelL, levelR; //LAs variables atómicas avisan al compilador que
    //Estas variables pueden ser usadas por multiples hilos de ejecución
    
    /* xfade approach
    float delayInSamples = 0.0f;
    float targetDelay = 0.0f;
    float xfade = 0.0f;
    float xfadeInc = 0.0f; //StepSize for the xfade
    */
    
    //Ducking approach with 1 pole filtering
    float delayInSamples = 0.0f;
    float targetDelay = 0.0f;
    float fade = 0.0f;
    float fadeTarget = 0.0f;
    float coeff = 0.0f;
    float wait = 0.0f;
    float waitInc = 0.0f; //StepSize
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayRound2AudioProcessor)
    
    //User added
    
    //Varialbes/Objetos
    float feedbackL = 0.0f;
    float feedbackR = 0.0f;

    float lastLowCut = -1.0f;
    float lastHighCut = -1.0f;

    DelayLine delayLineL, delayLineR;
    
    juce::dsp::StateVariableTPTFilter<float> lowCutFilter;
    juce::dsp::StateVariableTPTFilter<float> highCutFilter;
    
    Tempo tempo;
    
    //Funciones

};

