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
    drawTitle (g);
    drawOutline (g);

    Font font = equationFont;
    while (font.getStringWidth (equation) > getWidth() - 2.0 * Global::margin)
        font = font.withHeight(font.getHeight() - 0.1);
    
    g.setFont (font);
    g.drawText (equation, Global::margin, 30 + Global::margin, getWidth() - 2.0 * Global::margin, 25, Justification::centredLeft);
}

void DifferenceEq::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void DifferenceEq::calculate()
{
    equation = "y[n] = ";
    
    // x-component
    for (int i = 0; i < Global::numCoeffs * 0.5; ++i)
    {
        if (coefficients[i] == 0)
            continue;
        
        // if it is not the first entry
        if (equation != "y[n] = ")
        {
            if (coefficients[i] > 0)
                equation += " + ";
            else
                equation += " - ";
        }
        
        if (abs(coefficients[i]) != 1)
            equation += String (abs (coefficients[i]));
        
        equation += "x[n";
        if (i != 0)
        {
            equation += " - ";
            equation += String (i);
        }
        equation += "]";
        
        if (i == Global::numCoeffs - 1)
            break;
        
    }
    
    // y-component
    for (int i = Global::numCoeffs * 0.5 + 1; i < Global::numCoeffs; ++i)
    {
        if (coefficients[i] == 0)
            continue;
        
//        if (coefficients[i] < 0)
//            equation = equation.dropLastCharacters(2);
        
        // if it is not the first entry
        if (equation != "y[n] = ")
        {
            if (coefficients[i] > 0)
                equation += " + ";
            else
                equation += " - ";
        }
        
        if (abs (coefficients[i]) != 1)
            equation += String (abs (coefficients[i]));

        equation += "y[n - ";
        if (i != 0)
            equation += String (i - Global::numCoeffs * 0.5);
        equation += "] ";
//        equation += "+ ";
    }
    
    // if there are no values, set y[n] equal to 0
    if (equation ==  "y[n] = ")
        equation += "0";
//    else // otherwise remove the last "+ "
//        equation = equation.dropLastCharacters(2);
}
