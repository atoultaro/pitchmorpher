#include "PluginProcessor.h"
#include "PluginEditor.h"

// Define static parameter IDs
const juce::String PitchMorpherAudioProcessor::PITCH_ID = "pitch";
const juce::String PitchMorpherAudioProcessor::MIX_ID = "mix";
const juce::String PitchMorpherAudioProcessor::FORMANT_ID = "formant";
const juce::String PitchMorpherAudioProcessor::LATENCY_MODE_ID = "latency_mode";

//==============================================================================
PitchMorpherAudioProcessor::PitchMorpherAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
       parameters (*this, nullptr, "PitchMorpher", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout PitchMorpherAudioProcessor::createParameterLayout()
{
    // Create the parameter layout for our plugin
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Pitch shift parameter: -24 to +24 semitones
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        PITCH_ID,                  // Parameter ID
        "Pitch Shift",             // Parameter name
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f), // Range: -24 to +24 semitones with 0.01 step
        0.0f,                      // Default value (no pitch shift)
        "semitones",               // Label
        juce::AudioProcessorParameter::genericParameter, // Category
        [](float value, int) {     // String from value function
            // Format with sign and 2 decimal places
            return juce::String (value, 2) + " st";
        }
    ));
    
    // Wet/Dry mix parameter: 0% to 100%
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        MIX_ID,                    // Parameter ID
        "Mix",                     // Parameter name
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f), // Range: 0% to 100% with 0.1% step
        100.0f,                    // Default value (100% wet)
        "%",                       // Label
        juce::AudioProcessorParameter::genericParameter, // Category
        [](float value, int) {     // String from value function
            return juce::String (int (value)) + "%";
        }
    ));
    
    // Formant control parameter: -12 to +12 semitones
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        FORMANT_ID,                // Parameter ID
        "Formant",                 // Parameter name
        juce::NormalisableRange<float> (-12.0f, 12.0f, 0.01f), // Range: -12 to +12 semitones with 0.01 step
        0.0f,                      // Default value (no formant shift)
        "semitones",               // Label
        juce::AudioProcessorParameter::genericParameter, // Category
        [](float value, int) {     // String from value function
            return juce::String (value, 2) + " st";
        }
    ));
    
    // Low latency mode toggle
    layout.add (std::make_unique<juce::AudioParameterBool> (
        LATENCY_MODE_ID,           // Parameter ID
        "Low Latency Mode",        // Parameter name
        false                      // Default value (high quality mode)
    ));
    
    return layout;
}

PitchMorpherAudioProcessor::~PitchMorpherAudioProcessor()
{
}

//==============================================================================
const juce::String PitchMorpherAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PitchMorpherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PitchMorpherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PitchMorpherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PitchMorpherAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PitchMorpherAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PitchMorpherAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PitchMorpherAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PitchMorpherAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PitchMorpherAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PitchMorpherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Store sample rate and block size for later use
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    
    // Initialize working buffer for pitch shifting
    workingBuffer.setSize(2, samplesPerBlock * 4); // Extra space for overlap
    workingBuffer.clear();
    
    // Check if we're in low latency mode
    auto* latencyParam = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter(LATENCY_MODE_ID));
    if (latencyParam != nullptr)
        lowLatencyMode = latencyParam->get();
    
    // Set latency based on mode
    if (lowLatencyMode)
        setLatencySamples(0); // No latency for live use
    else
        setLatencySamples(samplesPerBlock); // One buffer of latency for better quality
}

void PitchMorpherAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PitchMorpherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void PitchMorpherAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Get parameter values
    auto* pitchParam = parameters.getRawParameterValue(PITCH_ID);
    auto* mixParam = parameters.getRawParameterValue(MIX_ID);
    auto* formantParam = parameters.getRawParameterValue(FORMANT_ID);
    auto* latencyModeParam = parameters.getRawParameterValue(LATENCY_MODE_ID);
    
    // Extract values
    float pitchShift = pitchParam->load();
    float wetDryMix = mixParam->load() / 100.0f; // Convert from percentage to 0-1 range
    float formantShift = formantParam->load();
    bool isLowLatencyMode = latencyModeParam->load() > 0.5f;
    
    // Update latency if mode changed
    if (isLowLatencyMode != lowLatencyMode)
    {
        lowLatencyMode = isLowLatencyMode;
        if (lowLatencyMode)
            setLatencySamples(0);
        else
            setLatencySamples(currentBlockSize);
    }
    
    // Skip processing if pitch shift is zero and mix is 100% wet
    if (std::abs(pitchShift) < 0.01f && wetDryMix > 0.99f)
        return;
    
    // Create a copy of the original buffer for wet/dry mixing
    juce::AudioBuffer<float> dryBuffer;
    if (wetDryMix < 0.99f) // Only copy if we need to mix in some dry signal
    {
        dryBuffer.makeCopyOf(buffer);
    }
    
    // Simple pitch shifting implementation (placeholder)
    // In a real implementation, this would be replaced with a proper phase vocoder
    // or other high-quality pitch shifting algorithm
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        // Very basic pitch shifting using simple resampling
        // This is just a placeholder and will have artifacts
        if (std::abs(pitchShift) > 0.01f)
        {
            // Calculate pitch ratio (2^(semitones/12))
            float ratio = std::pow(2.0f, pitchShift / 12.0f);
            
            // Simple resampling (will have artifacts, just for demonstration)
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                float position = i / ratio;
                int pos1 = static_cast<int>(position);
                int pos2 = pos1 + 1;
                float frac = position - pos1;
                
                // Simple linear interpolation
                if (pos2 < buffer.getNumSamples())
                {
                    float sample1 = channelData[pos1];
                    float sample2 = channelData[pos2];
                    channelData[i] = sample1 + frac * (sample2 - sample1);
                }
                else
                {
                    // If we run out of samples, just use the last valid one
                    channelData[i] = channelData[pos1];
                }
            }
        }
    }
    
    // Apply wet/dry mix if needed
    if (wetDryMix < 0.99f)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* wetData = buffer.getWritePointer(channel);
            auto* dryData = dryBuffer.getReadPointer(channel);
            
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                wetData[i] = wetData[i] * wetDryMix + dryData[i] * (1.0f - wetDryMix);
            }
        }
    }
}

//==============================================================================
bool PitchMorpherAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PitchMorpherAudioProcessor::createEditor()
{
    return new PitchMorpherAudioProcessorEditor (*this);
}

//==============================================================================
void PitchMorpherAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save the parameter state using AudioProcessorValueTreeState
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PitchMorpherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore the parameter state using AudioProcessorValueTreeState
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchMorpherAudioProcessor();
}
