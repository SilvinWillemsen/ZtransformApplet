/*
  ==============================================================================

    CoefficientComponent.h
    Created: 20 Jul 2020 3:07:18pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

    double getCoefficient() { return value.getText().getDoubleValue(); };
    
private:
    int ID;
    bool aCoeff;
    bool isA0;
    TextEditor value;
    
    Font font { "Latin Modern Roman", 25.0f, Font::plain };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoefficientComponent)
};
