/*
  ==============================================================================

    AudioPlayer.h
    Created: 28 Jul 2020 3:48:33pm
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
class AudioPlayer  : public AppComponent, public Button::Listener
{
public:
    AudioPlayer();
    ~AudioPlayer() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void calculate() override;
    
    void buttonClicked (Button* button) override;

    bool shouldPlayNoise() { return playNoise; };
    bool shouldScaleOutput() { return scaleOutput; };
    float getOutput() { return output; };
    
    void resetStates() { prevOutputs.clear(); prevOutputs.resize (Global::numCoeffs * 0.5, 0); };
    
    
    
private:
    std::unique_ptr<TextButton> noiseButton;
    std::unique_ptr<TextButton> scaleButton;

    bool playNoise = false;
    bool scaleOutput = true;
    Colour defaultButtonColour;

    std::vector<double> prevInputs;
    std::vector<double> prevOutputs;
    int readPointer = 1;
    int writePointer = 1;
    float output = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayer)
};
