/*
  ==============================================================================

    CoefficientComponent.h
    Created: 20 Jul 2020 3:07:18pm
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
class CoefficientComponent  : public Component
{
public:
    CoefficientComponent (int ID, bool aCoeff);
    ~CoefficientComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    double getCoefficient() { return isA0 ? 1 : value.getText().getDoubleValue(); };

    TextEditor& getTextEditor() { return value; };
    
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    
private:
    int ID;
    bool aCoeff;
    bool isA0;
    TextEditor value;
    
    int curX = 0;
    double curVal = 0;
    int diffX = 0;
    Font font { Typeface::createSystemTypefaceFor (BinaryData::CMUSerifItalic_ttf, BinaryData::CMUSerifItalic_ttfSize) };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoefficientComponent)
};
