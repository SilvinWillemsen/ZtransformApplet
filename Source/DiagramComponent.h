/*
  ==============================================================================

    DiagramComponent.h
    Created: 23 Jul 2020 2:04:26pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Global.h"

//==============================================================================
/*
*/
using namespace juce;

enum ComponentType
{
    arrow,
    inOutput,
    equation,
    delay,
    add,
    gain,
};

enum ArrowType
{
    hor,
    vert,
    cor, //corner
    diag
};

class DiagramComponent  : public juce::Component
{
public:
    DiagramComponent (ComponentType type, ArrowType arrowType = hor);
    ~DiagramComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    ComponentType getType() { return type; };
    
    float getData() { return data; };
    void setData (float dat) { data = dat; }
    
    float getCompHeight() { return height; };
    
    ArrowType getArrowType();
    void setArrowType (ArrowType arrType);
    
    bool isACoefficient() { return isACoeff; };
    void setACoeff (bool isA) { isACoeff = isA; };
    
private:
    ComponentType type;
    Font equationFont { "CMU Serif", 25.0f, Font::italic };

    float data = 0.0;
    float height;
    
    ArrowType arrowType = hor;
    bool isACoeff = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiagramComponent)
};
