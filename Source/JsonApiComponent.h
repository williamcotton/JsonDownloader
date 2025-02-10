#pragma once

#include <JuceHeader.h>
#include <ranges>
#include "HttpClient.h"
#include "SliderComponent.h"

typedef int TodoItemId;

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

            // Look ma, no loops!
            std::vector<int> numbers = {1, 2, 3, 4, 5};
            auto evens = numbers
                | std::views::filter([](int n) { return n % 2 == 0; })
                | std::views::transform([](int n) { return n * 2; });

            std::ranges::for_each(evens, [](int n) { std::cout << n << " "; });
        };

        // Add a text editor to display results
        addAndMakeVisible(resultText);
        resultText.setMultiLine(true);
        resultText.setReadOnly(true);
        resultText.setCaretVisible(false);

        // Add the slider component
        addAndMakeVisible(sliderComponent);

        sliderComponent.onSlide = [this](float value) {
            std::cout << "Slider value changed to: " << value << std::endl;
            todoItemId = (int)value;
        };

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
            std::string url = "https://jsonplaceholder.typicode.com/todos/" + std::to_string(todoItemId);
            auto response = httpClient->makeGetRequest(url);
            

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
    TodoItemId todoItemId = 1;

  private:
    std::unique_ptr<HttpClient> httpClient;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JsonApiComponent)
};