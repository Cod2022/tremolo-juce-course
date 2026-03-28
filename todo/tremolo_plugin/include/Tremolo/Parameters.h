#pragma once

namespace tremolo {
struct Parameters {
  explicit Parameters(juce::AudioProcessor&);

  juce::AudioParameterFloat& rate;
  juce::AudioParameterFloat& gain;

  // because of the reference member (juce::AudioParameterFloat& rate),
  // we use this macro to avoid accidental copies or moves of the Parameters struct
  // it prevents copying instances of this class
  JUCE_DECLARE_NON_COPYABLE(Parameters);
  // and this macro prevents moving the Parameters class
  JUCE_DECLARE_NON_MOVEABLE(Parameters);
};
}  // namespace tremolo
