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
DiagramComponent::DiagramComponent (ComponentType type, ArrowType arrowType) : type (type), arrowType (arrowType)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    switch (type)
    {
        case arrow:
        {
            switch (arrowType)
            {
                case hor:
                    height = Global::arrowHeight;
                    break;
                case vert:
                    height = Global::vertArrowLength;
                    break;
                case cor:
                    height = 2.0 * Global::vertArrowLength + 0.5 * Global::arrowHeight;
                    break;
                case diag:
                    height = Global::bdCompDim; // should give actual height here as it is variable
                    break;
            }
            break;
        }
        case inOutput:
            height = 25.0f;
            break;
        case equation:
            height = 25.0f;
            break;
        case delay:
            height = Global::bdCompDim;
            data = 1;
            break;
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
            switch (arrowType)
            {
                case hor:
                {
                    Line<float> line (0.0, getHeight() * 0.5, getWidth(), getHeight() * 0.5);
                    g.drawArrow (line, 1, Global::arrowHeight, Global::arrowHeight);
                    break;
                }
                case vert:
                {
                    Line<float> line (getWidth() * 0.5, 0, getWidth() * 0.5, getHeight());
                    g.drawArrow (line, 1, Global::arrowHeight, Global::arrowHeight);
                    break;
                }
                case cor:
                {
                    Line<float> line (getWidth() * 0.5, 0.0, getWidth() * 0.5, getHeight() * 0.5 - Global::arrowHeight * 0.5 - 1);
                    Line<float> arrow (getWidth() * 0.5, getHeight() * 0.5 - Global::arrowHeight * 0.5 - 1, isACoeff ? 0.0 : getWidth(), getHeight() * 0.5 - Global::arrowHeight * 0.5 - 1);
                    g.drawLine (line, 1);
                    if (data < -0.5) // then there is no gain value
                        g.drawLine (arrow, 1);
                    else
                        g.drawArrow (arrow, 1, Global::arrowHeight, Global::arrowHeight);
                    break;
                }
                case diag:
                {
                    Line<float> line (isACoeff ? getWidth() : 0.0, getHeight() - 1, getWidth() * 0.5, getHeight() - 1);
                    Line<float> arrow (getWidth() * 0.5, getHeight() - 1, isACoeff ? Global::arrowHeight : (getWidth() - Global::arrowHeight), 0.0);
                    g.drawLine (line, 1);
                    g.drawArrow (arrow, 1, Global::arrowHeight, Global::arrowHeight);
                    break;
                }
            }
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
            g.drawText ("z", getWidth() * 0.30 - equationFont.getStringWidthFloat ("z") * 0.5, getHeight() * 0.6 - 12.5 , equationFont.getStringWidthFloat ("z"), 25.0, Justification::centred);
            g.setFont (equationFont.withHeight (16.0f));
            g.drawText ("-" + String (data), getWidth() * 0.35, getHeight() * 0.45 - 16.0 * 0.5, equationFont.getStringWidthFloat ("-" + String(data)), 16.0, Justification::centred);
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
            
            if (isACoeff)
            {
                AffineTransform transform;
                transform = transform.rotated(double_Pi, getWidth() * 0.5, getHeight() * 0.5);
                g.addTransform (transform);
            }
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

ArrowType DiagramComponent::getArrowType()
{
    if (type != arrow)
    {
        std::cout << "Component is not an arrow" << std::endl;
        return arrowType;
    }
    return arrowType;
}

void DiagramComponent::setArrowType (ArrowType arrType)
{
    if (type != arrow)
    {
        std::cout << "Component is not an arrow" << std::endl;
        return;
    }
    arrowType = arrType;
}
