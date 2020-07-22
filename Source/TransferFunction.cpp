/*
  ==============================================================================

    TransferFunction.cpp
    Created: 21 Jul 2020 2:08:56pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TransferFunction.h"

//==============================================================================
TransferFunction::TransferFunction() : AppComponent ("Transfer Function", true)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    xPowers.resize (Global::numCoeffs * 0.5);
    yPowers.resize (Global::numCoeffs * 0.5);
    xPowerLocs.resize (Global::numCoeffs * 0.5);
    yPowerLocs.resize (Global::numCoeffs * 0.5);

}

TransferFunction::~TransferFunction()
{
}

void TransferFunction::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.setFont (textFont);
    g.setColour (Colour (Global::textColour));
    g.drawText (getTitle(), 0, 0, getWidth() * 0.5, 25, Justification::centredLeft);
    g.setFont (equationFont);

    //reset all vectors
    
    xPowersAmount = 0;
    yPowersAmount = 0;
    
    xPowers.resize (Global::numCoeffs * 0.5, 0);
    yPowers.resize (Global::numCoeffs * 0.5, 0);
    
    xPowerLocs.resize (Global::numCoeffs * 0.5, 0);
    yPowerLocs.resize (Global::numCoeffs * 0.5, 0);

    if (!hasYcomponent)
    {
        g.drawText ("H(z) = ", 0, 25 + Global::margin, getWidth(), 25, Justification::centredLeft);
        removePowers (xEquation, true);
        
        g.drawText(xEquation, 65, 25 + Global::margin, getWidth(),  25, Justification::centredLeft);
        g.setFont (equationFont.withHeight(16.0f));
        for (int i = 0; i < xPowersAmount; ++i)
            g.drawText("-" + String (xPowers[i]), 65 + xPowerLocs[i], 25 + Global::margin, equationFont.getStringWidthFloat("-" + String (xPowers[i])), 16, Justification::centred);
        
    } else {
        
        removePowers (xEquation, true);
        removePowers (yEquation, false);
        
        float totStringWidth = std::max (equationFont.getStringWidthFloat (xEquation), equationFont.getStringWidthFloat (yEquation));

        float xPowersOffset = 0;
        float yPowersOffset = 0;

        if (equationFont.getStringWidthFloat (xEquation) > equationFont.getStringWidthFloat (yEquation))
            yPowersOffset = (equationFont.getStringWidthFloat (xEquation) + equationFont.getStringWidthFloat (yEquation)) * 0.5 - equationFont.getStringWidthFloat (yEquation);
        else
            xPowersOffset = (equationFont.getStringWidthFloat (xEquation) + equationFont.getStringWidthFloat (yEquation)) * 0.5 - equationFont.getStringWidthFloat (xEquation);
        
        g.drawText ("H(z) = ", 0, 35 + Global::margin, getWidth(), 25, Justification::centredLeft);
        g.drawText(xEquation, 65, 20 + Global::margin, totStringWidth, 25, Justification::centred);
        g.setColour (Colours::black);
        g.drawLine (63, 35 + Global::margin + 25 * 0.5, 67 + totStringWidth, 35 + Global::margin + 25 * 0.5);
        g.drawText(yEquation, 65, 50 + Global::margin, totStringWidth, 25, Justification::centred);
        
        g.setFont (equationFont.withHeight(16.0f));
        for (int i = 0; i < xPowersAmount; ++i)
            g.drawText("-" + String (xPowers[i]), 70 + xPowerLocs[i] + xPowersOffset, 20 + Global::margin, equationFont.getStringWidthFloat("-" + String (xPowers[i])), 16, Justification::centredRight);
        
        for (int i = 0; i < yPowersAmount; ++i)
            g.drawText("-" + String (yPowers[i]), 70 + yPowerLocs[i] + yPowersOffset, 50 + Global::margin, equationFont.getStringWidthFloat("-" + String (yPowers[i])), 16, Justification::centredRight);
    }

}

void TransferFunction::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void TransferFunction::generateEquation()
{
    xEquation = "";
    
    // x-component
    for (int i = Global::numCoeffs * 0.5; i < Global::numCoeffs; ++i)
    {
        if (data[i] == 0)
            continue;
        
        if (data[i] < 0)
            xEquation = xEquation.dropLastCharacters(2);
        
        if (i == Global::numCoeffs * 0.5 || data[i] != 1)
            xEquation += String(data[i]);
        
        if (i == Global::numCoeffs * 0.5)
            xEquation += " ";
        
        if (i != Global::numCoeffs * 0.5)
        {
            xEquation += "z^-";
            xEquation += String (i - Global::numCoeffs * 0.5);
            xEquation += " ";
        }
        
        xEquation += "+ ";
    }
    
    if (xEquation ==  "")
        xEquation = "0";
    else
        xEquation = xEquation.dropLastCharacters (2);
    
    // y-component
    yEquation = "1";
    hasYcomponent = true;
    
    for (int i = 1; i < Global::numCoeffs * 0.5; ++i)
    {
        if (data[i] == 0)
            continue;
        
        if (data[i] > 0)
        {
            yEquation += " - ";
        } else {
            yEquation += " + ";
        }
        
        if (abs(data[i]) != 1)
            yEquation += String (abs (data[i]));
        
        if (i != Global::numCoeffs * 0.5)
        {
            yEquation += "z^-";
            yEquation += String(i);
            yEquation += " ";
        }
        
    }
    
    if (yEquation == "1")
        hasYcomponent = false;
}

void TransferFunction::removePowers (String& equation, bool isX)
{
    if (!equation.contains("z"))
        return;

    bool flag = true;
    int idx = 0;
    
    std::vector<String> parts;
    String equationWithoutPowers = "";
    
    while (flag)
    {
        equationWithoutPowers += equation.substring (0, equation.indexOf("z") + 1);
        if (!equation.contains("^-"))
        {
            flag = false;
            break;
        }
        equation = equation.fromFirstOccurrenceOf ("^-", false, false);
        
        if (isX)
        {
            xPowers[idx] = equation.substring(0, 1).getIntValue();
            xPowerLocs[idx] = equationFont.getStringWidthFloat (equationWithoutPowers);
        } else {
            yPowers[idx] = equation.substring(0, 1).getIntValue();
            yPowerLocs[idx] = equationFont.getStringWidthFloat (equationWithoutPowers);

        }
        
        ++idx;
        equation = equation.substring (2, equation.length());
        equationWithoutPowers += "   ";

    }
    if (isX)
        xPowersAmount = idx;
    else
        yPowersAmount = idx;
    
    equation = equationWithoutPowers;
}
