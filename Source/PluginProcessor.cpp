/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayRound2AudioProcessor::DelayRound2AudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ), params(apvts)
{
    //No haga nada
}

DelayRound2AudioProcessor::~DelayRound2AudioProcessor()
{
}

//==============================================================================
const juce::String DelayRound2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayRound2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayRound2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayRound2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayRound2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayRound2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayRound2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayRound2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayRound2AudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayRound2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayRound2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();
}

void DelayRound2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayRound2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#

void DelayRound2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]]juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //Cleaning
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //DSP
    params.update();
    
    float *channelDataL = buffer.getWritePointer(0);
    float *channelDataR = buffer.getWritePointer(1);
    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
        
        params.smoothen();
        
        channelDataL[sample] *= params.gain;
        channelDataR[sample] *= params.gain;
    }
    
 
}

//==============================================================================
bool DelayRound2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayRound2AudioProcessor::createEditor()
{
    return new DelayRound2AudioProcessorEditor (*this);
}

//==============================================================================
void DelayRound2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(),destData);
}

void DelayRound2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayRound2AudioProcessor();
}





    

    

