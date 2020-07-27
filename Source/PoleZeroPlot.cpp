/*
  ==============================================================================

    PoleZeroPlot.cpp
    Created: 22 Jul 2020 1:55:43pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PoleZeroPlot.h"

//==============================================================================
PoleZeroPlot::PoleZeroPlot() : AppComponent ("Pole-Zero Plot", false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
}

PoleZeroPlot::~PoleZeroPlot()
{
}

void PoleZeroPlot::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    drawTitle (g);
    drawOutline (g);
    
    double diameter = getWidth() - 2.0 * (Global::pzAxisMargin + Global::margin);
    
    // draw unit circle
    g.setColour(Colours::grey);
    g.drawEllipse (Global::pzAxisMargin + Global::margin,
                  Global::pzAxisMargin + Global::margin,
                  diameter, diameter, 1.0);
    
    // draw axes
    g.drawLine (getWidth() * 0.5, Global::margin + 0.5 * Global::pzAxisMargin, getWidth() * 0.5, getHeight() - Global::margin - 0.5 * Global::pzAxisMargin);
    g.drawLine (Global::margin + 0.5 * Global::pzAxisMargin, getHeight() * 0.5, getWidth() - Global::margin - 0.5 * Global::pzAxisMargin, getHeight() * 0.5);
    
    g.setColour (Colours::black);
    for (int i = 0; i < highestXOrder; ++i)
        g.drawEllipse (getWidth() * 0.5 + Global::limit (realZeroVector[i], -100, 100) * diameter * 0.5 - 3,
                       getHeight() * 0.5 - Global::limit (imaginaryZeroVector[i], -100, 100) * diameter * 0.5 - 3,
                       6, 6, 1);
    
    // draw axes labels
    g.setFont (equationFont.withHeight(20.0f));
    
    g.drawText("Re", getWidth() - Global::margin - 0.5 * Global::pzAxisMargin - 10, getHeight() * 0.5 + 3, 20, 20, Justification::centred);
    g.drawText("Im", getWidth() * 0.5 + 3, Global::margin + 0.5 * Global::pzAxisMargin - 5, 20, 20, Justification::centred);
    
    for (int i = 0; i < highestYOrder; ++i)
    {
        float xSize = 4;
        g.drawLine(getWidth() * 0.5 + realPoleVector[i] * diameter * 0.5 - xSize,
                   getHeight() * 0.5 - imaginaryPoleVector[i] * diameter * 0.5 - xSize,
                   getWidth() * 0.5 + realPoleVector[i] * diameter * 0.5 + xSize,
                   getHeight() * 0.5 - imaginaryPoleVector[i] * diameter * 0.5 + xSize,
                   1);
        g.drawLine(getWidth() * 0.5 + realPoleVector[i] * diameter * 0.5 - xSize,
                   getHeight() * 0.5 - imaginaryPoleVector[i] * diameter * 0.5 + xSize,
                   getWidth() * 0.5 + realPoleVector[i] * diameter * 0.5 + xSize,
                   getHeight() * 0.5 - imaginaryPoleVector[i] * diameter * 0.5 - xSize,
                   1);
//        Rectangle<float> poleBounds (getWidth() * 0.5 + realPoleVector[i] * diameter * 0.5 - 8,
//                                     getHeight() * 0.5 - imaginaryPoleVector[i] * diameter * 0.5 - 8,
//                                     16, 16);
//        g.drawText (String (CharPointer_UTF8 ("\xc3\x97")),
//                    poleBounds, Justification::centred);
    }

}

void PoleZeroPlot::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void PoleZeroPlot::calculate()
{
    findRoots();
}

void PoleZeroPlot::findRoots()
{
    // x roots
    highestXOrder = 0;
    for (int i = Global::numCoeffs * 0.5 - 1; i >= 0; --i)
    {
        if (coefficients[i] != 0)
        {
            highestXOrder = i;
            break;
        }
    }
    
    realZeroVector.clear();
    realZeroVector.resize(Global::numCoeffs * 0.5, 0);
    
    imaginaryZeroVector.clear();
    imaginaryZeroVector.resize(Global::numCoeffs * 0.5, 0);

    std::vector<double> xCoeffs (Global::numCoeffs * 0.5);
    
    // invert order of coefficients for Polynomial class
    for (int i = 0; i <= highestXOrder; ++i)
    {
        xCoeffs[highestXOrder - i] = coefficients [i];
    }
    
    Polynomial xPolynomial (&xCoeffs[0], highestXOrder);
    int numberOfXRoots;
    xPolynomial.FindRoots (&realZeroVector[0],
                          &imaginaryZeroVector[0],
                          &numberOfXRoots);
    
    // y roots
    highestYOrder = 0;
    for (int i = Global::numCoeffs - 1; i >= Global::numCoeffs * 0.5; --i)
    {
        if (coefficients[i] != 0)
        {
            highestYOrder = i - Global::numCoeffs * 0.5;
            break;
        }
    }
    
    realPoleVector.clear();
    realPoleVector.resize(Global::numCoeffs * 0.5, 0);
    
    imaginaryPoleVector.clear();
    imaginaryPoleVector.resize(Global::numCoeffs * 0.5, 0);
    
    // invert order of coefficients for polynomial
    std::vector<double> yCoeffs (Global::numCoeffs * 0.5, 0);
    yCoeffs[highestYOrder] = 1;
    
    for (int i = 0; i < highestYOrder; ++i)
    {
        // also flip the sign of the coefficients in the case of y
        yCoeffs[highestYOrder - i - 1] = -coefficients[i + 1 - Global::numCoeffs * 0.5];
    }

    Polynomial yPolynomial (&yCoeffs[0], highestYOrder);
    int numberOfYRoots;
    yPolynomial.FindRoots (&realPoleVector[0],
                           &imaginaryPoleVector[0],
                           &numberOfYRoots);
}

void PoleZeroPlot::invertYCoeffs()
{
    for (int i = Global::numCoeffs * 0.5 + 1; i < Global::numCoeffs; ++i)
    {
        coefficients[i] = -coefficients[i];
    }
}
