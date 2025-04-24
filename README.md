# 🎛️ PitchMorpher

## 1. Purpose

PitchMorpher is a plugin that provides musicians, producers, and sound designers with a simple and efficient tool to alter the pitch of an audio signal in real time without changing its tempo or duration. Ideal for vocal pitch correction, instrument tuning, creative effects, or harmonizing.

## 2. Target Audience

- Music producers (bedroom to pro)
- Vocalists and remixers
- Podcasters / content creators
- DJs and live performers
- Sound designers for film/game audio

## 3. Platform Support

- Initial release: macOS Audio Unit (AUv2) for Logic Pro
- Optional future support: VST3, CLAP, and AAX
- JUCE Starter License will be used for development

## 4. Core Features

| Feature | Description |
|--------|-------------|
| 🔼🔽 Pitch Shifting | Adjust pitch ±24 semitones with smooth, real-time control. |
| ⏱️ Time Preservation | Maintains original time/tempo using phase vocoder or elastique-style algorithm. |
| 🎚️ Wet/Dry Mix | Blend between original and pitch-shifted signal (0–100%). |
| 🎛️ Formant Control (Optional) | Adjust formants to preserve vocal character when pitch-shifting. |
| 💻 Low Latency Mode | Toggle for use in live settings with slight quality trade-off. |
| 📈 Visualization (Optional) | Simple waveform or spectral view showing pitch shift effect in real time. |

## 5. User Interface

- Clean, minimalist single-screen layout
- Controls:
  - Pitch Slider / Knob (±24 semitones or cents)
  - Formant Slider (optional)
  - Wet/Dry Mix
  - Latency/Quality toggle
- UI scaling for retina displays
- Host automation support (Logic, Ableton, etc.)

## 6. Technical Requirements

| Area | Details |
|------|---------|
| Language | C++ with JUCE Framework |
| Plugin Formats | AU (initial), optional VST3/AAX/CLAP |
| Audio Engine | Real-time pitch shifting via phase vocoder or WSOLA-like approach |
| Sample Rates | Support for 44.1kHz – 96kHz |
| CPU Usage | Target under 5% at 44.1kHz on Apple M1 or Intel i7 |
| Latency | Toggle between zero-latency (lower quality) and high-quality (with small buffer) |

## 7. Out-of-Scope (for MVP)

- No pitch detection/tuning correction (e.g. not like Auto-Tune)
- No tempo changes or time-stretching independently
- No MIDI input or sidechain input in v1
- No multi-track/harmonizer engine

## 8. Success Metrics

- Functional and stable in Logic Pro (AU)
- Latency below 20ms in low-latency mode
- Pitch shifting accuracy within ±2 cents
- Smooth automation and parameter changes with no zipper noise
- Good feedback from at least 10 beta users in music/audio production

## 9. Monetization Strategy

- Starter Version (Free): Pitch shift up to ±5 semitones
- Pro Version ($29–49): Full ±24 semitones, formant shifting, low-latency toggle
- Distribution via own site, KVR, Plugin Boutique, and/or Gumroad

## 10. Timeline (MVP)

| Phase | Deliverable | Duration |
|-------|-------------|----------|
| 1. Research | Choose DSP algorithm | 1 week |
| 2. Prototype | Working pitch shifter with CLI | 2 weeks |
| 3. Plugin Integration | AU plugin using JUCE | 3 weeks |
| 4. UI Design | Basic UI with pitch slider | 1 week |
| 5. Beta Testing | Private testers + feedback | 2 weeks |
| 6. Launch | Upload to marketplaces | 1 week |
"""
