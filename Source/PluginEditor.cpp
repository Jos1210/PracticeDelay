/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayRound2AudioProcessorEditor::DelayRound2AudioProcessorEditor (DelayRound2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    addAndMakeVisible(delayGroup);
    
    feedbackGroup.setText("Feedback");
    feedbackGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    feedbackGroup.addAndMakeVisible(feedbackKnob);
    feedbackGroup.addAndMakeVisible(stereoKnob);
    addAndMakeVisible(feedbackGroup);
    
    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    outputGroup.addAndMakeVisible(outGainKnob);
    outputGroup.addAndMakeVisible(drySignalKnob);
    outputGroup.addAndMakeVisible(wetSignalKnob);
    addAndMakeVisible(outputGroup);

    setSize (500, 500);
}

DelayRound2AudioProcessorEditor::~DelayRound2AudioProcessorEditor()
{
}

//==============================================================================
void DelayRound2AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::indigo);
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
    
    feedbackKnob.setTopLeftPosition(20, 20);
    stereoKnob.setTopLeftPosition(feedbackKnob.getX(), feedbackKnob.getBottom() +10);
    
    drySignalKnob.setTopLeftPosition(20, 20);
    wetSignalKnob.setTopLeftPosition(drySignalKnob.getX(), drySignalKnob.getBottom() +10);
    outGainKnob.setTopLeftPosition(drySignalKnob.getX(), wetSignalKnob.getBottom() +10);
    
    
}
