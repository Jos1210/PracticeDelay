/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 26 Aug 2024 8:53:15pm
    Author:  Jose Gonzalez

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String &text,
                       juce::AudioProcessorValueTreeState &apvts,
                       const juce::ParameterID &parameterID)
    :attachment(apvts, parameterID.getParamID(), slider)
{
    
    
    //Slider
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag); //Por defecto ese es de 70x70 pixeles
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16); //(texto box location, ReadOnly, Width, Height)
    slider.setBounds(0, 0, 70, 86); //Posición estandar del componente (x, y, width, height)
    addAndMakeVisible(slider);
    
    //Label
    label.setText(text, juce::NotificationType::dontSendNotification); //NotificationType...Para no enviar notif a "listeners"
    label.setText("Out Gain", juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred); //Donde se va a ubicar según el componente
    label.setBorderSize(juce::BorderSize<int>(0)); // "Empuja" el componente desde una dirección (top, left, bottom right)
    label.attachToComponent(&slider, false); //Indica que siempre ira pegado a este componente
    addAndMakeVisible(label); //Lo hace visible
    
    setSize(70, 110); //Tamaño completo del componente, 70 del knob (W) y 110 (h) [70 knob + 24 label + 16 bottom text]
}

RotaryKnob::~RotaryKnob()
{
}


void RotaryKnob::resized() //Para definir posicion de child components
{
    slider.setTopLeftPosition(0, 24);

}
