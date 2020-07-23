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
AppComponent::AppComponent (String title, bool isEquation) : title (title), isEquation (isEquation)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    coefficients.resize (Global::numCoeffs);
    if (!isEquation)
        data.resize (Global::fftOrder);
    
    
}

AppComponent::~AppComponent()
{
}

void AppComponent::paint (Graphics& g)
{
}

void AppComponent::drawTitle (Graphics& g)
{
    g.setFont (textFont);
    g.setColour (Colour (Global::textColour));
    g.drawText (title, Global::margin, Global::margin, getWidth() * 0.5, 25, Justification::centredLeft);
    g.setFont (equationFont);
}

void AppComponent::drawOutline (Graphics& g)
{
    g.setColour (Colours::black);
    g.drawRect (getLocalBounds());
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
