/*
  ==============================================================================

    DifferenceEq.cpp
    Created: 21 Jul 2020 10:53:27am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DifferenceEq.h"

//==============================================================================
DifferenceEq::DifferenceEq() : AppComponent ("Difference Equation", true)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

DifferenceEq::~DifferenceEq()
{
}

void DifferenceEq::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.setFont (textFont);
    g.setColour (Colour (Global::textColour));
    g.drawText (getTitle(), 0, 0, getWidth() * 0.5, 25, Justification::centredLeft);
    g.setFont (equationFont);
    g.drawText (equation, 0, 25 + Global::margin, getWidth(), 25, Justification::centredLeft);
}

void DifferenceEq::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void DifferenceEq::generateEquation()
{
    equation = "y[n] = ";
    
    // x-component
    for (int i = Global::numCoeffs * 0.5; i < Global::numCoeffs; ++i)
    {
        if (data[i] == 0)
            continue;
        
        if (data[i] < 0)
            equation = equation.dropLastCharacters(2);
        
        if (data[i] != 1)
            equation += String(data[i]);
        
        equation += "x[n";
        if (i != Global::numCoeffs * 0.5)
        {
            equation += " - ";
            equation += String (i - Global::numCoeffs * 0.5);
        }
        equation += "] ";
        equation += "+ ";
    }
    
    // y-component
    for (int i = 1; i < Global::numCoeffs * 0.5; ++i)
    {
        if (data[i] == 0)
            continue;
        
        if (data[i] < 0)
            equation = equation.dropLastCharacters(2);
        
        if (data[i] != 1)
            equation += String(data[i]);
        

        equation += "y[n - ";
        if (i != 0)
            equation += String (i);
        equation += "] ";
        equation += "+ ";
    }
    
    // if there are no values, set y[n] equal to 0
    if (equation ==  "y[n] = ")
        equation += "0";
    else // otherwise remove the last "+ "
        equation = equation.dropLastCharacters(2);
}
