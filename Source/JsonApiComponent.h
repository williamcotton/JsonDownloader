#pragma once

#include <JuceHeader.h>
#include "HttpClient.h"

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
        }
        #endif
        
        requestButton.setBounds(area.removeFromTop(30));
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


private:
    std::unique_ptr<HttpClient> httpClient;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JsonApiComponent)
};