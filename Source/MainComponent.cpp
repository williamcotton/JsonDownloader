#include "MainComponent.h"
#include "JsonApiComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Create and add the JsonApiComponent
    jsonApiComponent = std::make_unique<JsonApiComponent>();
    addAndMakeVisible(jsonApiComponent.get());

    setSize(600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // Make the JsonApiComponent fill the entire window
    jsonApiComponent->setBounds(getLocalBounds());
}
