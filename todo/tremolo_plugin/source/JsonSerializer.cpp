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
		// We save plugin name into a mutable std::string variable because it should be overriten when deserializing 
		std::string pluginName = TREMOLO_PLUGIN_NAME;

		archive(named("pluginName", pluginName));
		// abort further parsing if plugin name does not match
		if (pluginName != TREMOLO_PLUGIN_NAME) {
			return;
		}
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
	// abort serialization if the convertion fails (JSON does not have value)
	if (!json.has_value()) {
		return;
	}

	juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}
								.withSpacing(juce::JSON::Spacing::multiLine)
								.withMaxDecimalPlaces(2));

	
}

juce::Result JsonSerializer::deserialize(juce::InputStream& input,
                                         Parameters& parameters) {

  // deserialize parameters from the JSON input stream
	juce::var parsedResult;
	const auto result = juce::JSON::parse(input.readEntireStreamAsString(),
		parsedResult);

	if (result.failed()) {
		return result;
	}

	const auto parsedParameters = juce::FromVar::convert<SerializableParameters>(parsedResult);

	if (!parsedParameters.has_value()) {
		return juce::Result::fail("failed to parse parameters from JSON representation");
	}
	// get the index of a waveform because parameters.waveform can not be assigned with a string, it needs the index of a chosen waveform
	const auto modulationWaveformIndex = parameters.waveform.choices.indexOf(
		parsedParameters->waveform);
	parameters.waveform = modulationWaveformIndex;
	parameters.rate = parsedParameters->rate;
	parameters.bypassed = parsedParameters->bypassed;


  return juce::Result::ok();
}
}  // namespace tremolo
