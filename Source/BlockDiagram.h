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
    
    void drawArrow (Graphics& g);
    
    bool hasDelays();
    bool hasGain();

private:
    
    OwnedArray<DiagramComponent> components;
    OwnedArray<DiagramComponent> coefficientComps;

    float spacing = 3;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlockDiagram)
};
