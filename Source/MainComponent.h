#pragma once

#include <JuceHeader.h>
#include "Coefficientlist.h"
#include "AppComponent.h"
#include "DifferenceEq.h"
#include "TransferFunction.h"
#include "FreqResponse.h"
#include "PoleZeroPlot.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
public TextEditor::Listener
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
    
    void mouseDown (const MouseEvent& e) override;

private:
    //==============================================================================
    // Your private member variables go here...
    Coefficientlist coefficientList;

    std::vector<std::shared_ptr<AppComponent>> appComponents;
    
    std::shared_ptr<DifferenceEq> differenceEq;
    std::shared_ptr<TransferFunction> transferFunction;
    std::shared_ptr<FreqResponse> freqResponse;
    std::shared_ptr<PoleZeroPlot> poleZeroPlot;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
