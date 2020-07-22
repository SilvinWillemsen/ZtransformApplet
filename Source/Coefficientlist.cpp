/*
  ==============================================================================

    Coefficientlist.cpp
    Created: 20 Jul 2020 2:43:14pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Coefficientlist.h"

//==============================================================================
Coefficientlist::Coefficientlist()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    coefficients.resize (Global::numCoeffs);
    list.setColour (ListBox::backgroundColourId, getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        
    for (int i = 0; i < Global::numCoeffs; ++i)
        coeffComps.add (new CoefficientComponent (i < Global::numCoeffs * 0.5 ? i : i - Global::numCoeffs * 0.5, i < Global::numCoeffs * 0.5));
    
    refreshCoefficients();
    addAndMakeVisible (list);
}

Coefficientlist::~Coefficientlist()
{
    coeffComps.clear();
    list.updateContent();

}

void Coefficientlist::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
}

void Coefficientlist::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    list.setBounds (getLocalBounds());
    list.setRowHeight (getHeight() / static_cast<float> (Global::numCoeffs));

}

void Coefficientlist::paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
    
//    auto alternateColour = getLookAndFeel().findColour (ListBox::backgroundColourId).interpolatedWith (getLookAndFeel().findColour (ListBox::textColourId), 0.03f);
//    auto alternateColour2 = getLookAndFeel().findColour (ListBox::backgroundColourId).interpolatedWith (Colours::darkgrey, 0.03f);
//
    g.fillAll (Colour (rowNumber % 2 ?  Global::backgroundColour : Global::alternateColour));
}

Component* Coefficientlist::refreshComponentForRow (int rowNumber, bool isRowSelected, Component* existingComponentToUpdate)
{
    if (rowNumber < Global::numCoeffs)
        return coeffComps[rowNumber];
    
    return nullptr;
}

std::vector<double>& Coefficientlist::getCoefficients()
{
    return coefficients;
}


void Coefficientlist::updateCoeff (int idx)
{
        coefficients[idx] = coeffComps[idx]->getCoefficient();
}

void Coefficientlist::refreshCoefficients()
{
    for (int i = 0; i < Global::numCoeffs; ++i)
        coefficients[i] = coeffComps[i]->getCoefficient();
}
