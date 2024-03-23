/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <JuceHeader.h>

//==============================================================================
PalmutesAudioProcessor::PalmutesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    this->synth.setup();

    // setup gain
    this->addParameter(gainParamter = new juce::AudioParameterFloat(
        "gain",
        "Gain",
        0.f,
        5.f,
        .5f
    ));

    // setup attack time
    this->addParameter(attackTime = new juce::AudioParameterFloat(
        "attack",
        "Attack Time",
        this->minTime,
        this->maxTime,
        this->defaultTime
    ));

    // setup release time
    this->addParameter(releaseTime= new juce::AudioParameterFloat(
        "release",
        "Release Time",
        this->minTime,
        this->maxTime,
        this->defaultTime
    ));

    this->addParameter(preGainParam = new juce::AudioParameterFloat(
        "pregain",
        "Pre-gain",
        0.f,
        50.f,
        30.f
    ));
}

PalmutesAudioProcessor::~PalmutesAudioProcessor()
{
}

//==============================================================================
const juce::String PalmutesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PalmutesAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PalmutesAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PalmutesAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PalmutesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PalmutesAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PalmutesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PalmutesAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PalmutesAudioProcessor::getProgramName (int index)
{
    return {};
}

void PalmutesAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PalmutesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);

    // configure spec
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;

    // setup compressor
    // fixing the parameter values here is temporary; later, allow those parameters to be
    // modifiable from the editor TODO
    compressor.setSpec(this->spec);
    compressor.setParams(
        100,
        500,
        -15,
        4
    );

    // configure stero widener. Later, alllow the width to be modifiable from the editor TODO
    stereoWidener.outputChannelCount = getTotalNumOutputChannels();
    stereoWidener.width = 2.5f;

    convolution.prepare(spec);
    convolution.loadImpulseResponse(
        BinaryData::ir_two_wav,
        BinaryData::ir_two_wavSize,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0
    );

    distortion.setup(spec);
    distortion.updateParams();

    gate.prepare(spec);
    gate.setAttack(20);
    gate.setRelease(300);
    gate.setRatio(2.f);
    gate.setThreshold(-12.f);

    highPass.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 90.f, 1.f);
    highPass.prepare(spec);

    midBooster.state = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 3400.f, 1.f, 5.f);
    midBooster.prepare(spec);

    lowpass.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 11000.f, 1.f);
    lowpass.prepare(spec);

    _1kBooster.state = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 900.f, 1.f, 1.2f);
    _1kBooster.prepare(spec);

    float presenceEQ = 1.2f;
    presence.state = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 3000.f + presenceEQ * 500.f, 1.f, presenceEQ);
    presence.prepare(spec);
}

void PalmutesAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PalmutesAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PalmutesAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // let keyboardState process before the synth, to allow interaction of the synth
    // from the keyboard component
    this->keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    // let synth play audio
    synth.runtimeSoundConfiguration(midiMessages);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // setup context to be passed in DSP classes
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    // "amp-like" effects
    distortion.process(buffer, context, totalNumOutputChannels);
    convolution.process(context);

    // filtering
    presence.process(context);

    highPass.process(context);
    lowpass.process(context);
    
    _1kBooster.process(context);
    midBooster.process(context);

    // other effects
    gate.process(context);
    stereoWidener.process(buffer);
    compressor.process(buffer);

    buffer.applyGain(*gainParamter);
}

//==============================================================================
bool PalmutesAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PalmutesAudioProcessor::createEditor()
{
    return new PalmutesAudioProcessorEditor (*this);
}

//==============================================================================
void PalmutesAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream(destData, true).writeFloat(*gainParamter);
    juce::MemoryOutputStream(destData, true).writeFloat(*attackTime);
    juce::MemoryOutputStream(destData, true).writeFloat(*releaseTime);
    juce::MemoryOutputStream(destData, true).writeFloat(*preGainParam);
}

void PalmutesAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    *gainParamter = juce::MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *attackTime = juce::MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *releaseTime = juce::MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *preGainParam = juce::MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
}

void PalmutesAudioProcessor::reset()
{
    convolution.reset();
    gate.reset();
    highPass.reset();
    midBooster.reset();
    lowpass.reset();
    _1kBooster.reset();
    presence.reset();

    compressor.reset();
    distortion.reset();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PalmutesAudioProcessor();
}
