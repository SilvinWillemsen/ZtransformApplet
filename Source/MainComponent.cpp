#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    
    addAndMakeVisible (coefficientList);

    for (int i = 0; i < Global::numCoeffs; ++i)
        coefficientList.getTextEditor (i).addListener(this);
    
    appComponents.resize (7);
    appComponents[0] = std::make_shared<DifferenceEq> ();
    differenceEq = std::static_pointer_cast<DifferenceEq>(appComponents[0]);
    appComponents[1] = std::make_shared<TransferFunction> ();
    transferFunction = std::static_pointer_cast<TransferFunction>(appComponents[1]);
    appComponents[2] = std::make_shared<PoleZeroPlot> ();
    poleZeroPlot = std::static_pointer_cast<PoleZeroPlot>(appComponents[2]);
    appComponents[3] = std::make_shared<BlockDiagram> ();
    blockDiagram = std::static_pointer_cast<BlockDiagram>(appComponents[3]);
    appComponents[4] = std::make_shared<AudioPlayer> ();
    audioPlayer = std::static_pointer_cast<AudioPlayer>(appComponents[4]);


    for (int i = 0; i < 5; ++i)
    {
        appComponents[i]->setCoefficients (coefficientList.getCoefficients());
        appComponents[i]->refresh();
        addAndMakeVisible (appComponents[i].get());

    }

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
        setAudioChannels (0, 2);
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
    int idx = appComponents.size() - 2;
    appComponents[idx] = std::make_shared<FreqResponse> (sampleRate);
    freqResponse = std::static_pointer_cast<FreqResponse>(appComponents[idx]);
    appComponents[idx]->setCoefficients (coefficientList.getCoefficients());
    appComponents[idx]->refresh();
    addAndMakeVisible (appComponents[idx].get());

    appComponents[idx+1] = std::make_shared<PhaseResponse> (sampleRate);
    phaseResponse = std::static_pointer_cast<PhaseResponse>(appComponents[idx+1]);
    appComponents[idx+1]->setCoefficients (coefficientList.getCoefficients());
    appComponents[idx+1]->refresh();
    addAndMakeVisible (appComponents[idx+1].get());

    setSize (1140, 500);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
    float* const channeldataL = bufferToFill.buffer->getWritePointer (0);
    float* const channeldataR = bufferToFill.buffer->getWritePointer (1);
    
    if (poleZeroPlot->isStable())
        audioPlayer->resetStates();
    
    if (poleZeroPlot->isStable() && audioPlayer->shouldPlayNoise())
        play = true;
    else
        play = false;
    
    outputScaling = audioPlayer->shouldScale() ? 1.0 / freqResponse->getHighestGain() : 1.0;
    for (int i = 0; i < bufferToFill.buffer->getNumSamples(); ++i)
    {
        audioPlayer->calculate();
        channeldataL[i] = Global::limit (play ? audioPlayer->getOutput() * outputScaling : 0.0, -1, 1);
        if (channeldataR != nullptr)
            channeldataR[i] = Global::limit (play ? audioPlayer->getOutput() * outputScaling : 0.0, -1, 1);
    }
   
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
    g.drawRect (blockDiagram->getBounds().withHeight (blockDiagram->getHeight() * 0.5));
    g.setFont (Font (20.0f));
    g.drawText (blockDiagram->getTitle(), blockDiagram->getX() + Global::margin, blockDiagram->getY() + Global::margin, blockDiagram->getWidth(), 20.0, Justification::centredLeft);
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
//    AffineTransform transform;
//    transform = transform.scaled(getWidth() / 1140.0f, getHeight() / 500.0f);
//    setTransform (transform);
    Rectangle<int> totArea = getLocalBounds();
    coefficientList.setBounds (totArea.removeFromLeft (100));
    
    Rectangle<int> leftPart = totArea.removeFromLeft (totArea.getWidth() * 0.4);
    Rectangle<int> middlePart = totArea.removeFromLeft (300);
    Rectangle<int> rightPart = totArea;

    audioPlayer->setBounds (leftPart.removeFromTop (50));
    freqResponse->setBounds(leftPart.removeFromTop (leftPart.getHeight() * 0.5));
    phaseResponse->setBounds(leftPart);

    poleZeroPlot->setBounds (middlePart.removeFromBottom (middlePart.getWidth()));
    differenceEq->setBounds (middlePart.removeFromTop (middlePart.getHeight() * 0.5));
    transferFunction->setBounds (middlePart);
    blockDiagram->setBounds (rightPart.withHeight (rightPart.getHeight() * 2.0));
    
//        if (appComponents[i]->getTitle() == "Frequency Response")
//            appComponents[i]->setBounds (totArea.removeFromTop (200));
//        else if (appComponents[i]->getTitle() == "Pole-Zero Plot")
//        {
//            appComponents[i]->setBounds (totArea.getX(), totArea.getY(), 200, 200);
//            totArea.removeFromTop (200);
//        } else
//            appComponents[i]->setBounds (totArea.removeFromTop (100));
//
//    }

}

void MainComponent::textEditorTextChanged (TextEditor& textEditor)
{
    // remove zeros
    if (textEditor.getText() != "0" && textEditor.getText().startsWith("0") && textEditor.getText().substring(1, 2) != ".")
            textEditor.setText (textEditor.getText().substring(1, textEditor.getText().length()));
    
    bool isACoeff = textEditor.getName().startsWith("a");
    int idx = textEditor.getName().removeCharacters (isACoeff ? "a" : "b").getIntValue() + (isACoeff ? Global::numCoeffs * 0.5 : 0);
    
    coefficientList.updateCoeff (idx);
    for (auto comp : appComponents)
    {
        comp->setCoefficients (coefficientList.getCoefficients());
        if (comp->getTitle() != "Audio")
        comp->refresh();
    }
}

void MainComponent::mouseDown (const MouseEvent& e)
{
}
