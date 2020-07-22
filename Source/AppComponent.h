/*
  ==============================================================================

    AppComponent.h
    Created: 21 Jul 2020 10:53:39am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Global.h"
#include <complex>

using namespace juce;
//==============================================================================
/*
*/
class AppComponent  : public Component
{
public:
    AppComponent (String title, bool isEquation);
    ~AppComponent() override;

    void paint (Graphics&) override;
    void drawTitle (Graphics&);
    void drawOutline (Graphics&);
    
    void resized() override;

    String& getTitle() { return title; };
    void setCoefficients (std::vector<double>& coefficientsToSet);
    
    void refresh();
    
    virtual void calculate() {};

protected:
    Font textFont { 20.0f };
    Font equationFont { "CMU Serif", 25.0f, Font::italic };

    std::vector<double> coefficients;
    std::vector<std::complex<double>> data;
    
private:
    String title;
    bool isEquation;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AppComponent)
};
