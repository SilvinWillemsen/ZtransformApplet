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
    zoomButton = std::make_unique<TextButton> ("Zoom out");
    zoomButton->setButtonText ("Zoom out");
    addAndMakeVisible (zoomButton.get());
    zoomButton->addListener (this);
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
    switch (unstable) {
        case -1:
            g.fillAll (Colours::white);
            break;
        case 0:
            g.fillAll (Colours::orange.withAlpha (0.2f));
            break;
        case 1:
            g.fillAll (Colours::red.withAlpha (0.2f));
        default:
            break;
    }
    drawAppComp (g);
    
    
    double diameter = getWidth() - 2.0 * (Global::pzAxisMargin + Global::margin);
    
    AffineTransform transform;
    float zoomfactor = zoomed ? scaling : 1.0;
//    transform = transform.scaled ((zoomed ? 1.0 : scaling), (zoomed ? 1.0 : scaling), 150, 150);
    transform = transform.scaled (zoomfactor, zoomfactor, 150, 150);

    g.addTransform (transform);

    // draw unit circle
    g.setColour (Colours::grey);
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
    
    g.drawText ("Re", getWidth() - Global::margin - 0.5 * Global::pzAxisMargin - 10, getHeight() * 0.5 + 3, 20, 20, Justification::centred);
    g.drawText ("Im", getWidth() * 0.5 + 3, Global::margin + 0.5 * Global::pzAxisMargin - 5, 20, 20, Justification::centred);
    
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
    zoomButton->setBounds (getWidth() - Global::margin - 100, Global::margin, 100, 20);
}

void PoleZeroPlot::calculate()
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
    realZeroVector.resize (Global::numCoeffs * 0.5, 0);
    
    imaginaryZeroVector.clear();
    imaginaryZeroVector.resize (Global::numCoeffs * 0.5, 0);
    
    xCoeffs.clear();
    xCoeffs.resize (Global::numCoeffs * 0.5, 0);
    
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
    yCoeffs.clear();
    yCoeffs.resize (Global::numCoeffs * 0.5, 0);
    yCoeffs[highestYOrder] = 1;
    
    for (int i = 0; i < highestYOrder; ++i)
    {
        // also flip the sign of the coefficients in the case of y
        yCoeffs[highestYOrder - i - 1] = -coefficients[i + 1 + Global::numCoeffs * 0.5];
    }
    
    Polynomial yPolynomial (&yCoeffs[0], highestYOrder);
    int numberOfYRoots;
    yPolynomial.FindRoots (&realPoleVector[0],
                           &imaginaryPoleVector[0],
                           &numberOfYRoots);
    
    // for background colour drawing
    unstable = -1;
    for (int i = 0; i < Global::numCoeffs * 0.5; ++i)
    {
        // because we're comparing floats with integers, this is how we add a range of 0.0001;
        float poleMagnitude = round((sqrt (realPoleVector[i] * realPoleVector[i] + imaginaryPoleVector[i] * imaginaryPoleVector[i]) * 10000.0)) / 10000.0;
        if (poleMagnitude == 1)
            unstable = 0;
        if (poleMagnitude > 1)
        {
            unstable = 1;
            break;
        }
    }
    
    float maxDist = 1;
    
    for (int i = 0; i < Global::numCoeffs * 0.5; ++i)
    {
        if (abs (realZeroVector[i]) > maxDist)
            maxDist = abs (realZeroVector[i]);
        if (abs (imaginaryZeroVector[i]) > maxDist)
            maxDist = abs (imaginaryZeroVector[i]);
        
        if (abs (realPoleVector[i]) > maxDist)
            maxDist = abs (realPoleVector[i]);
        if (abs (imaginaryPoleVector[i]) > maxDist)
            maxDist = abs (imaginaryPoleVector[i]);

    }
    
    scaling = 1.0 / maxDist;
    if (maxDist > 1)
        zoomButton->setVisible (true);
    else
    {
//        if (zoomed)
//            buttonClicked (zoomButton.get());
        zoomButton->setVisible (false);
    }
}

void PoleZeroPlot::buttonClicked (Button* button)
{
    zoomed = !zoomed;
    zoomButton->setButtonText (zoomed ? "Zoom in" : "Zoom out");
    repaint();
}
