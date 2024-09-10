

#include <JuceHeader.h>
#include "LevelMeter.h"
#include "LookAndFeel.h"
//==============================================================================
LevelMeter::LevelMeter(std::atomic<float> &measurementL_,
                       std::atomic<float> &measurementR_)
    :measurementL(measurementL_), measurementR(measurementR_),
     dBLevelL(clampdB), dBLevelR(clampdB)
{
    setOpaque(true);
    startTimerHz(1);

}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint (juce::Graphics& g) {
    const auto bounds = getLocalBounds();
    g.fillAll(Colors::LevelMeter::background);
    drawLevel(g, dBLevelL, 0, 7);
    drawLevel(g, dBLevelR, 9, 7);
    g.setFont(10.00f);
    
    for (float db = maxdB; db >= mindB; db -= stepdB) {
        
        int y = positionForLevel(db);
        g.setColour(Colors::LevelMeter::tickLine);
        g.fillRect(0, y, 10, 1);
        g.setColour(Colors::LevelMeter::tickLabel);
        g.drawSingleLineText(juce::String(int(db)), bounds.getWidth(), y + 3, juce::Justification::right);
    }
    
}
void LevelMeter::resized()
{
    maxPos = 4.0f;
    minPos = float(getHeight() - 4.0f);

}

//UserAdded
void LevelMeter::timerCallback(){

    dBLevelL = juce::Decibels::gainToDecibels(measurementL.load(), clampdB);
    dBLevelR = juce::Decibels::gainToDecibels(measurementR.load(), clampdB);
    
}

void LevelMeter::drawLevel(juce::Graphics& g, float level, int x, int width) {
    int y = positionForLevel(level);
    if (level > 0.0f) {
        int y0 = positionForLevel(0.0f);
        g.setColour(Colors::LevelMeter::tooLoud);
        g.fillRect(x, y, width, y0 - y);
        g.setColour(Colors::LevelMeter::levelOK);
        g.fillRect(x, y0, width, getHeight() - y0);
        
    }else if (y < getHeight()) {
        g.setColour(Colors::LevelMeter::levelOK);
        g.fillRect(x, y, width, getHeight() - y);
    }
}
