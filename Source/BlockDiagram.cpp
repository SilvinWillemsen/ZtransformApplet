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

    // as this component scales fully, we're drawing this separately
//    drawTitle (g);
//    drawOutline (g);
}

void BlockDiagram::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    float topLoc = 60;
    float centerLoc = getWidth() * 0.5;
    
    float curX = Global::margin;
    float curY = topLoc;
    float compWidth;
    
    bool justOneArrow = false;
    
//    bool arrowLengthFlag = false;
    bool topRow = true;
    int arrowCounter = 0;
    
    for (auto coeffComp : coefficientComps)
        coeffComp->setVisible (false);
    
    bool changeX = true;
    int incrementX = 1; // drawing from left to right (1) or right to left (-1)
    
    int curCoeffIdx = 0;
    drawingX = true;
    
    bool noGainFlag = false;
    
    // reset visibility and "a-coefficient-ness"
    for (auto comp : components)
    {
        if (comp->getType() == arrow && comp->getArrowType() == cor)
            comp->setData (0.0);
        comp->setVisible (false);
        comp->setACoeff (false);
    }
    
    int numXDelaysDrawn = 0;
    int numYDelaysDrawn = 0;
    
    this->setPaintingIsUnclipped (true);
    for (auto comp : components)
    {
        if (!drawingX)
            comp->setACoeff (true);
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
                        compWidth = 2.0 / 3.0 * getWidth();
                        justOneArrow = true;
                    }
                    else if (hasGain() && !hasDelays())
                    {
                        if (arrowCounter == 2)
                            continue;
                        compWidth = 1.0 / 3.0 * getWidth() - Global::gainWidth * 0.5;
                    }
                    else if (hasGain() && hasDelays())
                    {
                        
                        switch (arrowCounter)
                        {
                            case 1:
                                compWidth = Global::bdCompDim - 5;
                                break;
                            case 2:
                                compWidth = 1.0 / 3.0 * getWidth() - (Global::bdCompDim - 5) - Global::gainWidth - Global::bdCompDim * 0.5;
                                break;
                            case 3:
                                compWidth = 1.0 / 3.0 * getWidth() - Global::bdCompDim * 0.5;
                                break;
                        }
                        
//                        if (arrowCounter == 3)
//                            arrowLengthFlag = true;
//
//                        if (!arrowLengthFlag)
//                        {
//                            compWidth = 50 - Global::gainWidth;
//                        } else {
//                            compWidth = 100 - Global::bdCompDim * 0.5;
//                        }
                    } else {
                        if (arrowCounter == 2)
                            continue;
                        compWidth = 1.0 / 3.0 * getWidth() - Global::bdCompDim * 0.5;
                    }
                } else { // all others
                    switch (comp->getArrowType())
                    {
                        case hor:
                        {
                            std::cout << "Shouldn't be horizontal arrow" << std::endl;
                            break;
                        }
                        case vert:
                        {
                            curX = drawingX ? Global::bdCompDim + 5 : 2.0 / 3.0 * getWidth() + 15; // set curX to be either right after x[n] or right before y[n]
                            bool drawingXPrev = drawingX;
                            if (!checkForNextCoefficient (curCoeffIdx)) // if there are no more coefficients
                            {
                                int delaysFit = 4;
                                float normalHeight = Global::bdCompDim * 0.5 + (2.0 * Global::vertArrowLength + Global::bdCompDim) * delaysFit + Global::gainHeight;
                                float curHeight = normalHeight + std::max(0, std::max (numXDelaysDrawn, numYDelaysDrawn) - delaysFit) * (2.0 * Global::vertArrowLength + Global::bdCompDim);
                                scaling = normalHeight / curHeight;
                                AffineTransform transform;
                                transform = transform.scale (scaling, scaling, getX() + 0.5 * getWidth(), getY() + (topLoc - 0.5 * Global::bdCompDim));
                                setTransform (transform);
//                                this->setBounds (getBounds().withHeight (getHeight() * 0.5));
                                return;
                            }
                            else
                            {
                                if (drawingX != drawingXPrev) // if drawingX changed, we're now drawing the feedback
                                {
                                    incrementX = -1;
                                    curX = 2.0 / 3.0 * getWidth() + 15;
                                    curY = topLoc;
                                }
                                curY += Global::vertArrowLength * 0.5;
                                if (drawingX)
                                    ++numXDelaysDrawn;
                                else
                                    ++numYDelaysDrawn;
                            }
                            compWidth = Global::bdCompDim;
                            changeX = false;
                            break;
                        }
                        case cor:
                        {
                            if (coefficients[curCoeffIdx] == 1) // set it to a corner line without gain
                            {
                                compWidth =  Global::bdCompDim + 2.0 * Global::gainWidth;
                                curX -= Global::gainWidth;
                                noGainFlag = true;
                                comp->setData (-1.0);
                            } else {
                                compWidth = Global::bdCompDim;
                            }
                            curY += Global::vertArrowLength + 0.5 * Global::bdCompDim;
                            changeX = true;
                            break;
                        }
                        case diag:
                        {
                            if (!drawingX)
                                if (coefficients[curCoeffIdx] != 1)
                                    curX -= (1.0 / 3.0 * getWidth() - (Global::bdCompDim - 5) - Global::gainWidth - Global::gainWidth + 3);
                                else
                                    curX += (1.0 / 3.0 * getWidth() - (Global::bdCompDim - 5) - 3.0 * Global::gainWidth - 7);
                            else
                                curX -= 1;
                            compWidth = 1.0 / 3.0 * getWidth() - (Global::bdCompDim - 5) - Global::gainWidth;
                            break;
                        }
                    }
                }
                break;
            }
            case inOutput:
            {
                compWidth = equationFont.getStringWidthFloat (comp->getData() < 0.5 ? "x[n]" : "y[n]") + 10;
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
                comp->setData (delayVal);
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
                if (noGainFlag)
                {
                    noGainFlag = false;
                    continue;
                }
                if (!drawingX)
                    curX += Global::gainWidth;
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
            comp->setBounds(curX, topLoc + Global::bdCompDim * 0.5, compWidth + Global::arrowHeight, curY - topLoc - Global::bdCompDim * 0.5 + 1);
        else if (comp ->getType() == arrow && comp->getArrowType() == cor) // we want the bottom half of the arrow to stick out a bit
            comp->setBounds (curX, curY - comp->getCompHeight() * 0.5 + Global::arrowHeight * 0.5, compWidth, comp->getCompHeight() + Global::arrowHeight * 0.5);
        else
            comp->setBounds (curX, curY - comp->getCompHeight() * 0.5, compWidth, comp->getCompHeight());
        
        // add gain value to the gain arrow
        if (comp->getType() == gain && coefficients[curCoeffIdx] != 1)
        {
            coefficientComps[curCoeffIdx]->setBounds (curX + 0.5 * compWidth - equationFont.getStringWidthFloat (String (coefficients[curCoeffIdx])) * 0.5, curY - comp->getCompHeight() * 0.5 - Global::gainFont * 0.75 , equationFont.getStringWidthFloat (String (coefficients[curCoeffIdx])), Global::gainFont);
            addAndMakeVisible (coefficientComps[curCoeffIdx]);
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
    if (coefficients[0] != 1)
        return true;
    return false;
}

bool BlockDiagram::checkForNextCoefficient (int& idx)
{
    delayVal = 1;
    for (int i = idx + 1; i < Global::numCoeffs; ++i)
    {
        if (i > Global::numCoeffs * 0.5 && drawingX)
        {
            drawingX = false;
            delayVal = 1;
        }
        if (i != Global::numCoeffs * 0.5 && coefficients[i] != 0)
        {
            idx = i;
            return true;
        }
        ++delayVal;
    }
    
    return false;
}
