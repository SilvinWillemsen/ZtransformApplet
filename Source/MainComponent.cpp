#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    addAndMakeVisible (coefficientList);

    for (int i = 0; i < Global::numCoeffs; ++i)
        coefficientList.getTextEditor (i).addListener(this);
        
    appComponents.add (new DifferenceEq ());
    appComponents.add (new TransferFunction ());

    for (auto appComp : appComponents)
    {
        appComp->setData (coefficientList.getData());
        appComp->refresh();
        addAndMakeVisible (appComp);

    }
    
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colour (Global::backgroundColour));
    
    g.setColour (Colours::black);
    Rectangle<int> totArea = getLocalBounds();
    totArea.removeFromRight (100);
    
    for (int i = 0; i < appComponents.size(); ++i )
        g.drawRect (totArea.removeFromTop(100), 1);

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    Rectangle<int> totArea = getLocalBounds();
    coefficientList.setBounds (totArea.removeFromRight (100));
    
    for (int i = 0; i < appComponents.size(); ++i)
        appComponents[i]->setBounds (totArea.removeFromTop (100).reduced (Global::margin));

}

void MainComponent::textEditorTextChanged (TextEditor& textEditor)
{
    // remove zeros
    if (textEditor.getText().startsWith("0"))
        textEditor.setText (textEditor.getText().substring(1, textEditor.getText().length()));
    
    bool isACoeff = textEditor.getName().startsWith("a");
    int idx = textEditor.getName().removeCharacters(isACoeff ? "a" : "b").getIntValue() + (isACoeff ? 0 : Global::numCoeffs * 0.5);
    
    coefficientList.updateCoeff (idx);
    for (auto comp : appComponents)
    {
        comp->setData (coefficientList.getData());
        comp->refresh();
    }
}

void MainComponent::mouseDown (const MouseEvent& e)
{
}
