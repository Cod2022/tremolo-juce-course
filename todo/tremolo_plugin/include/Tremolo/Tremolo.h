#pragma once

namespace tremolo {
class Tremolo {
public:
  Tremolo() {
	// We can temporarily set frequency to 440Hz just to be able to hear the output of our LFO
	// But for actual LFO we need the range between 1 and 20 HZ
	lfo.setFrequency(5.f, true);
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
	lfo.prepare(processSpec);


  }

  void process(juce::AudioBuffer<float>& buffer) noexcept {
    // for each frame
    for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {
      // generate the LFO value
	  const auto lfoValue = lfo.processSample(0.f); 

      // TODO: calculate the modulation value

      // for each channel sample in the frame
      for (const auto channelIndex :
           std::views::iota(0, buffer.getNumChannels())) {
        // get the input sample
        const auto inputSample = buffer.getSample(channelIndex, frameIndex);

        // Checking our lfoValuue just by outputing it instead of the original input signal
		// So we should hear its sound instead of that sended to our plugin
        const auto outputSample = 0.1f * lfoValue;

        // set the output sample
        buffer.setSample(channelIndex, frameIndex, outputSample);
      }
    }
  }

  void reset() noexcept {
	// Releasing our lfo resources
	lfo.reset();
  }

private:
  // You should put class members and private functions here
  
  // Creating an Oscillator class instance for our LFO
  juce::dsp::Oscillator<float> lfo{ [](auto phase){ return std::sin(phase); }};
};
}  // namespace tremolo
