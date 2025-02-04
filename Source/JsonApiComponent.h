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

        // Create JSON object for request body (if needed)
        juce::var jsonBody;
        jsonBody.getDynamicObject()->setProperty("key", "value");
        
        // Convert JSON to string
        juce::String jsonString = juce::JSON::toString(jsonBody);

        // First set the POST data on the URL
        url = url.withPOSTData(jsonString);

        // Create input stream options
        juce::URL::InputStreamOptions options(juce::URL::ParameterHandling::inPostData);
        options.withConnectionTimeoutMs(10000)
               .withExtraHeaders(headerString)
               .withHttpRequestCmd("POST");

        // Create HTTP stream
        std::unique_ptr<juce::InputStream> stream(url.createInputStream(options));

        if (stream != nullptr)
        {
            // Read the response
            juce::String response = stream->readEntireStreamAsString();
            
            // Parse JSON response
            juce::var jsonResponse = juce::JSON::parse(response);
            
            // Update UI on the message thread
            juce::MessageManager::callAsync([this, response]()
            {
                resultText.setText(response);
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