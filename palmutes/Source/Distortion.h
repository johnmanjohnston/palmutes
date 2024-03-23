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
    
    float drive = 1.2f;
    //float bias = .5f;

    float preGainValue = 30.f;
    float postGainValue = -7.f;

    void reset() 
    {
        preGain.reset();
        postGain.reset();
    }

    void setup(juce::dsp::ProcessSpec spec) 
    {
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
        x = std::atan(x * drive);
        //x = std::tanh(x * drive + bias);
        //x = 1.5f * x - 0.5f * pow(x, 3);

        return x;
    }
};