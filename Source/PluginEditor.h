#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class PitchMorpherAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit PitchMorpherAudioProcessorEditor (PitchMorpherAudioProcessor&);
    ~PitchMorpherAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PitchMorpherAudioProcessor& processorRef;
    
    // UI Components
    juce::Slider pitchSlider;
    juce::Slider mixSlider;
    juce::Slider formantSlider;
    juce::ToggleButton latencyModeButton;
    
    // Labels for the sliders
    juce::Label pitchLabel;
    juce::Label mixLabel;
    juce::Label formantLabel;
    
    // Parameter attachments to link UI controls with parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> formantAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> latencyModeAttachment;
    
    // Custom LookAndFeel for the sliders
    juce::LookAndFeel_V4 lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchMorpherAudioProcessorEditor)
};
