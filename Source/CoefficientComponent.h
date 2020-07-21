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
class CoefficientComponent  : public Component, public TextEditor::Listener
{
public:
    CoefficientComponent (int ID, bool aCoeff);
    ~CoefficientComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    double getCoefficient() { return isA0 ? 1 : value.getText().getDoubleValue(); };

    TextEditor& getTextEditor() { return value; };
    
private:
    int ID;
    bool aCoeff;
    bool isA0;
    TextEditor value;
    
    Font font { "CMU Serif", 25.0f, Font::italic };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoefficientComponent)
};
