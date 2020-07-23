/*
  ==============================================================================

    TransferFunction.h
    Created: 21 Jul 2020 2:08:56pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Global.h"
#include "AppComponent.h"

//==============================================================================
/*
*/
class TransferFunction  : public AppComponent
{
public:
    TransferFunction();
    ~TransferFunction() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void calculate() override;

    void removePowers (String& equation, bool isX);
private:
    String xEquation;
    String yEquation;

    std::vector<int> xPowers;
    std::vector<int> yPowers;
    std::vector<float> xPowerLocs;
    std::vector<float> yPowerLocs;
    
    int xPowersAmount = 0;
    int yPowersAmount = 0;

    bool hasYcomponent = false;
    
    float scaling = 1.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransferFunction)
};
