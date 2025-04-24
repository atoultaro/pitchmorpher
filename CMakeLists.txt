cmake_minimum_required(VERSION 3.15)
project(PitchMorpher VERSION 0.0.1)

# Add the JUCE directory (now within the project) as a subdirectory
add_subdirectory(JUCE)

# Add the plugin target
juce_add_plugin(PitchMorpher
    COMPANY_NAME "YourCompany" # Change as needed
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE # Copies to default locations
    PLUGIN_MANUFACTURER_CODE "Manu" # Needs to be unique 4-char code
    PLUGIN_CODE "Ptmr"            # Needs to be unique 4-char code
    AU_MAIN_TYPE kAudioUnitType_Effect # Audio Unit Effect type
    FORMATS AU                    # Build AU format initially
    PRODUCT_NAME "PitchMorpher")

# Generate JuceHeader.h
juce_generate_juce_header(PitchMorpher)

# Specify C++ standard
target_compile_features(PitchMorpher PUBLIC cxx_std_17)

# Link JUCE modules
target_link_libraries(PitchMorpher PUBLIC
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Add source files (CMake will create placeholders if they don't exist)
target_sources(PitchMorpher PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

# Define where the source files are relative to this CMakeLists.txt
target_include_directories(PitchMorpher PUBLIC Source)
