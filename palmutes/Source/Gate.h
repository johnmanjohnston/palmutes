/*
  ==============================================================================

    Gate.h
    Created: 28 Mar 2024 7:30:47pm
    Author:  USER

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class Gate {
public:
    juce::dsp::NoiseGate<float> _gate;
    
    float _attackTime;
    float _releaseTime;
    float _threshold;
    float _ratio;

    void setSpec(juce::dsp::ProcessSpec spec) { _gate.prepare(spec); }

    void setParams(float attackTime, float releaseTime,
        float threshold, float ratio)
    {
        _attackTime = attackTime;
        _releaseTime = releaseTime;
        _threshold = threshold;
        _ratio = ratio;

        _gate.setAttack(_attackTime);
        _gate.setRatio(_ratio);
        _gate.setRelease(_releaseTime);
        _gate.setThreshold(_threshold);
    }

    void process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block{ buffer };
        _gate.process(juce::dsp::ProcessContextReplacing<float>(block));
    }

    void reset() 
    {
        _gate.reset();
    }
};