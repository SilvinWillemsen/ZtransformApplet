/*
  ==============================================================================

    Coefficientlist.h
    Created: 20 Jul 2020 2:43:14pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CoefficientComponent.h"

using namespace juce;
//==============================================================================
/*
*/
class Coefficientlist  : public Component, public ListBoxModel
{
public:
    Coefficientlist();
    ~Coefficientlist() override;

    void paint (Graphics&) override;
    void resized() override;
    
    int getNumRows() override { return numCoeffs; };

    void paintListBoxItem (int rowNumber,
                           Graphics& g,
                           int width, int height,
                           bool rowIsSelected) override;
    
    Component* refreshComponentForRow (int rowNumber, bool isRowSelected, Component* existingComponentToUpdate) override;

private:
    ListBox list { {}, this };
    
    std::vector<double> coefficients;
    OwnedArray<CoefficientComponent> coeffComps;
    
    int numCoeffs = 10;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Coefficientlist)
};
