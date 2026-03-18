#pragma once

namespace tremolo {
class Tremolo {
public:
  enum class LfoWaveform : size_t {
	sine = 0,
	triangle = 1, 
  };
  Tremolo() {
	// We can temporarily set frequency to 440Hz just to be able to hear the output of our LFO
	// But for actual LFO we need the range between 1 and 20 HZ
	// Second argument "true" means that we can change the initial frequency of the oscillator
	for (auto& lfo : lfos) {
		lfo.setFrequency(5.f, true);
	}
  }
  void prepare(double sampleRate, int expectedMaxFramesPerBlock) {

	// This is the Structure containing info about sampleRate, BlockSize and numChannels
	// Many functions of juce_dsp classes take this structure as an argument
	const juce::dsp::ProcessSpec processSpec {
		.sampleRate = sampleRate,
		.maximumBlockSize = static_cast<juce::uint32>(expectedMaxFramesPerBlock),
		.numChannels = 1u,
	};
	// Like this one for example
	for (auto& lfo : lfos) {
		lfo.prepare(processSpec);
	}
  }

  void setLfoWaveform(LfoWaveform waveform) {
	// using jassert to perform a rintime check (in case user passes value more than 0 or 1)
	jassert(waveform == LfoWaveform::sine || waveform == LfoWaveform::triangle);
	lfoToSet = waveform;
  }

  void process(juce::AudioBuffer<float>& buffer) noexcept {
	updateLfoWaveform();
    // for each frame
    for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {
      // generate the LFO value
      const auto lfoValue = getNextLfoValue();

	  // set the modulation depth
	  constexpr auto modulationDepth = 0.4f;
      // calculate the modulation value
	  const auto modulationValue = modulationDepth * lfoValue + 1.f;

	  // if we don`t normalize lfoValue by adding +1 to it, we will get ring modulation effect
	  //const auto modulationValue = modulationDepth * lfoValue;
	
      // for each channel sample in the frame
      for (const auto channelIndex :
           std::views::iota(0, buffer.getNumChannels())) {
        // get the input sample
        const auto inputSample = buffer.getSample(channelIndex, frameIndex);

        // Applying modulation to the current input sample just by multiplying it by modulationValue;
        const auto outputSample = inputSample * modulationValue;

        // set the output sample
        buffer.setSample(channelIndex, frameIndex, outputSample);
      }
    }
  }

  void reset() noexcept {
	// Releasing our lfo resources
	for (auto& lfo : lfos) {
		lfo.reset();
	}
  }

private:
  // You should put class members and private functions here
  
  // function for building a triangle waveform using the formula for the triangle waveform
  static float triangle(float phase) {
	const auto ft = phase / juce::MathConstants<float>::twoPi;// calculating "theta/2Pi" part of the formula first
	return 4.f * std::abs(ft - std::floor(ft + 0.5f)) - 1.f;// applying the whole formula using the previous calculation in it
	}
  // using toUnderlyingType instead of static_cast to cast Enum class to size_t type for using it as an index of our array lfos
  float getNextLfoValue() {
    return lfos[juce::toUnderlyingType(currentLfo)].processSample(0.f);
  }
  // TODO: implement LFO waveshape change smoothing
  // Handler function to update LFO waveform
  void updateLfoWaveform() {
	if (currentLfo != lfoToSet) {
		currentLfo = lfoToSet;
	}
  }
  
  // Creating an Oscillator class instance for our LFO
  std::array<juce::dsp::Oscillator<float>, 2u> lfos {
      juce::dsp::Oscillator<float>{[](auto phase) { return std::sin(phase); }},
	  juce::dsp::Oscillator<float>{triangle},
  };
  // Creating a Enum instance for sine LFO
  LfoWaveform currentLfo = LfoWaveform::sine;
  // Creating handler to change LFO waveform (if we do it in the currentLfo variable we might have clicks)
  LfoWaveform lfoToSet = currentLfo;

};
}  // namespace tremolo
