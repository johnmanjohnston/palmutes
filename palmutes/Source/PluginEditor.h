/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PalmutesLookAndFeel.h"
#include "SharedData.h"
#include <vector>

//==============================================================================
/**
*/

class KeyboardStateListener : public juce::MidiKeyboardState::Listener, public juce::ChangeBroadcaster {
public:
    std::vector<int> activeNotes;

    void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override {
        activeNotes.push_back(midiNoteNumber);
        sendChangeMessage();
    }

    void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override {
        for (auto it = activeNotes.begin(); it != activeNotes.end(); ++it) {
            if (*it == midiNoteNumber) {
                activeNotes.erase(it);
                break;
            }
        }
        sendChangeMessage();
    }
};

class PalmutesAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::ChangeListener
{
public:
    PalmutesAudioProcessorEditor (PalmutesAudioProcessor&);
    ~PalmutesAudioProcessorEditor() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    /*
    class ChListener : public juce::ChangeListener {
        void changeListenerCallback(ChangeBroadcaster* source) override;
    };

    ChListener listener;
    */

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    int getStringVerticalPosition(int stringNumber); // 0 is low E, 6 is high E
    int getFretHorizontalPosition(int fret);

    juce::MidiKeyboardComponent keyboardComponent;

    juce::Slider gainSlider;
    juce::SliderParameterAttachment gainSliderParamAttachment;

    juce::Slider attackTimeSlider;
    juce::SliderParameterAttachment attackTimeSliderParamAttachment;

    juce::Slider releaseTimeSlider;
    juce::SliderParameterAttachment releaseTimeSliderParamAttachment;

    juce::Slider preGainSlider;
    juce::SliderParameterAttachment pregainSliderAttachment;

    juce::ToggleButton harmonizationChekbox;

    PalmutesLookAndFeel palmutesLookAndFeel;
    KeyboardStateListener kbListener;

    std::vector<int> activeNotes;

    // range for drawing fretting indicators
    const int MIN_PLAYABLE_MIDI_NOTE = 52; // E2
    const int MAX_PLAYABLE_MIDI_NOTE = 67; 
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PalmutesAudioProcessor& audioProcessor;

    const uint16_t WINDOW_RATIO_X = 16;
    const uint16_t WINDOW_RATIO_Y = 7;
    const uint16_t WINDOW_RATIO_MULTIPLIER = 50;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PalmutesAudioProcessorEditor)
};
