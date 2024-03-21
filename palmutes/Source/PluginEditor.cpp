/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PalmutesAudioProcessorEditor::PalmutesAudioProcessorEditor(PalmutesAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    gainSliderParamAttachment(*p.gainParamter, gainSlider), attackTimeSliderParamAttachment(*p.attackTime, attackTimeSlider),
    releaseTimeSliderParamAttachment(*p.releaseTime, releaseTimeSlider), pregainSliderAttachment(*p.preGainParam, preGainSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    this->setSize(WINDOW_RATIO_X * WINDOW_RATIO_MULTIPLIER, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER);
    this->addAndMakeVisible(keyboardComponent);

    p.keyboardState.addListener(&kbListener);
    kbListener.addChangeListener(this);
}

PalmutesAudioProcessorEditor::~PalmutesAudioProcessorEditor()
{   
    gainSlider.setLookAndFeel(nullptr);
    attackTimeSlider.setLookAndFeel(nullptr);
    releaseTimeSlider.setLookAndFeel(nullptr);
    harmonizationChekbox.setLookAndFeel(nullptr);

    preGainSlider.setLookAndFeel(nullptr);
}

void PalmutesAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
    DBG("from the editirorasdfasdfasdforo");
    this->activeNotes = kbListener.activeNotes;

    repaint();
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
    juce::Image guitar = juce::ImageCache::getFromMemory(BinaryData::guitaroriented_png, BinaryData::guitaroriented_pngSize);
    g.drawImageWithin(guitar, (WINDOW_RATIO_X * WINDOW_RATIO_MULTIPLIER / 2.6f), -(WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 4.f), 500, 500, 0);

    gainSlider.getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::grey);

    // draw parameter UI components
    // draw gain slider
    addAndMakeVisible(gainSlider);
    gainSlider.setRange(audioProcessor.gainParamter->range.start, audioProcessor.gainParamter->range.end);
    gainSlider.setTextValueSuffix(" - GAIN");
    gainSlider.setSkewFactor(1.2f);
    gainSlider.setNumDecimalPlacesToDisplay(2);
    gainSlider.setLookAndFeel(&palmutesLookAndFeel);
    gainSlider.setSize(150, 18);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    //gainSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 80, 26);

    // draw attack time slider
    addAndMakeVisible(attackTimeSlider);
    attackTimeSlider.setRange(audioProcessor.minTime, audioProcessor.maxTime);
    attackTimeSlider.setTextValueSuffix(" - ATTACK");
    attackTimeSlider.setNumDecimalPlacesToDisplay(2);
    attackTimeSlider.setLookAndFeel(&palmutesLookAndFeel);
    attackTimeSlider.setSize(150, 18);
    attackTimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    
    // draw release time slider
    addAndMakeVisible(releaseTimeSlider);
    releaseTimeSlider.setRange(audioProcessor.minTime, audioProcessor.maxTime);
    releaseTimeSlider.setTextValueSuffix(" - RELEASE");
    releaseTimeSlider.setNumDecimalPlacesToDisplay(2);
    releaseTimeSlider.setLookAndFeel(&palmutesLookAndFeel);
    releaseTimeSlider.setSize(150, 18);
    releaseTimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);

    // draw pregain slider
    addAndMakeVisible(preGainSlider);
    preGainSlider.setRange(audioProcessor.preGainParam->range.start, audioProcessor.preGainParam->range.end);
    preGainSlider.setTextValueSuffix(" - PRE-GAIN");
    preGainSlider.setSkewFactor(1.2f);
    preGainSlider.setNumDecimalPlacesToDisplay(2);
    preGainSlider.setLookAndFeel(&palmutesLookAndFeel);
    preGainSlider.setSize(150, 18);
    preGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);

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
    
    preGainSlider.setValue(audioProcessor.preGainParam->get());
    audioProcessor.distortion.preGainValue = preGainSlider.getValue();
    preGainSlider.onValueChange = [this] { audioProcessor.distortion.updateParams(); };

    // draw any active notes
    for (int note : activeNotes) {
        // g.drawRect(getFretHorizontalPosition(note % 12), getStringVerticalPosition(note % 6), 5, 5);

        if (note <= MAX_PLAYABLE_MIDI_NOTE && note >= MIN_PLAYABLE_MIDI_NOTE) {
            int noteNumberRelativeToE2 = note - 44;
            int fret = noteNumberRelativeToE2 % 8; // wrap
            int string = std::floor(noteNumberRelativeToE2 / 8) - 1;

            if (string > 0) {
                fret += 3;
            }

            DBG("fret is" << fret);
            DBG("string is " << string);

            int x = getFretHorizontalPosition(fret);
            int y = getStringVerticalPosition(string);

            DBG(x);
            DBG(y);

            //g.drawRect(x, y, 5, 5);
            g.drawImageAt(juce::ImageCache::getFromMemory(BinaryData::fretmarker_png, BinaryData::fretmarker_pngSize), x, y);
        }
    }
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
    gainSlider.setBounds       (15, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 4.5f, 300, 50);
    releaseTimeSlider.setBounds(15, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 3.5f, 300, 50);
    attackTimeSlider.setBounds (15, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 2.84f,300, 50);
    preGainSlider.setBounds    (15, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 2.39f, 300, 50);

    harmonizationChekbox.setBounds(12, WINDOW_RATIO_Y * WINDOW_RATIO_MULTIPLIER / 2.22f, 300, 50);
}

int PalmutesAudioProcessorEditor::getStringVerticalPosition(int stringNumber)
{
    int retval = 167 - (stringNumber * 4 + (stringNumber / 2));
    return retval;
}

int PalmutesAudioProcessorEditor::getFretHorizontalPosition(int fret)
{
    // <fret number>: <value>
    // 1: 0
    // 2: 19
    // 3: 39
    // 4: 57
    // 5: 73
    // 6: 89
    // 7: 103
    // 8: 117
    // 9: 130

    int maxFret =     9;
    int fretPositions[9] = {
        -20,
        0,
        19,
        39,
        56,
        72,
        88,
        102,
        116,
        //129
    };

    int retval = 464;
    if (fret >= maxFret) { retval += fretPositions[8]; return retval; }

    retval += fretPositions[fret];
    return retval;
}
