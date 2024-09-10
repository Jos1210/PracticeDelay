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
    lowCutFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    highCutFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
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
    
    //Process specification object
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock);
    spec.numChannels = 2;
    
    
    //Inicializa el Delay
    double numSamples = (Parameters::maxDelayTime / 1000.0f) * sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));
    
    delayLineL.setMaximumDelayInSamples(maxDelayInSamples);
    delayLineL.reset();
    
    delayLineR.setMaximumDelayInSamples(maxDelayInSamples);
    delayLineR.reset();
    
    //Feedback
    feedbackL = 0.0f;
    feedbackR = 0.0f;
    
    //Filtering
    lowCutFilter.prepare(spec);
    highCutFilter.prepare(spec);
    lowCutFilter.reset();
    highCutFilter.reset();
    
    lastLowCut = -1.0f;
    lastHighCut = -1.0f;
    
    tempo.reset();
    
    levelL.store(0.0f); //Mejor usar Store para indicar en contexto el uso de una variable atómica
    levelR.store(0.0f);

}

void DelayRound2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayRound2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto mono = juce::AudioChannelSet::mono();
    const auto stereo = juce::AudioChannelSet::stereo();
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();
    
    if (mainIn == mono && mainOut == mono)
        return true;
    
    if (mainIn == mono && mainOut == stereo)
        return true;
    
    if (mainIn == stereo && mainOut == stereo)
        return true;
    
    return false;
}

void DelayRound2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]]juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    //Get inpunts and outputs
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //Cleaning
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //DSP
    float sampleRate = float(getSampleRate());

    
    params.update();
    
    //Calculate Tempo
    tempo.update(getPlayHead());
    float syncedTime = float(tempo.getMillisecondsForNoteLength(params.delayNote));
    if(syncedTime > Parameters::maxDelayTime)
        syncedTime = Parameters::maxDelayTime;
    
    //get Audio buffer data
    
    /*
    float *channelDataL = buffer.getWritePointer(0) //Obtiene valores del buffer del canal L
    float *channelDataR = buffer.getWritePointer(1)
     */
    
    //For mono and stereo  bus compatibility
    auto mainInput = getBusBuffer(buffer, true, 0);
    auto mainInputChannels = mainInput.getNumChannels();
    auto isMainInputStereo = mainInputChannels > 1;
    const float *inputDataL = mainInput.getReadPointer(0);
    const float *inputDataR = mainInput.getReadPointer(isMainInputStereo ? 1 : 0);
    
    auto mainOutput = getBusBuffer(buffer, false, 0);
    auto mainOutputChannels = mainOutput.getNumChannels();
    auto isMainOutputStereo = mainOutputChannels > 1;
    float *outputDataL = mainOutput.getWritePointer(0);
    float *outputDataR = mainOutput.getWritePointer(isMainOutputStereo ? 1 : 0);
    
    //Max levels
    float maxL = 0.0f;
    float maxR = 0.0f;
    
    //Dsp process block
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
        
        params.smoothen();
        
        float delayTime = params.tempoSync ? syncedTime : params.delayTime;
        float delayInSamples = delayTime / 1000.0f * sampleRate;
                
        //Filtros
        if (params.lowCut != lastLowCut){
            lowCutFilter.setCutoffFrequency(params.lowCut);
            lastLowCut = params.lowCut;
        }
        
        if (params.highCut != lastHighCut){
            highCutFilter.setCutoffFrequency(params.highCut);
            lastHighCut = params.highCut;
        }
        
        float dryL = inputDataL[sample]; //Lee los valores que entran
        float dryR = inputDataR[sample];
        float dryMono = (dryL+dryR) * 0.5f;
        
        delayLineL.write(dryMono * params.panL + feedbackR); //meter valor actual en delay line
        delayLineR.write(dryMono * params.panR + feedbackL);
        
        dryL *= params.drySignal; //Aplica el % de Dry para la salida
        dryR *= params.drySignal;
        
        float wetL = delayLineL.read(delayInSamples) * params.wetSignal;
        float wetR = delayLineR.read(delayInSamples) * params.wetSignal;
        
        /*
        float wetL = delayLine.popSample(0) * params.wetSignal; //Leer valor del pasado
        float wetR = delayLine.popSample(1) * params.wetSignal;
        */
        
        //Define feedback y filtra el feedback
        feedbackL = wetL * params.feedback;
        feedbackL = lowCutFilter.processSample(0, feedbackL);
        feedbackL = highCutFilter.processSample(0, feedbackL);
        
        feedbackR = wetR * params.feedback;
        feedbackR = lowCutFilter.processSample(1, feedbackR);
        feedbackR = highCutFilter.processSample(1, feedbackR);

        float mixL = dryL + wetL;
        float mixR = dryR + wetR;
        
        float outL = mixL * params.outGain;
        float outR = mixR * params.outGain;
        
        outputDataL[sample] = outL;
        outputDataR[sample] = outR;
        
        maxL = std::max(maxL, std::abs(outL)); //Max regresa el valor mas grande entre los dos entregados
        maxR = std::max(maxR, std::abs(outR));
    }
    levelL.store(maxL);
    levelR.store(maxR);
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





    

    

