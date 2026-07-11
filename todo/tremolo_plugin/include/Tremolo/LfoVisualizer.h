#pragma once

namespace tremolo {
class LfoVisualizer : public juce::Component {
public:
	void paint (juce::Graphics& g) override {
		g.setColour(juce::Colours::orange);
		g.strokePath(waveformPath, juce::PathStrokeType{strokeWidth});
	}

	void resized() override {
		waveformPath.clear();// clearing old paths

		const int halfStrokeWidth = static_cast<int>(strokeWidth) / 2;
		const int halfHeight = getHeight() / 2;
		const int amplitude = halfHeight - strokeWidth / 2; // subtracting the half of the strokeWidth (reducing the amplitude) to leave space for the waveform's peaks

		// x coordinates to fix the "cutting off" of the waveform at the beginning and the end
		const int startX = 0 - halfStrokeWidth;
		const int endX = getWidth() + halfStrokeWidth;

		// Fetch the correct math rule instantly based on our enum index
		auto getWaveformValue = waveformFormulas[juce::toUnderlyingType(currentWaveform)];

	
		// scaling the size of the waveform by multiplying it by amplitude (we'll get a "louder" waveform) and shifting it downward by adding the halfHeight
		const float startXFloat = static_cast<float>(startX);
		waveformPath.startNewSubPath(startXFloat, halfHeight + amplitude * getWaveformValue(startXFloat));

		for (const auto x : std::views::iota(startX, endX)) {
			const float xFloat = static_cast<float>(x);
			waveformPath.lineTo(xFloat, halfHeight + amplitude * getWaveformValue(xFloat));
		}
	}

	void setWaveform(Tremolo::LfoWaveform newWaveform) {
		if (currentWaveform != newWaveform) {
			currentWaveform = newWaveform;
			resized();
			repaint();
		}
	}
private:
	// class which specifies the type of the curves to draw
	juce::Path waveformPath;
	const float strokeWidth = 4.f;
	Tremolo::LfoWaveform currentWaveform = Tremolo::LfoWaveform::triangle;

	// alias for a waveform math function pointer, allowing uniform storage in a std::array
	using WaveformFormulaPtr = float(*)(float);
    std::array<WaveformFormulaPtr, 2u> waveformFormulas {
		[](float x) { return std::sinf(0.1 * x); }, // multiplying x by 0.1f to decrease the "frequency" of the waveform (so we actually divide x by 10)
		[](float x) { return Tremolo::triangle(0.05f * x + juce::MathConstants<float>::halfPi); } // add a phase offset to shift the starting point to the middle
	};
};
} // namespace tremolo
