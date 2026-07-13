#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class Coordinates : public juce::Component {
public:
  Coordinates() { setBounds(0, 0, 400, 400); }

  void paint(juce::Graphics& g) override {
    g.setColour(juce::Colours::red);
    g.fillRect(rect1);
    g.setColour(juce::Colours::blue);
    g.fillRect(rect2);
	g.setColour(juce::Colours::white);
	g.fillEllipse(ellipse);
  }

  void resized() override {
    const auto bounds = getLocalBounds().toFloat();
    const auto halfHeight = bounds.getHeight() / 2.f;
	const auto halfWidth = bounds.getWidth() / 2.f;

    rect1 = juce::Rectangle<float>{bounds.getX(), bounds.getY(),
                                   bounds.getWidth(), halfHeight};
    rect2 = juce::Rectangle<float>{bounds.getX(), bounds.getY() + halfHeight,
                                   bounds.getWidth(), halfHeight};

	const auto circleDiameter = std::min(halfHeight, halfWidth);
	ellipse = juce::Rectangle<float>{halfWidth, halfHeight, 
									 circleDiameter, circleDiameter};
	ellipse.setCentre(halfWidth, halfHeight);
  }

private:
  juce::Rectangle<float> rect1;
  juce::Rectangle<float> rect2;
  juce::Rectangle<float> ellipse;
};

using MainComponent = Coordinates;
