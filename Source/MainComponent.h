#pragma once

#include <JuceHeader.h>
#include "Coefficientlist.h"
#include "AppComponent.h"
#include "DifferenceEq.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public TextEditor::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void textEditorTextChanged (TextEditor& textEditor) override;

private:
    //==============================================================================
    // Your private member variables go here...
    Coefficientlist coefficientList;

    OwnedArray<AppComponent> appComponents;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
