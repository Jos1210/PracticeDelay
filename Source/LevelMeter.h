#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LevelMeter  : public juce::Component, private::juce::Timer
{
public:
    
    LevelMeter(std::atomic<float> &measurementL,
               std::atomic<float> &measurementR);
    
    ~LevelMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    std::atomic<float> &measurementL;
    std::atomic<float> &measurementR;
    
    
    static constexpr float maxdB = 6.0f;
    static constexpr float mindB = -60.0f;
    static constexpr float stepdB = 6.0f;
    
    float maxPos = 0.0f;
    float minPos = 0.0f;
    
    static constexpr float clampdB = -120.0f;
    static constexpr float clampLevel = 0.000001f;
    
    float dBLevelL, dBLevelR;
    
    //functions
    int positionForLevel(float dbLevel) const noexcept{
        return int(std::round(juce::jmap(dbLevel, maxdB, mindB, maxPos, minPos)));
    }
    
    void timerCallback() override;
    void drawLevel(juce::Graphics &g, float level, int x, int width);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
