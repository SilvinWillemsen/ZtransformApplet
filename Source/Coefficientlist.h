/*
  ==============================================================================

    Coefficientlist.h
    Created: 20 Jul 2020 2:43:14pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Global.h"
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
    
    int getNumRows() override { return Global::numCoeffs; };

    void paintListBoxItem (int rowNumber,
                           Graphics& g,
                           int width, int height,
                           bool rowIsSelected) override;
    
    Component* refreshComponentForRow (int rowNumber, bool isRowSelected, Component* existingComponentToUpdate) override;

    std::vector<double>& getData();
    
    void updateCoeff (int idx);
    
    void refreshCoefficients();
    
    TextEditor& getTextEditor (int idx) { return coeffComps[idx]->getTextEditor(); };

private:
    
    std::vector<std::shared_ptr<CoefficientComponent>> coeffComps;
//    CoefficientComponent coeffCompTest {0, true};
    ListBox list { {}, this };
    
    std::vector<double> coefficients;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Coefficientlist)
};
