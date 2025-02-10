#pragma once

#include <JuceHeader.h>
#include "HttpClient.h"
#include "SliderComponent.h"

class JsonApiComponent : public juce::Component,
                        public juce::Thread
{
public:
    JsonApiComponent(std::unique_ptr<HttpClient> client = std::make_unique<RealHttpClient>())
        : Thread("HTTPThread")
        , httpClient(std::move(client))
    {
        // Add a button to trigger the request
        addAndMakeVisible(requestButton);
        requestButton.setButtonText("Make Request");
        requestButton.onClick = [this]() {
            startThread(); 
        };

        // Add a text editor to display results
        addAndMakeVisible(resultText);
        resultText.setMultiLine(true);
        resultText.setReadOnly(true);
        resultText.setCaretVisible(false);

        // Add the slider component
        addAndMakeVisible(sliderComponent);
    }

    ~JsonApiComponent() override
    {
        stopThread(4000);
    }

    void resized() override
    {
        auto area = getLocalBounds();
        
        #if JUCE_IOS
        // Account for iOS safe area at the top
        if (auto* display = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay())
        {
            auto safeInsets = display->safeAreaInsets;
            area.removeFromTop(safeInsets.getTop());
            area.removeFromBottom(safeInsets.getBottom());
        }
        #endif

        // Reserve space for the request button at the top
        requestButton.setBounds(area.removeFromTop(30));

        // Reserve space for the slider at the bottom (adjust height as needed)
        auto sliderHeight = 50;
        sliderComponent.setBounds(area.removeFromBottom(sliderHeight));

        // Use the remaining area for the result text
        resultText.setBounds(area);
    }


    void run() override
    {
        try {
            auto response = httpClient->makeGetRequest("https://jsonplaceholder.typicode.com/todos/1");
            
            auto jsonResponse = juce::JSON::parse(response);

            juce::String formattedResponse;
            if (auto* obj = jsonResponse.getDynamicObject())
            {
                formattedResponse = "Received JSON:\n\n";
                for (auto& prop : obj->getProperties())
                {
                    formattedResponse += prop.name.toString() + ": " + 
                                       prop.value.toString() + "\n";
                }
            }
            else
            {
                formattedResponse = "Invalid JSON response:\n" + response;
            }
            
            juce::MessageManager::callAsync([this, formattedResponse = std::move(formattedResponse)]() {
                resultText.setText(formattedResponse);
            });
        }
        catch (const std::exception& e) {
            juce::MessageManager::callAsync([this]() {
                resultText.setText("Failed to connect to the server!");
            });
        }
    }

    juce::TextButton requestButton;
    juce::TextEditor resultText;
    SliderComponent sliderComponent;


private:
    std::unique_ptr<HttpClient> httpClient;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JsonApiComponent)
};