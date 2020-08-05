#pragma once

#include <JuceHeader.h>
#include "Global.h"
#include "AppComponent.h"
#include <complex>

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
    void linearGainToDB();
    
    void buttonClicked (Button* button) override;
    
    double getHighestGain() { return highestGain; };
    
private:
    
    bool gainAbove0;
    std::vector<double> dBData;
    std::vector<double> gridLineCoords;
    
    bool logPlot = false;
    double logBase = 1000;
    double fs;
    int zeroDbHeight = 0;
    
    double plotYStart = 40 + Global::margin;
    
    std::unique_ptr<TextButton> logPlotButton;
    
    double highestGain = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaseResponse)
};
