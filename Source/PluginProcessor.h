#pragma once

#include <JuceHeader.h>

//==============================================================================
class PitchMorpherAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PitchMorpherAudioProcessor();
    ~PitchMorpherAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // AudioProcessorValueTreeState for managing parameters
    juce::AudioProcessorValueTreeState parameters;
    
    // Parameter IDs - made public so they can be accessed from the editor
    static const juce::String PITCH_ID;
    static const juce::String MIX_ID;
    static const juce::String FORMANT_ID;
    static const juce::String LATENCY_MODE_ID;

private:
    // Create the parameter layout for AudioProcessorValueTreeState
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    // DSP-related member variables
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
    bool lowLatencyMode = false;
    
    // Buffer for pitch shifting algorithm
    juce::AudioBuffer<float> workingBuffer;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchMorpherAudioProcessor)
};
