#include <JuceHeader.h>
#include "../../Source/JsonApiComponent.h"

class JsonDownloaderUnitTest final : public juce::UnitTest
{
public:
    JsonDownloaderUnitTest() : juce::UnitTest ("JsonDownloaderUnitTest") {}


    void runTest() override
    {
        beginTest ("End-to-end test");

        {
            juce::ScopedJuceInitialiser_GUI juceInit;
            std::shared_ptr<juce::MessageManager> messageManager(juce::MessageManager::getInstance());
            messageManager->setCurrentThreadAsMessageThread();


            {
                JsonApiComponent component;
                component.setSize(600, 400);
                expect (component.getWidth() == 600);
                expect (component.getHeight() == 400);
                
                component.requestButton.triggerClick();

                // Use a timer to stop the message loop after processing messages
                juce::Timer::callAfterDelay(5000, [messageManager]() {
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
