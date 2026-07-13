namespace tremolo {
PluginEditor::PluginEditor(PluginProcessor& p) : AudioProcessorEditor(&p) {
  background.setImage(juce::ImageCache::getFromMemory(
      assets::Background_png, assets::Background_pngSize));

  logoLeft.setImage(
      juce::ImageCache::getFromMemory(assets::Logo_png, assets::Logo_pngSize));

  logoCentre.setImage(
	  juce::ImageCache::getFromMemory(assets::Logo_png, assets::Logo_pngSize));

  logoRight.setImage(
	  juce::ImageCache::getFromMemory(assets::Logo_png, assets::Logo_pngSize));

  addAndMakeVisible(background);
  addAndMakeVisible(logoLeft);
  addAndMakeVisible(logoCentre);
  addAndMakeVisible(logoRight);

  rateSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  rateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  rateSlider.setPopupDisplayEnabled(true, false, this);
  rateSlider.setRange(1.0, 30.0, 0.5);
  rateSlider.onValueChange = [this] {
	  DBG("Rate slider value: " << rateSlider.getValue());
	  };
  rateSlider.setTextValueSuffix(" Hz");
  addAndMakeVisible(rateSlider);

  waveformWidthSlider.setRange(1.0, 10.0, 0.5);
  waveformWidthSlider.setValue(lfoVisualizer.getStrokeWidth(), juce::dontSendNotification); // set the default value of the slider upon creation
  waveformWidthSlider.onValueChange = [this] {
		lfoVisualizer.setStrokeWidth(waveformWidthSlider.getValue());
	  };
  addAndMakeVisible(waveformWidthSlider);

  addAndMakeVisible(lfoVisualizer);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(540, 300);
}

void PluginEditor::resized() {
  auto bounds = getLocalBounds();
  bounds.removeFromBottom(30);
  background.setBounds(bounds);

  DBG("x: " << bounds.getX() << ", y: " << bounds.getY() << ", width: " << bounds.getWidth() << ", height: " << bounds.getHeight());
  DBG(bounds.toString());

  const auto logoArea = juce::Rectangle<int>{16, 16, 105, 24};
  logoLeft.setBounds(logoArea);
  logoCentre.setBounds(logoArea.withX(bounds.getCentreX() - (logoArea.getWidth() / 2)));
  logoRight.setBounds(logoArea.withX(bounds.getRight() - logoArea.getX() - logoArea.getWidth()));

  auto rateSliderBounds = bounds;
  rateSliderBounds.removeFromLeft(230);
  rateSliderBounds.removeFromRight(230);
  rateSliderBounds.removeFromTop(40);
  rateSliderBounds.removeFromBottom(150);
  rateSlider.setBounds(rateSliderBounds);

  auto waveformWidthSliderBounds = getLocalBounds();
  waveformWidthSliderBounds.removeFromTop(270);
  waveformWidthSliderBounds.removeFromRight(230);
  waveformWidthSlider.setBounds(waveformWidthSliderBounds);
  
  // set bounds for our LfoVisualizer
  lfoVisualizer.setBounds({18, 149, 504, 92});
}
}  // namespace tremolo
