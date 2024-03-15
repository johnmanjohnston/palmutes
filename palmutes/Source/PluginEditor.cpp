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
    gainSlider.setLookAndFeel(nullptr);
    attackTimeSlider.setLookAndFeel(nullptr);
    releaseTimeSlider.setLookAndFeel(nullptr);
    harmonizationChekbox.setLookAndFeel(nullptr);
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
    g.drawFittedText("Palmutes, by Ethan John / JohnmanJohnston\nFree and open-source at github.com/johnmanjohnston/palmutes/", 10, (WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 1.58f), 500, 300, juce::Justification::left, 2);

    // draw texture image, and draw
    juce::Image textureImg = juce::ImageCache::getFromMemory(BinaryData::bgtexture_png, BinaryData::bgtexture_pngSize);
    g.drawImageAt(textureImg, 0, 0);

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
    gainSlider.setTextValueSuffix(" - GAIN");
    gainSlider.setSkewFactor(1.2f);
    gainSlider.setNumDecimalPlacesToDisplay(2);
    gainSlider.setLookAndFeel(&palmutesLookAndFeel);
    //gainSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 80, 26);

    // draw attack time slider
    addAndMakeVisible(attackTimeSlider);
    attackTimeSlider.setRange(audioProcessor.minTime, audioProcessor.maxTime);
    attackTimeSlider.setTextValueSuffix(" - ATTACK");
    attackTimeSlider.setNumDecimalPlacesToDisplay(2);
    attackTimeSlider.setLookAndFeel(&palmutesLookAndFeel);
    
    // draw release time slider
    addAndMakeVisible(releaseTimeSlider);
    releaseTimeSlider.setRange(audioProcessor.minTime, audioProcessor.maxTime);
    releaseTimeSlider.setTextValueSuffix(" - RELEASE");
    releaseTimeSlider.setNumDecimalPlacesToDisplay(2);
    releaseTimeSlider.setLookAndFeel(&palmutesLookAndFeel);

    // draw harmonization checkbox
    addAndMakeVisible(harmonizationChekbox);
    harmonizationChekbox.setButtonText("HARMONIZE");
    harmonizationChekbox.setLookAndFeel(&palmutesLookAndFeel);

    // handle value initialization and setting for attack and release time sliders
    attackTimeSlider.setValue(audioProcessor.attackTime->get());
    audioProcessor.synth.attackTimeToSet = attackTimeSlider.getValue();
    attackTimeSlider.onValueChange = [this] {audioProcessor.synth.attackTimeToSet = attackTimeSlider.getValue(); audioProcessor.synth.refreshParams = true; };

    releaseTimeSlider.setValue(audioProcessor.releaseTime->get());
    audioProcessor.synth.releaseTimeToSet = releaseTimeSlider.getValue();
    releaseTimeSlider.onValueChange = [this] {audioProcessor.synth.releaseTimeToSet = releaseTimeSlider.getValue(); audioProcessor.synth.refreshParams = true; };
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
    gainSlider.setBounds       (15, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 5.f, 300, 50);
    releaseTimeSlider.setBounds(15, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 3.5f,300, 50);
    attackTimeSlider.setBounds (15, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 2.7f,300, 50);

    harmonizationChekbox.setBounds(12, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 2.22f, 300, 50);
}
