/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PalmutesLookAndFeel.h"
#include <vector>

//==============================================================================
/**
*/
class PalmutesAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PalmutesAudioProcessorEditor (PalmutesAudioProcessor&);
    ~PalmutesAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::MidiKeyboardComponent keyboardComponent;

    juce::Slider gainSlider;
    juce::SliderParameterAttachment gainSliderParamAttachment;

    juce::Slider attackTimeSlider;
    juce::SliderParameterAttachment attackTimeSliderParamAttachment;

    juce::Slider releaseTimeSlider;
    juce::SliderParameterAttachment releaseTimeSliderParamAttachment;

    juce::ToggleButton harmonizationChekbox;

    PalmutesLookAndFeel palmutesLookAndFeel;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PalmutesAudioProcessor& audioProcessor;

    const uint16_t WINDOW_RATIO_X = 16;
    const uint16_t WINDOW_RATIO_Y = 7;
    const uint16_t WINDOW_RATIO_MULTIPLIER = 50;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PalmutesAudioProcessorEditor)
};
