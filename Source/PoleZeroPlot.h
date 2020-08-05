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
#include "math.h"
//==============================================================================
/*
*/
class PoleZeroPlot  : public AppComponent, public Button::Listener
{
public:
    PoleZeroPlot();
    ~PoleZeroPlot() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void calculate() override;
    
    void buttonClicked (Button* button) override;
    
    bool isStable() { return unstable == -1; };
    
private:
    std::vector<double> realZeroVector { Global::numCoeffs * 0.5 - 1, 0 };
    std::vector<double> imaginaryZeroVector { Global::numCoeffs * 0.5 - 1, 0 };
    
    std::vector<double> realPoleVector { Global::numCoeffs * 0.5 - 1, 0 };
    std::vector<double> imaginaryPoleVector { Global::numCoeffs * 0.5 - 1, 0 };
    
    std::vector<double> xCoeffs;
    std::vector<double> yCoeffs;
    int highestXOrder = 0;
    int highestYOrder = 0;

    int unstable = -1; // -1 : stable, 0 : on the edge, 1 : unstable
    
    bool zoomed = false;
    std::unique_ptr<TextButton> zoomButton;
    
    float scaling = 1.0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PoleZeroPlot)
};
