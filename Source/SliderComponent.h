/*
  ==============================================================================

    SliderComponent.h
    Created: 10 Feb 2025 7:28:38am
    Author:  administrator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../JuceLibraryCode/BinaryData.h"

using namespace juce;

class CustomSliderLookAndFeel : public LookAndFeel_V2 {
public:
  CustomSliderLookAndFeel() {
    setColour(Slider::thumbColourId, Colours::purple);
  }

  void drawLinearSlider(Graphics &g, int x, int y, int width, int height,
                        float sliderPos, float minSliderPos, float maxSliderPos,
                        const Slider::SliderStyle style,
                        Slider &slider) override {
    // Draw track
    if (style == Slider::LinearVertical)
      g.fillRect(x + width / 2 - 2, y, 4, height);
    else
      g.fillRect(x, y + height / 2 - 2, width, 4);

    // Draw thumb
    if (customThumb != nullptr) {
      const float thumbWidth = 40.0f;
      const float thumbHeight = 40.0f;

      if (style == Slider::LinearVertical) {
        float thumbY = sliderPos - thumbHeight / 2;
        Rectangle<float> thumbArea(x, thumbY, (float)width, thumbHeight);
        customThumb->drawWithin(g, thumbArea, RectanglePlacement::centred,
                                1.0f);
      } else {
        float thumbX = sliderPos - thumbWidth / 2;
        Rectangle<float> thumbArea(thumbX, y, thumbWidth, (float)height);
        customThumb->drawWithin(g, thumbArea, RectanglePlacement::centred,
                                1.0f);
      }
    }
  }

  int getSliderThumbRadius(Slider &slider) override { return 20; }

  void setCustomThumb(const Image &newThumb) {
    customThumb = std::make_unique<DrawableImage>();
    customThumb->setImage(newThumb);
  }

private:
  std::unique_ptr<DrawableImage> customThumb;
};

class SliderComponent : public juce::Component {
public:
  SliderComponent() {
    addAndMakeVisible(slider);
    customLookAndFeel.setCustomThumb(ImageCache::getFromMemory(
        BinaryData::sliderKnob_png, BinaryData::sliderKnob_pngSize));
    
    slider.setLookAndFeel(&customLookAndFeel);
  } 

  void resized() override {
    slider.setBounds(getLocalBounds());
  }

  CustomSliderLookAndFeel customLookAndFeel;


  Slider slider;

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderComponent)
};