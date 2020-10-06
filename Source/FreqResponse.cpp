/*
  ==============================================================================

    FreqResponse.cpp
    Created: 22 Jul 2020 10:25:11am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FreqResponse.h"

//==============================================================================
FreqResponse::FreqResponse (double fs) : AppComponent ("Frequency Response", false), fs (fs)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    dBData.resize (Global::fftOrder);
    
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
    
    //// Labels ////
    dBLabel = std::make_unique<Label> ("dB", "dB");
    dBLabel->setColour(Label::textColourId, Colours::black);
    dBLabel->setColour(Label::backgroundColourId, Colours::white.withAlpha(0.0f));
    dBLabel->setJustificationType(Justification::centred);
    addAndMakeVisible (dBLabel.get());
    
    freqLabel = std::make_unique<Label> ("Freq", "Freq (Hz)");
    freqLabel->setColour(Label::textColourId, Colours::black);
    freqLabel->setColour(Label::backgroundColourId, Colours::white.withAlpha(0.0f));
    freqLabel->setJustificationType(Justification::centred);
    addAndMakeVisible (freqLabel.get());
    
}

FreqResponse::~FreqResponse()
{
}

void FreqResponse::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    drawAppComp (g);

    if (round (highestDB * 10000) / 10000.0 == round (lowestDB * 10000) / 10000.0)
    {
        if (round (highestDB * 10000) / 10000.0 == 0)
        {
            zeroDbHeight = (getHeight() - Global::axisMargin - plotYStart) * 0.5 + plotYStart;
            visualScaling = 1;
        }
        else if (highestDB > 0)
        {
            zeroDbHeight = plotYStart + plotHeight;
            visualScaling  = plotHeight / highestDB;
        }
        else
        {
            zeroDbHeight = plotYStart;
            visualScaling  = plotHeight / -lowestDB;
        }
    }
    else
    {
        visualScaling = plotHeight / (highestDB - lowestDB);
        zeroDbHeight = getHeight() - Global::axisMargin - Global::margin - (0 - lowestDB) * plotHeight / (highestDB - lowestDB);
    }
    
    //// Draw zero DB line ////
    if (lowestDB < 0 && highestDB >= 0)
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
    g.setFont (equationFont.withHeight (14.0f));

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
    
    //// Draw y-axis labels ////
    g.setColour (Colours::black);
    int highValue10 = round (highestDB * 10);
    int lowValue10 = round (lowestDB * 10);

    if (round (highestDB * 10000) / 10000.0 == round (lowestDB * 10000) / 10000.0)
    {
       if (round (highestDB * 10000) / 10000.0 == 0)
       {
           g.drawText ("0",
                       0.0f,
                       zeroDbHeight - equationFont.getHeight() * 0.5,
                       Global::axisMargin + Global::margin * 0.5,
                       equationFont.getHeight(), Justification::centredRight, false);
       }
       else if (highestDB > 0)  
       {
           g.drawText (String (highValue10 / 10.0),
                       0.0f,
                       plotYStart - equationFont.getHeight() * 0.5,
                       Global::axisMargin + Global::margin * 0.5,
                       equationFont.getHeight(), Justification::centredRight, false);
           
           g.drawText (String ("0"),
                       0.0f,
                       plotYStart + plotHeight - equationFont.getHeight() * 0.5,
                       Global::axisMargin + Global::margin * 0.5,
                       equationFont.getHeight(), Justification::centredRight, false);

       }
       else
       {
           g.drawText ("0",
                       0.0f,
                       plotYStart - equationFont.getHeight() * 0.5,
                       Global::axisMargin + Global::margin * 0.5,
                       equationFont.getHeight(), Justification::centredRight, false);
           
           g.drawText (String (lowValue10 / 10.0),
                       0.0f,
                       plotYStart + plotHeight - equationFont.getHeight() * 0.5,
                       Global::axisMargin + Global::margin * 0.5,
                       equationFont.getHeight(), Justification::centredRight, false);
           
       }
    } else {
    
        g.drawText (String (highValue10 / 10.0),
                    0.0f,
                    plotYStart - equationFont.getHeight() * 0.5,
                    Global::axisMargin + Global::margin * 0.5,
                    equationFont.getHeight(), Justification::centredRight, false);
        
        g.drawText (String (round (lowestDB * 10) / 10.0),
                    0.0f,
                    plotYStart + plotHeight - equationFont.getHeight() * 0.5,
                    Global::axisMargin + Global::margin * 0.5,
                    equationFont.getHeight(), Justification::centredRight, false);
    }
    
    //// Plot ////
    g.setColour (gainAbove0 ? Colours::red : Colours::black);
    g.strokePath (generateResponsePath(), PathStrokeType(2.0f));
    
    //// Draw axes ////
    g.setColour (Colours::black);
    g.drawLine (Global::margin + Global::axisMargin,
                plotYStart,
                Global::margin + Global::axisMargin,
                getHeight() - Global::axisMargin - Global::margin);
    
    g.drawLine (Global::margin + Global::axisMargin,
                getHeight() - Global::axisMargin - Global::margin,
                getWidth(),
                getHeight() - Global::axisMargin - Global::margin);

}

void FreqResponse::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
//    zeroDbHeight = (getHeight() - Global::axisMargin - plotYStart) * 0.5 + plotYStart;
    
    logPlotButton->setBounds(getWidth() - 100 - Global::margin, Global::margin, 100, 25);
    plotHeight = (getHeight() - Global::axisMargin - Global::margin - plotYStart);
    
    AffineTransform transformGain;
    transformGain = transformGain.rotated (-0.5 * double_Pi, Global::axisMargin * 0.5, plotYStart + 0.5 * plotHeight);
//    transformGain = transformGain.translated (-Global::axisMargin * 0.72, 0);
    dBLabel->setTransform (transformGain);
    
    dBLabel->setBounds (0, plotYStart, Global::axisMargin, plotHeight);
    freqLabel->setBounds (Global::axisMargin, plotYStart + plotHeight + Global::axisMargin * 0.6, getWidth() - Global::axisMargin, Global::axisMargin * 0.5);
}

Path FreqResponse::generateResponsePath()
{
    Path response;

    //response.startNewSubPath(0, -dBData[0] * visualScaling + zeroDbHeight);
    response.startNewSubPath (Global::margin + Global::axisMargin, -dBData[0] * visualScaling + zeroDbHeight); // draw RT instead of filter magnitude
    auto spacing = (getWidth() - Global::margin - Global::axisMargin) / static_cast<double> (dBData.size());
    double x = Global::margin + Global::axisMargin;
    float newY;
    for (int y = 0; y < data.size(); y++)
    {
        newY = -dBData[y] * visualScaling + zeroDbHeight;
        response.lineTo(x, newY);
        x += spacing;
    }
    response.lineTo (getWidth(), newY);
    
    return response;
}

void FreqResponse::calculate()
{
    std::complex<double> i (0.0, 1.0);
    std::complex<double> omega (0.0, 0.0);
    highestGain = 0;
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
        highestGain = std::max (abs (data[k-1]), highestGain);
    }
    
    linearGainToDB();
}

void FreqResponse::linearGainToDB()
{
    gainAbove0 = false;
    highestDB = -60.0;
    lowestDB = 100.0;
    for (int i = 0; i < Global::fftOrder; ++i)
    {
        dBData[i] = Global::limit (20.0 * log10(abs(data[i])), -60.0, 1000.0);
        highestDB = std::max (dBData[i], highestDB);
        lowestDB = std::min (dBData[i], lowestDB);

        if (dBData[i] > 0)
            gainAbove0 = true;
    }
}

void FreqResponse::buttonClicked (Button* button)
{
    logPlot = !logPlot;
    logPlotButton->setButtonText (logPlot ? "Lin. plot" : "Log. plot");
    refresh();
}
