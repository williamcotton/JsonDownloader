#include <JuceHeader.h>
#include "../../Source/JsonApiComponent.h"

class MockHttpClient : public HttpClient {
public:
    juce::String makeGetRequest(const juce::String& url) override {
        return R"({
            "userId": 1,
            "id": 1,
            "title": "Test Todo",
            "completed": false
        })";
    }
};

class JsonDownloaderUnitTest final : public juce::UnitTest
{
public:
    JsonDownloaderUnitTest() : juce::UnitTest ("JsonDownloaderUnitTest") {}

    void runTest() override
    {
        beginTest ("JSON Request Test");

        {
            juce::ScopedJuceInitialiser_GUI juceInit;
            std::shared_ptr<juce::MessageManager> messageManager(juce::MessageManager::getInstance());
            messageManager->setCurrentThreadAsMessageThread();

            {
                JsonApiComponent component(std::make_unique<MockHttpClient>());
                component.setSize(600, 400);
                
                component.requestButton.triggerClick();

                // Use a timer to stop the message loop after processing messages
                juce::Timer::callAfterDelay(1000, [messageManager]() {
                    messageManager->stopDispatchLoop();
                });

                // Run the message loop until stopped by the timer
                messageManager->runDispatchLoop();
                
                auto resultText = component.resultText.getText();
                std::cout << "component.resultText.getText() = " << resultText << std::endl;
                expect(resultText.contains("Received JSON"));
                expect(resultText.contains("userId"));
            }

            juce::DeletedAtShutdown::deleteAll();
            messageManager.reset();
        }
    }
};

static JsonDownloaderUnitTest jsonDownloaderUnitTest;

//==============================================================================
int main()
{
    juce::UnitTestRunner runner;
    runner.runAllTests();
    return 0;
}
