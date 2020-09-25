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
    font.setHeight (25.0f);
    value.setName ((aCoeff ? "a" : "b") + String (ID));
    
    isA0 = (ID == 0 && aCoeff == true) ? true : false;
    if (!isA0)
    {
        addAndMakeVisible (value);
        value.setInputRestrictions (0, "0123456789.eE-");
        value.setFont (font);
        value.setText ((ID == 0 && !aCoeff) ? "1" : "");
        value.setMultiLine (false);
        value.setTextToShowWhenEmpty ("0", Colours::white);
    }
}

CoefficientComponent::~CoefficientComponent()
{
    std::cout << ID << " is removed from the heap." << std::endl;
}

void CoefficientComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
//    g.setFont (font.withStyle (Font::italic));
    g.setFont (font);
    g.setColour (Colour (Global::textColour));
    g.drawText ((aCoeff ? "a " : "b "), getLocalBounds().withX (5), Justification::centredLeft);
    g.setFont (font.withHeight (16.0f));
    g.drawText (String (ID), 14, getHeight() * 0.58, 8, 8, Justification::centred);
    g.setFont (font);
    g.drawText (String(" =") + (isA0 ? " 1" : ""), getLocalBounds().withX (23), Justification::centredLeft);
}

void CoefficientComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> valueArea = getLocalBounds();
    valueArea.removeFromLeft (Global::margin);
    valueArea.removeFromLeft (font.getStringWidth ((aCoeff ? "a  =" : "b  =")));
    value.setBounds (valueArea.reduced (Global::margin * 0.75));
}

void CoefficientComponent::mouseDown (const MouseEvent& e)
{
    curX = e.x;
    curVal = value.getText().getDoubleValue();
};

void CoefficientComponent::mouseDrag (const MouseEvent& e)
{
    diffX = e.x - curX;
    value.setText (String(curVal + 0.01 * diffX));
};
