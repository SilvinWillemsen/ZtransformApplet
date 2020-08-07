/*
  ==============================================================================

    BlockDiagram.h
    Created: 23 Jul 2020 1:35:07pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Global.h"
#include "AppComponent.h"
#include "DiagramComponent.h"
//==============================================================================
/*
*/

using namespace juce;

class BlockDiagram  : public AppComponent
{
public:
    BlockDiagram();
    ~BlockDiagram() override;

    void paint (Graphics&) override;
    void resized() override;

    void calculate() override;
    
    bool hasDelays();
    bool hasGain();
    
    bool checkForNextCoefficient (int& idx);

    float getScaling() { return scaling; };
    float getTopLoc() { return topLoc; };
    
private:
    
    OwnedArray<DiagramComponent> components;
    OwnedArray<DiagramComponent> coefficientComps;

    float spacing = 3;
    
    int numXGains = 0;
    int numYGains = 0;
    
    bool drawingX = true;
    int delayVal = 1;
    float scaling = 1;
    
    float visibleHeight = 300;
    float topLoc = 60;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlockDiagram)
};
