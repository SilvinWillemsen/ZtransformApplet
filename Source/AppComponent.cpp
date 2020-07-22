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
    if (isEquation)
        data.resize (Global::numCoeffs);
    else
        data.resize (100);
}

AppComponent::~AppComponent()
{
}

void AppComponent::paint (juce::Graphics& g)
{
}

void AppComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
}

void AppComponent::setData (std::vector<double>& dataToSet)
{
    data = dataToSet;
}

void AppComponent::refresh()
{
    if (isEquation)
        generateEquation();
    
    repaint();
}
