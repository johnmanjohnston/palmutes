/*
  ==============================================================================

    PalmutesSynth.cpp
    Created: 9 Mar 2024 12:56:54pm
    Author:  USER

  ==============================================================================
*/

#include "PalmutesSynth.h"
#include "PluginEditor.h"
using namespace juce;

void PalmutesSynth::setup()
{
    for (int i = 0; i < 2; i++) {
        addVoice(new SamplerVoice());
    }

    this->audioFormatManager.registerBasicFormats();

    this->allNotes.setRange(0, 128, true);

    File* file = new File("C:\\Users\\USER\\OneDrive\\Documents\\Ableton\\Live Recordings\\2024-03-09 130654 Temp Project\\Samples\\Processed\\Consolidate\\GrandPiano C3 f [2024-03-09 130834].wav");
    ScopedPointer<AudioFormatReader> reader = audioFormatManager.createReaderFor(*file);
    addSound(new SamplerSound("default", *reader, allNotes, 60, 10, 10, 10.f));
}

void PalmutesSynth::runtimeSoundConfiguration(MidiBuffer& midiMessages)
{
    if (midiMessages.isEmpty()) return;

    for (const auto midiData : midiMessages) {
        const auto midiMessage = midiData.getMessage();
        const auto noteNumber = midiMessage.getNoteNumber();

        if (midiMessage.isNoteOn()) { this->noteOn(1, noteNumber, midiMessage.getFloatVelocity()); }
        if (midiMessage.isNoteOff()) { this->noteOff(1, noteNumber, midiMessage.getFloatVelocity(), true); }

        // structure for sample file path is: /samples/<harmonized OR unharmonized>/<note_number>_<velocity>.wav
        File* file = nullptr;
        if (noteNumber > 80) {
            file = new File("C:\\Users\\USER\\OneDrive\\Documents\\Ableton\\Live Recordings\\2024-03-09 130654 Temp Project\\Samples\\Processed\\Consolidate\\GrandPiano C3 f [2024-03-09 130834].wav");
        }
        else {
            file = new File("C:\\Users\\USER\\other-nerd-stuff\\projects\\palmutes\\samples\\pm-a.wav");
        }

        ScopedPointer<AudioFormatReader> reader = audioFormatManager.createReaderFor(*file);

        addSound(new SamplerSound("default", *reader, allNotes, 69, attackTimeToSet, releaseTimeToSet, 10.f));
        this->sounds.remove(0);

        DBG(noteNumber);
    }
}