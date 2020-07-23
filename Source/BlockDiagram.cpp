/*
  ==============================================================================

    BlockDiagram.cpp
    Created: 23 Jul 2020 1:35:07pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BlockDiagram.h"

//==============================================================================
BlockDiagram::BlockDiagram() : AppComponent ("Block Diagram", false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    components.add (new DiagramComponent (inOutput));
    components.add (new DiagramComponent (arrow));
    components.add (new DiagramComponent (gain));
    components.add (new DiagramComponent (arrow));
    components.add (new DiagramComponent (add));
    components.add (new DiagramComponent (arrow));
    components.add (new DiagramComponent (inOutput));

    components[components.size() - 1]->setData (1);
    
    for (int i = 0; i < Global::numCoeffs; ++i)
    {
        coefficientComps.add (new DiagramComponent (equation));
        addAndMakeVisible (coefficientComps[i]);
        coefficientComps[i]->setData (coefficients[i]);
    }
    for (auto comp : components)
    {
        addAndMakeVisible (comp);
    }
}

BlockDiagram::~BlockDiagram()
{
}

void BlockDiagram::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    drawTitle (g);
    drawOutline (g);
}

void BlockDiagram::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    float topLoc = 80;
    float centerLoc = getWidth() * 0.5;
    
    // x[n]
    float curX = Global::margin;
    float curY = topLoc;
    float compWidth;
    
    bool hasNoDelays = false;
    
    bool arrowLengthFlag = false;
    int arrowCounter = 0;
    
    for (auto coeffComp : coefficientComps)
        coeffComp->setVisible (false);
    
    for (auto comp : components)
    {
        comp->setVisible (false);

        switch (comp->getType())
        {
            case arrow:
            {
                ++arrowCounter;
                if (hasNoDelays)
                    continue;
                if (!hasDelays() && !hasGain())
                {
                    compWidth = 200;
                    hasNoDelays = true;
                }
                else if (hasGain() && !hasDelays())
                {
                    if (arrowCounter == 2)
                        continue;
                    compWidth = 100 - Global::gainWidth * 0.5;
                }
                else if (hasGain() && hasDelays())
                {
                    if (arrowCounter == 3)
                        arrowLengthFlag = true;
                    
                    if (!arrowLengthFlag)
                    {
                        compWidth = 50 - Global::gainWidth;
                    } else {
                        compWidth = 100 - Global::bdCompDim * 0.5;
                    }
                } else {
                    if (arrowCounter == 2)
                        continue;
                    compWidth = 100 - Global::bdCompDim * 0.5;
                }
                break;
            }
            case inOutput:
            {
                compWidth = equationFont.getStringWidthFloat (comp->getData() < 0.5 ? "x[n]" : "y[n]") + 3;
                break;
            }
            case equation:
            {
                compWidth = equationFont.getStringWidthFloat (String (comp->getData()));
                break;
            }
            case delay:
            case add:
            {
                if (!hasDelays())
                    continue;
                compWidth = comp->getCompHeight();
                break;
            }
            case gain:
            {
                if (!hasGain())
                    continue;
                compWidth = Global::gainWidth;
                break;
            }
            default:
                break;
        }
        comp->setVisible (true);
        comp->setBounds (curX, curY - comp->getCompHeight() * 0.5, compWidth, comp->getCompHeight());
        if (comp->getType() == gain)
        {
            coefficientComps[Global::numCoeffs * 0.5]->setBounds (curX + 0.5 * compWidth - equationFont.getStringWidthFloat (String (coefficients[Global::numCoeffs * 0.5])) * 0.5, curY - comp->getCompHeight() * 0.5 - Global::gainFont * 0.75 , equationFont.getStringWidthFloat (String (coefficients[Global::numCoeffs * 0.5])), Global::gainFont);
            addAndMakeVisible (coefficientComps[Global::numCoeffs * 0.5]);
        }
        curX += compWidth;
    }
    
//    if (!hasDelays())
//    {
//        components[1]->setBounds (components[0]->getX() + components[0]->getWidth() + spacing, topLoc - Global::arrowHeight * 0.5, getWidth() - 2.0 * Global::margin -equationFont.getStringWidthFloat ("x[n]y[n]") - 2.0 * spacing, Global::arrowHeight);
//    }
//    else
//    {
//
//    }
//    // y[n]
//    components[1]->setBounds (components[2]->getX() + components[2]->getWidth() + spacing, topLoc - 25.0f * 0.5, equationFont.getStringWidthFloat (components[1]->getData() ? "y[n]" : "x[n]") + 3, 25.0f);


}

void BlockDiagram::calculate()
{
    for (int i = 0; i < Global::numCoeffs; ++i)
        coefficientComps[i]->setData (coefficients[i]);
    resized();
}

bool BlockDiagram::hasDelays()
{
    for (int i = 1; i < Global::numCoeffs; ++i)
    {
        if (i == Global::numCoeffs * 0.5)
            continue;
        
        if (coefficients[i] != 0)
            return true;
    }
    return false;
}

bool BlockDiagram::hasGain()
{
    if (coefficients[Global::numCoeffs * 0.5] != 1)
        return true;
    return false;
}
