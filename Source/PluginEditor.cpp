/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayRound2AudioProcessorEditor::DelayRound2AudioProcessorEditor (DelayRound2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), meter(p.levelL, p.levelR)
{
    setLookAndFeel(&mainLF);
    
    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    delayGroup.addChildComponent(delayNoteKnob);
    
    //Sync Button
    tempoSyncButton.setButtonText("Sync");
    tempoSyncButton.setClickingTogglesState(true);
    tempoSyncButton.setBounds(0, 0, 70, 27);
    tempoSyncButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::rosybrown);
    delayGroup.addAndMakeVisible(tempoSyncButton);
    
    addAndMakeVisible(delayGroup);
    
    feedbackGroup.setText("Feedback");
    feedbackGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    feedbackGroup.addAndMakeVisible(feedbackKnob);
    feedbackGroup.addAndMakeVisible(stereoKnob);
    feedbackGroup.addAndMakeVisible(lowCutKnob);
    feedbackGroup.addAndMakeVisible(highCutKnob);
    addAndMakeVisible(feedbackGroup);
    
    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    outputGroup.addAndMakeVisible(outGainKnob);
    outputGroup.addAndMakeVisible(drySignalKnob);
    outputGroup.addAndMakeVisible(wetSignalKnob);
    outputGroup.addAndMakeVisible(meter);
    addAndMakeVisible(outputGroup);
    

    setSize (500, 500);
    
    updateDelayKnobs(audioProcessor.params.tempoSyncParam->get());
    audioProcessor.params.tempoSyncParam->addListener(this);

}

DelayRound2AudioProcessorEditor::~DelayRound2AudioProcessorEditor()
{
    audioProcessor.params.tempoSyncParam->removeListener(this);
    setLookAndFeel(nullptr);
    
}

//==============================================================================
void DelayRound2AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colors::background);
}

void DelayRound2AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    int y = 10;
    int height = bounds.getHeight()-20;
    
    //Posicionamiento de grupos
    delayGroup.setBounds(10, y, 110, height); //(x, y, w, h)
    outputGroup.setBounds(bounds.getWidth() - 160, y, 150, height);
    
    feedbackGroup.setBounds(delayGroup.getRight() +10, y,
                            outputGroup.getX() - delayGroup.getRight() - 20, height
    );
    
    //Posiciond e los knobs dentro de los grupos
    delayTimeKnob.setTopLeftPosition(20, 20);
    tempoSyncButton.setTopLeftPosition(20, delayTimeKnob.getBottom() + 10);
    delayNoteKnob.setTopLeftPosition(delayTimeKnob.getX(), delayTimeKnob.getY());
    
    feedbackKnob.setTopLeftPosition(20, 20);
    stereoKnob.setTopLeftPosition(feedbackKnob.getRight() + 20, feedbackKnob.getY());
    lowCutKnob.setTopLeftPosition(feedbackKnob.getX(), feedbackKnob.getBottom() + 10);
    highCutKnob.setTopLeftPosition(lowCutKnob.getRight() + 20, lowCutKnob.getY());
    
    drySignalKnob.setTopLeftPosition(20, 20);
    wetSignalKnob.setTopLeftPosition(drySignalKnob.getX(), drySignalKnob.getBottom() +10);
    outGainKnob.setTopLeftPosition(drySignalKnob.getX(), wetSignalKnob.getBottom() +10);
    meter.setBounds(outputGroup.getWidth() -45, 30, 30, outGainKnob.getBottom() - 30);
    
}

//User added function definitions

void DelayRound2AudioProcessorEditor::parameterValueChanged(int, float value){
    if (juce::MessageManager::getInstance()->isThisTheMessageThread()){
        
        updateDelayKnobs(value != 0.0f);
    }else{
        juce::MessageManager::callAsync([this, value]{  //Lambda
            updateDelayKnobs(value != 0.0f);    //Lambda define una "mini función" sin nombre usando los valores de this en value
        });
    }
}

void DelayRound2AudioProcessorEditor::updateDelayKnobs(bool tempoSyncActive){
    delayTimeKnob.setVisible(!tempoSyncActive);
    delayNoteKnob.setVisible(tempoSyncActive);
}

