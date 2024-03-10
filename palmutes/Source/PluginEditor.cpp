/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PalmutesAudioProcessorEditor::PalmutesAudioProcessorEditor (PalmutesAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    gainSliderParamAttachment(*p.gainParamter, gainSlider), attackTimeSliderParamAttachment(*p.attackTime, attackTimeSlider),
    releaseTimeSliderParamAttachment(*p.releaseTime, releaseTimeSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    this->setSize(WINDOW_RATIO_X * WINDOW_RATIO_MULTIPLIER, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER);
    this->addAndMakeVisible(keyboardComponent);
}

PalmutesAudioProcessorEditor::~PalmutesAudioProcessorEditor()
{
}

//==============================================================================
static const juce::Font GetSpaceGrotesk()
{
    static juce::Font typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::SpaceGroteskBold_ttf, BinaryData::SpaceGroteskBold_ttfSize);
    return typeface;
}

void PalmutesAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // set background gradient
    juce::Image image(juce::Image::ARGB, getWidth(), getHeight(), true);
    juce::Graphics tg(image);
    juce::Colour bgCol(35, 41, 48);
    juce::ColourGradient cg = juce::ColourGradient::horizontal(bgCol, 0.0, bgCol.brighter(.1f), getWidth());
    tg.setGradientFill(cg);
    tg.fillAll();
    g.drawImage(image, getLocalBounds().toFloat());

    // draw author credits
    g.setFont(GetSpaceGrotesk());
    auto grayedOutTextcolor = juce::Colour(100, 100, 110);
    g.setColour(grayedOutTextcolor);
    g.setFont(18.f);
    g.drawFittedText("Palmutes, by Ethan John / JohnmanJohnston", 10, (WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 1.58f), 500, 50, juce::Justification::left, 1);

    // draw Palm Mutes logo
    juce::Image palmutesLogo = juce::ImageCache::getFromMemory(BinaryData::Palm_Muteshighres_png, BinaryData::Palm_Muteshighres_pngSize);
    g.drawImageWithin(palmutesLogo, 10, -(WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 2.2f), 400, 400, false);

    // draw guitar
    juce::Image guitar = juce::ImageCache::getFromMemory(BinaryData::guitar_png, BinaryData::guitar_pngSize);
    g.drawImageWithin(guitar, (WINDOW_RATIO_X * WINDOW_RATIO_MULTIPLIER / 2.6f), -(WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 4.f), 500, 500, 0);

    // draw parameter UI components
    // draw gain slider
    addAndMakeVisible(gainSlider);
    gainSlider.setRange(audioProcessor.gainParamter->range.start, audioProcessor.gainParamter->range.end);
    gainSlider.setTextValueSuffix(" gain");
    gainSlider.setSkewFactor(1.2f);
    gainSlider.setNumDecimalPlacesToDisplay(2);

    // draw attack time slider
    addAndMakeVisible(attackTimeSlider);
    attackTimeSlider.setRange(audioProcessor.minTime, audioProcessor.maxTime);
    attackTimeSlider.setTextValueSuffix("Attack Time");
    attackTimeSlider.setNumDecimalPlacesToDisplay(2);

    // draw release time slider
    addAndMakeVisible(releaseTimeSlider);
    releaseTimeSlider.setRange(audioProcessor.minTime, audioProcessor.maxTime);
    releaseTimeSlider.setTextValueSuffix("Release Time");
    releaseTimeSlider.setNumDecimalPlacesToDisplay(2);

    // handle value initialization and setting for attack and release time sliders
    attackTimeSlider.setValue(audioProcessor.attackTime->get());
    attackTimeSlider.onValueChange = [this] {audioProcessor.synth.attackTimeToSet = attackTimeSlider.getValue(); };

    releaseTimeSlider.setValue(audioProcessor.releaseTime->get());
    releaseTimeSlider.onValueChange = [this] {audioProcessor.synth.releaseTimeToSet = releaseTimeSlider.getValue(); };
}

void PalmutesAudioProcessorEditor::resized()
{
    // position on-screen keyboard
    keyboardComponent.setBounds(
        0, 
        ((WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 3) + WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 3) + ((WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER) / 11), 
        WINDOW_RATIO_X * WINDOW_RATIO_MULTIPLIER, 
        WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 4
    );
    
    // position sliders
    gainSlider.setBounds(getLocalBounds() / 2);
    releaseTimeSlider.setBounds(WINDOW_RATIO_X * WINDOW_RATIO_MULTIPLIER / 2, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 3, 300, 50);
    attackTimeSlider.setBounds(WINDOW_RATIO_X * WINDOW_RATIO_MULTIPLIER / 2, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 2, 300, 50);
}