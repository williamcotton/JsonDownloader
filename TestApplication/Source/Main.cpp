#include <JuceHeader.h>
#include "../../Source/JsonApiComponent.h"

class MockHttpClient : public HttpClient {
public:
    juce::String makeGetRequest(const juce::String &url) override {
        auto lastPart = url.getLastCharacters(1);
        return juce::String::formatted(R"({
            "userId": "%s",
            "id": 1,
            "title": "Test Todo",
            "completed": false
        })", lastPart.toRawUTF8());
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
                expect(resultText.contains("1"));
            }

            juce::DeletedAtShutdown::deleteAll();
            messageManager.reset();
        }

        beginTest("Slider Change JSON Request Test");
        {
            juce::ScopedJuceInitialiser_GUI juceInit;
            std::shared_ptr<juce::MessageManager> messageManager(
                juce::MessageManager::getInstance());
            messageManager->setCurrentThreadAsMessageThread();

            {
                JsonApiComponent component(std::make_unique<MockHttpClient>());
                component.setSize(600, 400);

                component.sliderComponent.slider.setValue(2);
                component.requestButton.triggerClick();

                // Use a timer to stop the message loop after processing messages
                juce::Timer::callAfterDelay(1000, [messageManager]() {
                messageManager->stopDispatchLoop();
                });

                // Run the message loop until stopped by the timer
                messageManager->runDispatchLoop();

                auto resultText = component.resultText.getText();
                std::cout << "component.resultText.getText() = " << resultText
                        << std::endl;
                expect(resultText.contains("Received JSON"));
                expect(resultText.contains("userId"));
                expect(resultText.contains("2"));
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
