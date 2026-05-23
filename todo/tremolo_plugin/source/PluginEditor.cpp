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
  addAndMakeVisible(lfoVisualizer);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(540, 270);
}

void PluginEditor::resized() {
  const auto bounds = getLocalBounds();
  background.setBounds(bounds);

  DBG("x: " << bounds.getX() << ", y: " << bounds.getY() << ", width: " << bounds.getWidth() << ", height: " << bounds.getHeight());
  DBG(bounds.toString());

  const auto logoArea = juce::Rectangle<int>{16, 16, 105, 24};
  logoLeft.setBounds(logoArea);
  logoCentre.setBounds(logoArea.withX(bounds.getCentreX() - (logoArea.getWidth() / 2)));
  logoRight.setBounds(logoArea.withX(bounds.getRight() - logoArea.getX() - logoArea.getWidth()));

  lfoVisualizer.setBounds({18, 149, 504, 92});
}
}  // namespace tremolo
