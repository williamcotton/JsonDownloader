#pragma once
#include <JuceHeader.h>

class HttpClient {
public:
    virtual ~HttpClient() = default;
    virtual juce::String makeGetRequest(const juce::String& url) = 0;
};

class RealHttpClient : public HttpClient {
public:
    juce::String makeGetRequest(const juce::String& url) override {
        juce::URL urlObj(url);

        juce::String headerString;
        headerString << "Content-Type: application/json\r\n"
                     << "Accept: application/json\r\n";

        juce::URL::InputStreamOptions options(juce::URL::ParameterHandling::inAddress);

        auto opt = options.withConnectionTimeoutMs(10000)
            .withExtraHeaders(headerString)
            .withHttpRequestCmd("GET");
            
        std::unique_ptr<juce::InputStream> stream(urlObj.createInputStream(options));

        if (stream == nullptr) {
            throw std::runtime_error("Failed to connect to server");
        }

        return stream->readEntireStreamAsString();
    }
}; 