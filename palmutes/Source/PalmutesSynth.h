/*
  ==============================================================================

    PalmutesSynth.h
    Created: 9 Mar 2024 12:56:54pm
    Author:  USER

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <iostream>
#include <string>

class PalmutesSynth : public juce::Synthesiser {
public:
    void setup();
    void runtimeSoundConfiguration(juce::MidiBuffer& midiMessages);
    juce::BigInteger allNotes;

    float attackTimeToSet;
    float releaseTimeToSet;
private:
    juce::AudioFormatManager audioFormatManager;
    std::string SAMPLES_ROOT_DIR = "C:\\Users\\USER\\other-nerd-stuff\\projects\\palmutes\\samples";
};