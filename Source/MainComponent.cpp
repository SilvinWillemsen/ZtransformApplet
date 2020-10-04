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

    setSize (Global::initWidth, Global::initHeight);
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
    
    outputScaling = audioPlayer->shouldScaleOutput() ? 1.0 / freqResponse->getHighestGain() : 1.0;
    for (int i = 0; i < bufferToFill.buffer->getNumSamples(); ++i)
    {
        audioPlayer->calculate();
        channeldataL[i] = Global::outputLimit (play ? audioPlayer->getOutput() * outputScaling * 0.5 : 0.0);
        if (channeldataR != nullptr)
            channeldataR[i] = Global::outputLimit (play ? audioPlayer->getOutput() * outputScaling * 0.5 : 0.0);
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
    float scale = (Global::initHeight * static_cast<float>(getWidth()) / static_cast<float>(Global::initWidth)) / Global::initHeight;
    g.fillAll (Colour (Global::backgroundColour));
    
    g.drawRect ((differenceEq->getX() + differenceEq->getWidth()) * scale,
                blockDiagram->getY() * scale,
                blockDiagram->getWidth() * scale,
                blockDiagram->getHeight() * 0.5 * scale);
    g.setFont (Font (20.0f * scale));
    g.drawText (blockDiagram->getTitle(), (blockDiagram->getX() + Global::margin) * scale, (blockDiagram->getY() + Global::margin) * scale, blockDiagram->getWidth() * scale, 20.0 * scale, Justification::centredLeft);
    AffineTransform transform;
    transform = transform.scaled (scale);
    transform = transform.scaled (blockDiagram->getScaling(),
                                  blockDiagram->getScaling(),
//                                  getWidth(), getHeight());
                                  (816 + 162) * scale,
                                  0.0 + blockDiagram->getTopLoc() - 0.5 * Global::bdCompDim);
    blockDiagram->setTransform (transform);
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    AffineTransform transform;
    float scaling = floor(Global::initHeight * static_cast<float>(getWidth()) / static_cast<float>(Global::initWidth)) / Global::initHeight;
    transform = transform.scaled (scaling);
//    setTransform (transform);
//    for (auto comp : appComponents)
//    {
        blockDiagram->setScale (scaling);
//    }
    
    Rectangle<int> totArea (0, 0, Global::initWidth, Global::initHeightWithOffset);
    coefficientList.setBounds (totArea.removeFromLeft (150));
    coefficientList.setTransform (transform);
    
    Rectangle<int> leftPart = totArea.removeFromLeft (366);
    Rectangle<int> middlePart = totArea.removeFromLeft (300);
    Rectangle<int> rightPart = totArea.removeFromLeft (324);
    
    audioPlayer->setBounds (leftPart.removeFromTop (50));
    freqResponse->setBounds (leftPart.removeFromTop ((Global::initHeightWithOffset - 50) * 0.5));
    phaseResponse->setBounds (leftPart);
    
    poleZeroPlot->setBounds (middlePart.removeFromBottom (300));
    differenceEq->setBounds (middlePart.removeFromTop ((Global::initHeightWithOffset - 300) * 0.5));
    transferFunction->setBounds (middlePart);
    blockDiagram->setBounds (rightPart.withHeight (rightPart.getHeight() * 2.0));

    for (auto comp : appComponents)
        if (comp->getTitle() != "Block Diagram") // updated in the paint function
            comp->setTransform (transform);
//    this->repaint();
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
