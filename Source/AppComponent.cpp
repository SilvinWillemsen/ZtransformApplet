/*
  ==============================================================================

    AppComponent.cpp
    Created: 21 Jul 2020 10:53:39am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AppComponent.h"

//==============================================================================
AppComponent::AppComponent (String title, bool isEquation, bool isBlockDiagram) : title (title), isEquation (isEquation), isBlockDiagram (isBlockDiagram)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    equationFont.setHeight (25.0f);
    coefficients.resize (Global::numCoeffs);
    if (!isEquation)
        data.resize (Global::fftOrder);
    
    
}

AppComponent::~AppComponent()
{
}

void AppComponent::paint (Graphics& g)
{
    AffineTransform transform;
    transform = transform.scale (scale, scale);
    setTransform (transform);
}

void AppComponent::drawAppComp (Graphics& g)
{
    if (!isBlockDiagram)
    {
        g.setColour (Colours::black);
        g.drawRect (getLocalBounds());
        AffineTransform transform;
        transform = transform.scale (scale, scale);
        g.addTransform (transform);

        g.setFont (textFont);
        g.setColour (Colour (Global::textColour));
        g.drawText (title, Global::margin, Global::margin, getWidth() - Global::margin * 2.0, 25, Justification::centredLeft);
        g.setFont (equationFont);
    }
}

void AppComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
}

void AppComponent::setCoefficients (std::vector<double>& coefficientsToSet)
{
    coefficients = coefficientsToSet;
}

void AppComponent::refresh()
{
    calculate();
    repaint();
}
