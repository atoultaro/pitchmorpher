#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PitchMorpherAudioProcessorEditor::PitchMorpherAudioProcessorEditor (PitchMorpherAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Set up a modern look and feel
    lookAndFeel.setColourScheme({
        juce::Colour(0xff1a1a1a),  // windowBackground
        juce::Colour(0xff2a2a2a),  // widgetBackground
        juce::Colour(0xff3a3a3a),  // menuBackground
        juce::Colour(0xffe0e0e0),  // outline
        juce::Colour(0xffffffff),  // defaultText
        juce::Colour(0xffffffff),  // defaultFill
        juce::Colour(0xff42a2c8),  // highlightedText
        juce::Colour(0xff42a2c8),  // highlightedFill
        juce::Colour(0xffff5050)   // menuText
    });
    setLookAndFeel(&lookAndFeel);
    
    // Configure pitch slider
    pitchSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    pitchSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff42a2c8));
    pitchSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    pitchSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff3a3a3a));
    addAndMakeVisible(pitchSlider);
    
    // Configure mix slider
    mixSlider.setSliderStyle(juce::Slider::LinearVertical);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    mixSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff42a2c8));
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff3a3a3a));
    addAndMakeVisible(mixSlider);
    
    // Configure formant slider
    formantSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    formantSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    formantSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffff5050));
    formantSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    formantSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff3a3a3a));
    addAndMakeVisible(formantSlider);
    
    // Configure latency mode button
    latencyModeButton.setButtonText("Low Latency Mode");
    latencyModeButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    addAndMakeVisible(latencyModeButton);
    
    // Configure labels
    pitchLabel.setText("Pitch Shift", juce::dontSendNotification);
    pitchLabel.setFont(juce::Font(16.0f));
    pitchLabel.setJustificationType(juce::Justification::centred);
    pitchLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(pitchLabel);
    
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setFont(juce::Font(16.0f));
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(mixLabel);
    
    formantLabel.setText("Formant", juce::dontSendNotification);
    formantLabel.setFont(juce::Font(16.0f));
    formantLabel.setJustificationType(juce::Justification::centred);
    formantLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(formantLabel);
    
    // Create parameter attachments
    pitchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, processorRef.PITCH_ID, pitchSlider);
    
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, processorRef.MIX_ID, mixSlider);
    
    formantAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, processorRef.FORMANT_ID, formantSlider);
    
    latencyModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.parameters, processorRef.LATENCY_MODE_ID, latencyModeButton);
    
    // Set the plugin's window size
    setSize (500, 400);
}

PitchMorpherAudioProcessorEditor::~PitchMorpherAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void PitchMorpherAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill the background
    g.fillAll (juce::Colour(0xff1a1a1a));
    
    // Draw a border around the plugin
    g.setColour (juce::Colour(0xff3a3a3a));
    g.drawRect (getLocalBounds(), 2);
    
    // Draw the plugin title
    g.setColour (juce::Colours::white);
    g.setFont (juce::Font(24.0f));
    g.drawText ("PitchMorpher", getLocalBounds().removeFromTop(40), juce::Justification::centred, true);
}

void PitchMorpherAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    
    // Title area
    auto titleArea = area.removeFromTop(40);
    
    // Main controls area
    auto controlsArea = area.reduced(10);
    
    // Pitch section
    auto pitchArea = controlsArea.removeFromLeft(150);
    pitchLabel.setBounds(pitchArea.removeFromTop(30));
    pitchSlider.setBounds(pitchArea.reduced(10));
    
    // Mix section
    auto mixArea = controlsArea.removeFromRight(100);
    mixLabel.setBounds(mixArea.removeFromTop(30));
    mixSlider.setBounds(mixArea.reduced(10));
    
    // Formant section
    auto formantArea = controlsArea.removeFromLeft(150);
    formantLabel.setBounds(formantArea.removeFromTop(30));
    formantSlider.setBounds(formantArea.reduced(10));
    
    // Latency mode button at the bottom
    latencyModeButton.setBounds(area.removeFromBottom(30).reduced(10, 0));
}
