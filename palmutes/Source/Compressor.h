/*
  ==============================================================================

    Compressor.h
    Created: 17 Mar 2024 9:41:27pm
    Author:  USER

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Compressor {
public:
    float _attackTime;
    float _releaseTime;
    float _threshold;
    float _ratio;

    juce::dsp::Compressor<float> _compressor;

    void reset() 
    {
        _compressor.reset();
    }

    void setSpec(juce::dsp::ProcessSpec spec) 
    { 
        _compressor.prepare(spec); 
    }

    void setParams(float attackTime, float releaseTime,
                   float threshold, float ratio)
    {
        _attackTime = attackTime;
        _releaseTime = releaseTime;
        _threshold = threshold;
        _ratio = ratio;

        _compressor.setAttack(_attackTime);
        _compressor.setRelease(_releaseTime);
        _compressor.setRatio(_ratio);
        _compressor.setThreshold(_threshold);
    }

    void process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block{ buffer };
        _compressor.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
};