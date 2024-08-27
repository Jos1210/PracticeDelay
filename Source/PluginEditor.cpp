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
    addAndMakeVisible(outGainKnob);
    setSize (500, 330);
}

DelayRound2AudioProcessorEditor::~DelayRound2AudioProcessorEditor()
{
}

//==============================================================================
void DelayRound2AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::beige);
}

void DelayRound2AudioProcessorEditor::resized()
{
    outGainKnob.setTopLeftPosition(215, 100);
}
