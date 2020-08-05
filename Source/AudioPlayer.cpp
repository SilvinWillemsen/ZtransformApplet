/*
  ==============================================================================

    AudioPlayer.cpp
    Created: 28 Jul 2020 3:48:33pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioPlayer.h"

//==============================================================================
AudioPlayer::AudioPlayer() : AppComponent ("Audio", false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    prevInputs.resize (Global::numCoeffs * 0.5);
    prevOutputs.resize (Global::numCoeffs * 0.5);

    noiseButton = std::make_unique<TextButton> ("Noise");
    addAndMakeVisible (noiseButton.get());
    defaultButtonColour = noiseButton->getLookAndFeel().findColour (TextButton::buttonColourId);
    noiseButton->addListener (this);
    noiseButton->setColour (TextButton::buttonColourId, playNoise ? Colours::green : defaultButtonColour);

    
    scaleButton = std::make_unique<TextButton> ("Auto scale");
    addAndMakeVisible (scaleButton.get());
    scaleButton->addListener (this);
    scaleButton->setColour (TextButton::buttonColourId, scale ? Colours::green : defaultButtonColour);

}

AudioPlayer::~AudioPlayer()
{
}

void AudioPlayer::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    drawTitle (g);
    drawOutline (g);
    
}

void AudioPlayer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> totArea = getLocalBounds();
    totArea.reduce (Global::margin, Global::margin);
    noiseButton->setBounds (totArea.removeFromRight (100));
    totArea.removeFromRight(Global::margin);
    scaleButton->setBounds (totArea.removeFromRight (100));
}

void AudioPlayer::buttonClicked (Button* button)
{
    if (button == noiseButton.get())
    {
    playNoise = !playNoise;
    noiseButton->setColour (TextButton::buttonColourId, playNoise ? Colours::green : defaultButtonColour);
    }
    else if (button == scaleButton.get())
    {
        scale = !scale;
        scaleButton->setColour (TextButton::buttonColourId, scale ? Colours::green : defaultButtonColour);
    }
}

void AudioPlayer::calculate()
{
    Random r;
    output = 0;
    prevInputs[writePointer] = r.nextFloat() - 0.5;
    for (int i = 0; i < Global::numCoeffs * 0.5; ++i)
    {
        output += coefficients[i] * prevInputs[static_cast<int> (writePointer - i + Global::numCoeffs * 0.5) % static_cast<int> (Global::numCoeffs * 0.5)];
    }
    for (int i = Global::numCoeffs * 0.5 + 1; i < Global::numCoeffs; ++i)
    {
        output += coefficients[i] * prevOutputs[static_cast<int> (writePointer - (i - Global::numCoeffs * 0.5) + Global::numCoeffs * 0.5 ) % static_cast<int> (Global::numCoeffs * 0.5)];
    }
    prevOutputs[writePointer] = output;
    writePointer = (writePointer + 1) % static_cast<int> (Global::numCoeffs * 0.5);
}
