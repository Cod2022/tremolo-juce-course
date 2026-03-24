
namespace tremolo {
// using anonymous namespace for the AudioParameterFloat class
// it can be accessed and is visible only in the current .cpp
namespace { 
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
			.withStringFromValueFunction([](float value, int /*maxLen*/) {// reducing the number of difits after the dot in a float number parameter
				return juce::String(value, 2);}));
		// retrieving a reference to it
		auto& parameterReference = *parameter;
		// releasing unique pointer to add the parameter to our processor
		// we need to do that because addParameter deletes the object by itself
		// So would std::make::unique. That`s why we are releasing it to addParameter - to prevent double deleting of the same object
		processor.addParameter(parameter.release());
		return parameterReference;
	}
}
Parameters::Parameters(juce::AudioProcessor& processor)
// TODO: create parameters
// TODO: retrieve references to parameters
// add parameters to the processor
    : rate{createModulationRateParameter(processor)}

{
}
}  // namespace tremolo
