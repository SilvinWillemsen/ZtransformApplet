#pragma once

#include <JuceHeader.h>
#include "Global.h"
#include "AppComponent.h"
#include <complex>
#include <math.h>

//==============================================================================
/*
 */
class PhaseResponse  : public AppComponent, public Button::Listener
{
public:
    PhaseResponse (double fs);
    ~PhaseResponse() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void calculate() override;
    Path generateResponsePath();
    void linearGainToPhase();
    
    void buttonClicked (Button* button) override;
    
private:
    std::vector<double> phaseData;
    std::vector<double> gridLineCoords;
    
    bool logPlot = false;
    double logBase = 1000;
    double fs;
    int zeroDbHeight = 0;
    
    double plotYStart = 40 + Global::margin;
    bool phaseIsNan = false;
    std::unique_ptr<TextButton> logPlotButton;
    
    std::unique_ptr<Label> phaseLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaseResponse)
};
