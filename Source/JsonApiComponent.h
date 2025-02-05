#pragma once

#include <JuceHeader.h>

class JsonApiComponent : public juce::Component,
                        public juce::Thread
{
public:
    JsonApiComponent() : Thread("HTTPThread")
    {
        // Add a button to trigger the request
        addAndMakeVisible(requestButton);
        requestButton.setButtonText("Make Request");
        requestButton.onClick = [this]() { startThread(); };

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
        // Create URL object for the API endpoint
        juce::URL url("https://jsonplaceholder.typicode.com/todos/1");

        // Set up headers if needed
        juce::String headerString;
        headerString << "Content-Type: application/json\r\n"
                    << "Accept: application/json\r\n";

        // For this GET request, we don't need to send POST data
        // Create input stream options
        juce::URL::InputStreamOptions options(juce::URL::ParameterHandling::inAddress);
        auto opt = options.withConnectionTimeoutMs(10000)
               .withExtraHeaders(headerString)
               .withHttpRequestCmd("GET");  // Changed to GET since we're fetching data

        // Create HTTP stream
        std::unique_ptr<juce::InputStream> stream(url.createInputStream(options));

        if (stream != nullptr)
        {
            // Read the response
            juce::String response = stream->readEntireStreamAsString();
            
            // Parse JSON response
            juce::var jsonResponse = juce::JSON::parse(response);
            
            // Format the response nicely
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
            
            // Update UI on the message thread
            juce::MessageManager::callAsync([this, formattedResponse]()
            {
                resultText.setText(formattedResponse);
            });
        }
        else
        {
            juce::MessageManager::callAsync([this]()
            {
                resultText.setText("Failed to connect to the server!");
            });
        }
    }

private:
    juce::TextButton requestButton;
    juce::TextEditor resultText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JsonApiComponent)
};