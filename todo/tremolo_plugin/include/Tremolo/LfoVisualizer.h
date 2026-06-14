#pragma once

namespace tremolo {
class LfoVisualizer : public juce::Component {
public:
	void paint (juce::Graphics& g) override {
		// class which specifies the type of the curves to draw
		juce::Path sine;

		const auto strokeWidth = 4.f;
		const auto halfStrokeWidth = static_cast<int>(strokeWidth) / 2;
		const auto halfHeight = getHeight() / 2;
		const auto amplitude = halfHeight - strokeWidth / 2; // subtracting the half of the strokeWidth (reducing the amplitude) to leave space for the waveform's peaks

		// x coordinates to fix the "cutting off" of the waveform at the beginning and the end
		const auto startX = 0 - halfStrokeWidth;
		const auto endX = getWidth() + halfStrokeWidth;
	
		// scaling the size of the sine by multiplying it by amplitude (we'll get a "louder" waveform) and shifting it downward by adding the halfHeight
		sine.startNewSubPath(0, halfHeight + amplitude * std::sin(0.f));
		DBG("\n halfHeight: " << halfHeight);

		for (const auto x : std::views::iota(startX, endX)) {
			sine.lineTo(x, halfHeight + amplitude * std::sin(0.1f * x)); // multiplying x by 0.1f to decrease the "frequency" of the waveform (so we actually divide x by 10)
		}
		g.setColour(juce::Colours::orange);
		g.strokePath(sine, juce::PathStrokeType{strokeWidth});
	}
};

} // namespace tremolo
