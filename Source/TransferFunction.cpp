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
    
    drawAppComp (g);

    //reset all vectors
    
    xPowersAmount = 0;
    yPowersAmount = 0;
    
    xPowers.resize (Global::numCoeffs * 0.5, 0);
    yPowers.resize (Global::numCoeffs * 0.5, 0);
    
    xPowerLocs.resize (Global::numCoeffs * 0.5, 0);
    yPowerLocs.resize (Global::numCoeffs * 0.5, 0);

    Font font = equationFont;
    String equation = "H(z) = ";
    if (!hasYcomponent)
    {
        equation += xEquation;
    } else {
        equation += (equationFont.getStringWidthFloat (xEquation) > equationFont.getStringWidthFloat (yEquation) ? xEquation : yEquation);
    }
    
    while (font.getStringWidthFloat (equation) > getWidth() - 2.0 * Global::margin)
        font = font.withHeight(font.getHeight() - 0.1);
    scaling = font.getHeight() / equationFont.getHeight();
    
    AffineTransform transform;
    transform = transform.scale (scaling, scaling, Global::margin, getHeight() * 0.5);
//    transform = transform.translated (Global::margin * (1.0 - scaling), (30 + Global::margin) * (1.0 - scaling));
    g.addTransform (transform);
    
    if (!hasYcomponent)
    {
        g.drawText ("H(z) = ", Global::margin, (getHeight() + Global::margin + 12.5) * 0.5 - 12.5, getWidth(), 25, Justification::centredLeft, false);
        removePowers (xEquation, xEquationWithoutPowers, true);
        
        // Draw main equation (without powers)
        
        g.drawText (xEquationWithoutPowers, Global::margin + 65, (getHeight() + Global::margin + 12.5) * 0.5 - 12.5, getWidth() * 1.0 / scaling,  25, Justification::centredLeft, false);
        
        // Draw powers
        g.setFont (equationFont.withHeight(16.0f));
        for (int i = 0; i < xPowersAmount; ++i)
            g.drawText("-" + String (xPowers[i]),
                       Global::margin + 65 + xPowerLocs[i],
                       (getHeight() + Global::margin + 12.5) * 0.5 - 12.5,
                       equationFont.getStringWidthFloat("-" + String (xPowers[i])),
                       16,
                       Justification::centred);
        
    } else {
        
        removePowers (xEquation, xEquationWithoutPowers, true);
        removePowers (yEquation, yEquationWithoutPowers, false);
        
        float totStringWidth = std::max (equationFont.getStringWidthFloat (xEquationWithoutPowers), equationFont.getStringWidthFloat (yEquationWithoutPowers));

        float xPowersOffset = 0;
        float yPowersOffset = 0;

        // Calculate offset for numerator or denominator if the other is bigger
        if (equationFont.getStringWidthFloat (xEquation) > equationFont.getStringWidthFloat (yEquationWithoutPowers))
            yPowersOffset = (equationFont.getStringWidthFloat (xEquationWithoutPowers) + equationFont.getStringWidthFloat (yEquationWithoutPowers)) * 0.5 - equationFont.getStringWidthFloat (yEquationWithoutPowers);
        else
            xPowersOffset = (equationFont.getStringWidthFloat (xEquationWithoutPowers) + equationFont.getStringWidthFloat (yEquationWithoutPowers)) * 0.5 - equationFont.getStringWidthFloat (xEquationWithoutPowers);
        
        // Draw main equation (without powers)
        g.drawText ("H(z) = ", Global::margin, (getHeight() + Global::margin + 12.5) * 0.5 - 12.5, getWidth(), 25, Justification::centredLeft, false);
        g.drawText(xEquationWithoutPowers, 65 + Global::margin, (getHeight() + Global::margin + 12.5) * 0.5 - 12.5 - 15, totStringWidth, 25, Justification::centred, false);
        g.setColour (Colours::black);
        g.drawLine (Global::margin + 65,
                    (getHeight() + Global::margin + 12.5) * 0.5,
                    80 + totStringWidth + Global::margin,
                    (getHeight() + Global::margin + 12.5) * 0.5);
        
        g.drawText(yEquationWithoutPowers, Global::margin + 65, (getHeight() + Global::margin + 12.5) * 0.5 - 12.5 + 15, totStringWidth, 25, Justification::centred, false);
        
        // Draw powers
        g.setFont (equationFont.withHeight(16.0f));
        for (int i = 0; i < xPowersAmount; ++i)
            g.drawText("-" + String (xPowers[i]),
                       Global::margin + 70 + xPowerLocs[i] + xPowersOffset,
                       (getHeight() + Global::margin + 12.5) * 0.5 - 12.5 - 15,
                       equationFont.getStringWidthFloat("-" + String (xPowers[i])),
                       16, Justification::centredRight);
        
        for (int i = 0; i < yPowersAmount; ++i)
            g.drawText("-" + String (yPowers[i]),
                       Global::margin + 70 + yPowerLocs[i] + yPowersOffset,
                       (getHeight() + Global::margin + 12.5) * 0.5 - 12.5 + 15,
                       equationFont.getStringWidthFloat("-" + String (yPowers[i])),
                       16, Justification::centredRight);
    }

}

void TransferFunction::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void TransferFunction::calculate()
{
    xEquation = "";
    
    // x-component
    for (int i = 0; i < Global::numCoeffs * 0.5; ++i)
    {
        if (coefficients[i] == 0)
            continue;
        
        // if it is not the first entry
        bool firstInArray = xEquation == "";
        if (!firstInArray)
        {
            if (coefficients[i] > 0)
                xEquation += " + ";
            else
                xEquation += " - ";
        }
        
        if (i == 0 || abs(coefficients[i]) != 1)
            xEquation += String (firstInArray ? coefficients[i] : abs (coefficients[i]));
        
        if (i != 0)
        {
            xEquation += "z^-";
            xEquation += String (i);
        }
        
    }
    
    if (xEquation ==  "")
        xEquation = "0";
//    else
//        xEquation = xEquation.dropLastCharacters (2);
    
    // y-component
    yEquation = "1";
    hasYcomponent = true;
    
    for (int i = Global::numCoeffs * 0.5 + 1; i < Global::numCoeffs; ++i)
    {
        if (coefficients[i] == 0)
            continue;
        
        // opposite sign for denominator
        if (coefficients[i] > 0)
            yEquation += " - ";
        else
            yEquation += " + ";
        
        if (abs(coefficients[i]) != 1)
            yEquation += String (abs (coefficients[i]));
        
//        if (i != Global::numCoeffs * 0.5)
//        {
            yEquation += "z^-";
            yEquation += String (i - Global::numCoeffs * 0.5);
//        }
        
    }
    
    if (yEquation == "1")
        hasYcomponent = false;
}

void TransferFunction::removePowers (String equation, String& equationWithoutPowers, bool isX)
{
    equationWithoutPowers = "";
    
    if (!equation.contains("z"))
    {
        equationWithoutPowers = equation;
        return;
    }

    bool flag = true;
    int idx = 0;
    
    std::vector<String> parts;
    
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
}
