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

using namespace juce;
//==============================================================================
/*
*/
class AppComponent  : public juce::Component
{
public:
    AppComponent (String title, bool isEquation);
    ~AppComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    String& getTitle() { return title; };
    void setData (std::vector<double>& dataToSet);
    
    virtual void refresh() {};
    
protected:
    Font textFont { 20.0f };
    Font equationFont { "CMU Serif", 25.0f, Font::italic };

    std::vector<double> data;

private:
    String title;
    bool isEquation;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AppComponent)
};
