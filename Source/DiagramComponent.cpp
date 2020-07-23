/*
  ==============================================================================

    DiagramComponent.cpp
    Created: 23 Jul 2020 2:04:26pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DiagramComponent.h"

//==============================================================================
DiagramComponent::DiagramComponent (ComponentType type) : type (type)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    switch (type)
    {
        case arrow:
            height = Global::arrowHeight;
            break;
        case inOutput:
            height = 25.0f;
            break;
        case equation:
            height = 25.0f;
            break;
        case delay:
        case add:
            height = Global::bdCompDim;
            break;
        case gain:
            height = Global::gainHeight;
            break;
        default:
            height = 0;
            break;
    }
}

DiagramComponent::~DiagramComponent()
{
}

void DiagramComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.setFont (equationFont);
    switch (type) {
        case arrow:
        {
            Line<float> line (0.0, getHeight() * 0.5, getWidth(), getHeight() * 0.5);
            g.drawArrow (line, 1, Global::arrowHeight, Global::arrowHeight);
            break;
        }
        case inOutput:
        {
            g.setFont (equationFont);
            g.drawText (data < 0.5 ? "x[n]" : "y[n]", getLocalBounds(), Justification::centred);
            break;
        }
        case equation:
        {
            g.setFont (equationFont.withHeight (Global::gainFont));
            g.drawText (String (data), getLocalBounds(), Justification::centred);
            break;
        }
        case delay:
        {
            g.drawRect (getLocalBounds(), 1);
            g.drawText ("z  ", getLocalBounds(), Justification::centred);
            g.setFont (equationFont.withHeight (16.0f));
            g.drawText ("-" + String (data), getLocalBounds(), Justification::centred);
            break;
        }
        case add:
        {
            g.drawEllipse (getLocalBounds().toFloat().reduced(1, 1), 1);
            g.drawLine(1, getHeight() * 0.5, getWidth() - 1, getHeight() * 0.5);
            g.drawLine(getWidth() * 0.5, 1, getWidth() * 0.5, getHeight() - 1);
            break;
        }
        case gain:
        {
            g.drawLine (1, 1, 1, getHeight() - 1);
            g.drawLine (1, 1, getWidth() - 1, getHeight() * 0.5);
            g.drawLine (1, getHeight() - 1, getWidth() - 1, getHeight() * 0.5);
            g.setColour (Colours::white);
            g.drawLine (2, getHeight() * 0.5, getWidth() - 2, getHeight() * 0.5);
            break;
        }
        default:
            break;
    }
}

void DiagramComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

