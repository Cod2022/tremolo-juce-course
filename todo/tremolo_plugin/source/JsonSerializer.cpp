namespace tremolo {
namespace {
struct SerializableParameters {
	float rate;
	bool bypassed;
	juce::String waveform;
	// TODO: add gain parameter to serialize

	static constexpr auto marshallingVersion = 1;

	template <typename Archive, typename T>
	static void serialise(Archive& archive, T& t) {
		using namespace juce;
		if (archive.getVersion() != 1) {
			return;
		}

		// TREMOLO_PLUGIN_NAME is a preprocessor definition specified in the CMakeLists.txt
		archive(named("pluginName", TREMOLO_PLUGIN_NAME));
		archive(named("modulationRateHz", t.rate), named("bypassed", t.bypassed),
			named("modulationWaveform", t.waveform));
	
	}
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
	const auto json = juce::ToVar::convert(parametersToSerialize);
	// abort serialization if the convertion fails (json does not have value)
	if (!json.has_value()) {
		return;
	}

	juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}
								.withSpacing(juce::JSON::Spacing::multiLine)
								.withMaxDecimalPlaces(2));

	
}

juce::Result JsonSerializer::deserialize(juce::InputStream& input,
                                         Parameters& parameters) {
  juce::ignoreUnused(input, parameters);

  // TODO: deserialize parameters from the JSON input stream

  return juce::Result::fail("not implemented");
}
}  // namespace tremolo
