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

    // initialise feedforward and feedback components
    for (int i = 0; i < Global::numCoeffs - 2; ++i) // a0 and b0 are already included in the top row
    {
        components.add (new DiagramComponent (arrow, vert));
        components.add (new DiagramComponent (delay));
        components.add (new DiagramComponent (arrow, cor));
        components.add (new DiagramComponent (gain));
        components.add (new DiagramComponent (arrow, diag));
    }
    
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
    
    bool justOneArrow = false;
    
    bool arrowLengthFlag = false;
    bool topRow = true;
    int arrowCounter = 0;
    
    for (auto coeffComp : coefficientComps)
        coeffComp->setVisible (false);
    
    bool changeX = true;
    int incrementX = 1;
    float offset = 0; // if vertical arrow
    
    int curRow = 0;
    for (auto comp : components)
    {
        comp->setVisible (false);
        switch (comp->getType())
        {
            case arrow:
            {
                if (topRow) // only for toprow
                {
                    ++arrowCounter;
                    if (justOneArrow)
                        continue;
                    if (!hasDelays() && !hasGain())
                    {
                        compWidth = 200;
                        justOneArrow = true;
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
                } else { // all other rows
                    switch (comp->getArrowType())
                    {
                        case hor:
                        {
                            std::cout << "Shouldn't be horizontal arrow" << std::endl;
                            break;
                        }
                        case vert:
                        {
                            // new row!
                            ++curRow; // only add if y coeffs are also checked

                            curX = Global::bdCompDim; // set curX to be either right after x[n] or right before y[n]
                            if (!checkForNextCoefficient (curRow)) // if there are no more coefficients
                                return;
                            else
                            {
                                curY += Global::vertArrowLength * 0.5;
                            }
                            compWidth = Global::bdCompDim;
                            changeX = false;
                            break;
                        }
                        case cor:
                        {
                            compWidth = Global::bdCompDim;
                            curY += Global::vertArrowLength + 0.5 * Global::bdCompDim;
                            changeX = true;
                            break;
                        }
                        case diag:
                        {
                            compWidth = Global::bdCompDim;
                            break;
                        }
                    }
                }
                break;
            }
            case inOutput:
            {
                compWidth = equationFont.getStringWidthFloat (comp->getData() < 0.5 ? "x[n]" : "y[n]") + 3;
                if (comp->getData() > 0.5)
                {
                    topRow = false;
                }
                break;
            }
            case equation:
            {
                compWidth = equationFont.getStringWidthFloat (String (comp->getData()));
                break;
            }
            case delay:
            {
                // changeX = false; // should already be the case
                curY += 0.5 * (Global::vertArrowLength + Global::bdCompDim);
                compWidth = comp->getCompHeight();
                break;
            }
            case add:
            {
                if (!hasDelays())
                    continue;
                compWidth = comp->getCompHeight();
                break;
            }
            case gain:
            {
                if (topRow && !hasGain())
                    continue;
                compWidth = Global::gainWidth;
                break;
            }
            default:
                break;
        }
        comp->setVisible (true);
//        if (comp->getType() == arrow && comp->getArrowType() == vert)
//            offset = (Global::bdCompDim - Global::arrowHeight) * 0.5;
        if (comp->getType() == arrow && comp->getArrowType() == diag)
            comp->setBounds(curX, topLoc + Global::bdCompDim * 0.5, compWidth + Global::arrowHeight, curY - topLoc - Global::bdCompDim * 0.5);
        else if (comp ->getType() == arrow && comp->getArrowType() == cor) // we want the bottom half of the arrow to stick out a bit
            comp->setBounds (curX, curY - comp->getCompHeight() * 0.5 + Global::arrowHeight * 0.5, compWidth, comp->getCompHeight() + Global::arrowHeight * 0.5);
        else
            comp->setBounds (curX, curY - comp->getCompHeight() * 0.5, compWidth, comp->getCompHeight());
        
        offset = 0;
        // add gain value to the gain arrow
        if (comp->getType() == gain)
        {
            coefficientComps[Global::numCoeffs * 0.5]->setBounds (curX + 0.5 * compWidth - equationFont.getStringWidthFloat (String (coefficients[Global::numCoeffs * 0.5])) * 0.5, curY - comp->getCompHeight() * 0.5 - Global::gainFont * 0.75 , equationFont.getStringWidthFloat (String (coefficients[Global::numCoeffs * 0.5])), Global::gainFont);
            addAndMakeVisible (coefficientComps[Global::numCoeffs * 0.5]);
        }
        curX += changeX ? incrementX * compWidth : 0;
    }
}

void BlockDiagram::calculate()
{
    numXGains = 0;
    numYGains = 0;
    for (int i = 0; i < Global::numCoeffs; ++i)
    {
        coefficientComps[i]->setData (coefficients[i]);
        if (coefficients[i] != 0)
        {
            if (i > 0 && i < Global::numCoeffs * 0.5) // has a ygain
                ++numYGains;
            else if (i > Global::numCoeffs * 0.5)
                ++numXGains;
        }
    }
    std::cout << numXGains << " " << numYGains << std::endl;
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

bool BlockDiagram::checkForNextCoefficient (int& idx)
{
    for (int i = idx; i < Global::numCoeffs; ++i)
    {
        if (coefficients[i] != 0)
            return true;
    }
    
    return false;
}
