/*
  ==============================================================================

    PhaseResponse.cpp
    Created: 5 Aug 2020 3:53:29pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PhaseResponse.h"

//==============================================================================
PhaseResponse::PhaseResponse (double fs) : AppComponent ("Phase Response", false), fs (fs)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    phaseData.resize (Global::fftOrder);
    
    gridLineCoords.resize (32);
    int m = 10;
    int n = 0;
    for (int i = 1; i <= gridLineCoords.size(); ++i)
    {
        ++n;
        double y = m * n;
        gridLineCoords[i-1] = log(y * (logBase - 1.0) / (fs * 0.5) + 1.0) / log(logBase);
        if (n % 10 == 0)
        {
            m *= 10;
            n = 0;
        }
        
    }
    
    logPlotButton = std::make_unique<TextButton> ();
    logPlotButton->setButtonText (logPlot ? "Lin. plot" : "Log. plot");
    logPlotButton->addListener (this);
    addAndMakeVisible (logPlotButton.get());
}

PhaseResponse::~PhaseResponse()
{
}

void PhaseResponse::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
     draws some placeholder text to get you started.
     
     You should replace everything in this method with your own
     drawing code..
     */
    drawAppComp (g);
    
    //// Draw zero DB line ////
    g.drawLine (Global::axisMargin + Global::margin, zeroDbHeight, getWidth(), zeroDbHeight, 1.0);
    
    double plotWidth = getWidth() - (Global::axisMargin + Global::margin);
    
    //// Draw gridlines ////
    
    g.setColour (Colours::lightgrey);
    
    if (logPlot)
    {
        for (int n = 0; n < gridLineCoords.size(); ++n)
        {
            g.drawLine (gridLineCoords[n] * plotWidth + Global::axisMargin + Global::margin,
                        plotYStart,
                        gridLineCoords[n] * plotWidth + Global::axisMargin + Global::margin,
                        getHeight() - Global::axisMargin - Global::margin, 1.0f);
        }
    } else {
        int numLines = floor (fs * 0.5 * 0.0005) - 1;
        double distanceBetweenLines = plotWidth / static_cast<double> (numLines + 1);
        
        for (int n = 1; n <= numLines; ++n)
        {
            g.drawLine (Global::axisMargin + Global::margin + n * distanceBetweenLines,
                        plotYStart,
                        Global::axisMargin + Global::margin + n * distanceBetweenLines,
                        getHeight() - Global::axisMargin - Global::margin, 1.0f);
        }
    }
    
    
    //// Draw x-axis labels ////
    g.setFont (equationFont.withHeight (16.0f));
    
    if (logPlot)
    {
        int j = 1;
        
        int lim = 0; // varibale for deciding how many lines are shown in the non-band-lines case
        
        for (int n = 0; n < gridLineCoords.size(); ++n)
        {
            g.drawText (String (pow(10, j) * ((n+1) % 10)),
                        gridLineCoords[n] * plotWidth + Global::axisMargin + Global::margin - 20,
                        getHeight() - Global::axisMargin - Global::margin,
                        40, 20, Justification::centred);
            
            if (n % 10 == lim)
            {
                n += (9 - lim);
                ++j;
            }
        }
    } else {
        int numLines = floor (fs * 0.5 * 0.0005) - 1;
        double distanceBetweenLines = plotWidth / static_cast<double> (numLines + 1);
        
        for (int n = 1; n <= numLines; ++n)
        {
            g.drawText (String (n * 2000),
                        Global::axisMargin + Global::margin + n * distanceBetweenLines - 20,
                        getHeight() - Global::axisMargin - Global::margin,
                        40, 20, Justification::centred);
            
        }
    }
    
    //// Plot ////
    g.setColour (Colours::black);
    g.strokePath (generateResponsePath(), PathStrokeType(2.0f));
    
    //// Draw axes ////
    g.drawLine (Global::margin + Global::axisMargin,
                plotYStart,
                Global::margin + Global::axisMargin,
                getHeight() - Global::axisMargin - Global::margin);
    
    g.drawLine (Global::margin + Global::axisMargin,
                getHeight() - Global::axisMargin - Global::margin,
                getWidth(),
                getHeight() - Global::axisMargin - Global::margin);
    
}

void PhaseResponse::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    zeroDbHeight = (getHeight() - Global::axisMargin - Global::margin - plotYStart) * 0.5 + plotYStart;
    
    logPlotButton->setBounds(getWidth() - 100 - Global::margin, Global::margin, 100, 25);
    
}

Path PhaseResponse::generateResponsePath()
{
    Path response;
    float visualScaling = (getHeight() - Global::axisMargin - Global::margin - plotYStart) / double_Pi;
    
    //response.startNewSubPath(0, -dBData[0] * visualScaling + zeroDbHeight);
    response.startNewSubPath (Global::margin + Global::axisMargin, (isnan(phaseData[0]) ? 0.0 : -phaseData[0] * visualScaling) + zeroDbHeight); // draw RT instead of filter magnitude
    auto spacing = (getWidth() - Global::margin - Global::axisMargin) / static_cast<double> (phaseData.size());
    double x = Global::margin + Global::axisMargin;
    float newY;
    for (int y = 0; y < data.size(); y++)
    {
        newY = -phaseData[y] * visualScaling + zeroDbHeight;
        if (isnan(newY))
            continue;
        response.lineTo(x, newY);
        x += spacing;
    }
    response.lineTo (getWidth(), isnan(newY) ? zeroDbHeight : newY);
    
    return response;
}

void PhaseResponse::calculate()
{
    phaseIsNan = false;
    std::complex<double> i (0.0, 1.0);
    std::complex<double> omega (0.0, 0.0);
    
    for (int k = 1; k <= Global::fftOrder; ++k)
    {
        double linearVal = k / static_cast<double>(Global::fftOrder);
        if (logPlot)
            omega.real (double_Pi * ((pow (logBase, linearVal) - 1.0) / (logBase - 1.0)));
        else
            omega.real (double_Pi * linearVal);
        
        
        std::complex<double> numerator (0.0, 0.0);
        std::complex<double> denominator (1.0, 0.0);
        
        for (int j = 0; j < Global::numCoeffs * 0.5; ++j)
        {
            if (coefficients[j] == 0)
                continue;
            numerator += coefficients[j] * exp(-i * omega * std::complex<double>(j));
            
        }
        for (int j = 1; j < Global::numCoeffs * 0.5; ++j)
        {
            if (coefficients[j + Global::numCoeffs * 0.5] == 0)
                continue;
            denominator -= coefficients[j + Global::numCoeffs * 0.5] * exp(-i * omega * std::complex<double>(j));
        }
        data[k-1] = numerator / denominator;
        if (data[k-1].real() == 0)
            phaseIsNan = true;
    }
    // if the real part of the data is 0, set all phase data to 0;
//    if (phaseIsNan)
//    {
//        phaseData.clear();
//        phaseData.resize (Global::fftOrder, 0.0);
//    } else {
    linearGainToPhase();
//    }
}

void PhaseResponse::buttonClicked (Button* button)
{
    logPlot = !logPlot;
    logPlotButton->setButtonText (logPlot ? "Lin. plot" : "Log. plot");
    refresh();
}

void PhaseResponse::linearGainToPhase()
{
    for (int i = 0; i < Global::fftOrder; ++i)
        phaseData[i] = atan (data[i].imag() / data[i].real());
}
