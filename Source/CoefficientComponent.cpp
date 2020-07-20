/*
  ==============================================================================

    CoefficientComponent.cpp
    Created: 20 Jul 2020 3:07:18pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CoefficientComponent.h"

//==============================================================================
CoefficientComponent::CoefficientComponent (int ID, bool aCoeff) : ID (ID), aCoeff (aCoeff)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    isA0 = (ID == 0 && aCoeff == true) ? true : false;
    if (!isA0)
    {
        addAndMakeVisible (value);
        value.setInputRestrictions (0, "0123456789.eE-");
        value.setFont (font);
        value.setText ((ID == 0 && !aCoeff) ? "1" : "0");
        value.setMultiLine (false);
    }
}

CoefficientComponent::~CoefficientComponent()
{
}

void CoefficientComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.setFont (font);
    g.setColour (Colours::white);
    g.drawText ((aCoeff ? "a " : "b ") + String(" =") + (isA0 ? " 1" : ""), getLocalBounds().withX (5), Justification::centredLeft);
    g.setFont (font.withHeight (18.0f));
    g.drawText (String (ID), 13, getHeight() * 0.6, 8, 8, Justification::centred);
}

void CoefficientComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> valueArea = getLocalBounds();
    valueArea.removeFromLeft (getWidth() * 0.4);
    value.setBounds (valueArea.reduced (6, getHeight() * 0.25));
}
