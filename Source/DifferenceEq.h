/*
  ==============================================================================

    DifferenceEq.h
    Created: 21 Jul 2020 10:53:27am
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
class DifferenceEq  : public AppComponent
{
public:
    DifferenceEq();
    ~DifferenceEq() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void calculate() override;

private:
    String equation;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DifferenceEq)
};
