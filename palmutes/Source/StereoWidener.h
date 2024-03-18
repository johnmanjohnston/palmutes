/*
  ==============================================================================

    StereoWidener.h
    Created: 18 Mar 2024 6:03:55pm
    Author:  USER

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h> 

class SteroWidener {
public:
    float width;
    int outputChannelCount;

    void process(juce::AudioBuffer<float>& buffer) 
    {
        for (int channel = 0; channel < outputChannelCount; ++channel)
        {
            // apply stereo width using equation from https://www.musicdsp.org/en/latest/Effects/256-stereo-width-control-obtained-via-transfromation-matrix.html?highlight=width
            auto* outLeft = buffer.getWritePointer(0);
            auto* outRight = buffer.getWritePointer(1);

            auto scaleCoefficient = width * .5f;

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                auto inLeft = outLeft[sample];
                auto inRight = outRight[sample];

                auto m = (inLeft + inRight) * .5f;
                auto s = (inRight - inLeft) * scaleCoefficient;

                outLeft[sample] = m - s;
                outRight[sample] = m + s;
            }
        }
    }
};