namespace tremolo {
namespace {
struct SerializableParameters {
	float rate;
	bool bypassed;
	juce::String waveform;
};

SerializableParameters from(const tremolo::Parameters& parameters) {
	return {
		.rate = parameters.rate.get(),
		.bypassed = parameters.bypassed.get(),
		.waveform = parameters.waveform.getCurrentChoiceName(),
	};
}
}
void JsonSerializer::serialize(const Parameters& parameters,
                               juce::OutputStream& output) {

  // serialize parameters to the output stream as JSON
	const auto parametersToSerialize = from(parameters);
	
}

juce::Result JsonSerializer::deserialize(juce::InputStream& input,
                                         Parameters& parameters) {
  juce::ignoreUnused(input, parameters);

  // TODO: deserialize parameters from the JSON input stream

  return juce::Result::fail("not implemented");
}
}  // namespace tremolo
