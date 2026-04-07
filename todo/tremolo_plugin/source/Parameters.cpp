
namespace tremolo {
// using anonymous namespace for the AudioParameterFloat class
// it can be accessed and is visible only in the current .cpp
namespace {
	// template function to add any parameter to the processor
	auto& addParameterToProcessor(juce::AudioProcessor& processor, auto parameter) {
		// retrieving a reference to a parameter
		auto& parameterReference = *parameter;
		// releasing unique pointer to add the parameter to our processor
		// we need to do that because addParameter deletes the object by itself
		// So would std::make::unique. That`s why we are releasing it to addParameter - to prevent double deleting of the same object
		processor.addParameter(parameter.release());
		return parameterReference;
	}
	// Function to create a parameter. The general flow is as follows:
	// 1. Allocate memory for it
	// 2. Get the reference to it
	// 3. Add this parameter to the AudioProcessor
	// 4. Return the reference
	juce::AudioParameterFloat& createModulationRateParameter(juce::AudioProcessor& processor) {
		constexpr auto versionHint = 1;
		// allocating memory for our AudioParameterFloat class
		auto parameter = std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{"modulation.rate", versionHint},
			"Modulation rate",
			juce::NormalisableRange<float>{0.1f, 20.f, 0.0f, 0.4f},
			5.f,
			juce::AudioParameterFloatAttributes{}
			.withLabel("Hz")
			.withStringFromValueFunction([](float value, int /*maxLen*/) {// reducing the number of digits after the decimal in a float number parameter
				return juce::String(value, 1);}));
		return addParameterToProcessor(processor, std::move(parameter));	
	}

	juce::AudioParameterFloat& createGainParameter(juce::AudioProcessor& processor) {
		constexpr auto versionHint = 1;
		auto parameter = std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{"gain", versionHint},
			"Gain",
			juce::NormalisableRange<float>{-12.0f, 12.0f, 0.1f, 1.0f},
			0.0f,
			juce::AudioParameterFloatAttributes{}
			.withLabel("dB")
			.withStringFromValueFunction([](float value, int /*max len*/) {
				// converting from a linear gain value to dB, showing -INF if the value is 0.0f
				//if (value <= 0.0f) return juce::String("-INF");
				//auto db = juce::Decibels::gainToDecibels(value);
				return juce::String(value, 1);
				})
			);
		return addParameterToProcessor(processor, std::move(parameter));
	
	}

	juce::AudioParameterBool& createBypassedParameter(juce::AudioProcessor& processor) {
		constexpr auto versionHint = 1;
		auto parameter = std::make_unique<juce::AudioParameterBool>(
			juce::ParameterID("bypassed", versionHint), 
			"Bypass", 
			false);
		return addParameterToProcessor(processor, std::move(parameter));
	}
}// unnamed namespace
Parameters::Parameters(juce::AudioProcessor& processor)
// TODO: create parameters
// TODO: retrieve references to parameters
// add parameters to the processor
    : rate{createModulationRateParameter(processor)},
      gain{createGainParameter(processor)}, 
	  bypassed{createBypassedParameter(processor)}

{
}
}  // namespace tremolo
