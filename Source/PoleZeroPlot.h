/*
  ==============================================================================

    PoleZeroPlot.h
    Created: 22 Jul 2020 1:55:43pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Global.h"
#include "AppComponent.h"
#include "Polynomial.h"

//==============================================================================
/*
*/
class PoleZeroPlot  : public AppComponent
{
public:
    PoleZeroPlot();
    ~PoleZeroPlot() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void calculate() override;

    void findRoots();
    
    void invertYCoeffs();
private:
    std::vector<double> realZeroVector { Global::numCoeffs * 0.5 - 1, 0 };
    std::vector<double> imaginaryZeroVector { Global::numCoeffs * 0.5 - 1, 0 };
    
    std::vector<double> realPoleVector { Global::numCoeffs * 0.5 - 1, 0 };
    std::vector<double> imaginaryPoleVector { Global::numCoeffs * 0.5 - 1, 0 };

    int highestXOrder = 0;
    int highestYOrder = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PoleZeroPlot)
};
