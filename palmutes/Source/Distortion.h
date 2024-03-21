/*
  ==============================================================================

    Distortion.h
    Created: 21 Mar 2024 5:14:53pm
    Author:  USER

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Distortion {
public:
    juce::dsp::Gain<float> preGain;
    juce::dsp::Gain<float> postGain;
    
    float drive = 1.5f;
    float bias = .5f;

    float preGainValue = 30.f;
    float postGainValue = -15.f;

    void setup(juce::dsp::ProcessSpec spec) 
    {
        preGain.reset();
        postGain.reset();

        preGain.prepare(spec);
        postGain.prepare(spec);
    }

    void updateParams() 
    {
        preGain.setGainDecibels(preGainValue);
        postGain.setGainDecibels(postGainValue);
    }

    void process(   juce::AudioBuffer<float>& buffer, 
                    juce::dsp::ProcessContextReplacing<float> context,
                    int totalNumOutputChannels) 
    {
        preGain.process(context);

        for (int channel = 0; channel < totalNumOutputChannels; ++channel) 
        {
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample) 
            {
                auto* channelData = buffer.getWritePointer(channel);
                channelData[sample] = applyDistortion(channelData[sample]);
            }
        }

        postGain.process(context);
    }

    float applyDistortion(float x) 
    {
        return std::tanh(x * drive + bias);
    }
};