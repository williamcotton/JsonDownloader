#include <JuceHeader.h>
#include "../../Source/JsonApiComponent.h"

class MyFirstUnitTest final : public juce::UnitTest
{
public:
    MyFirstUnitTest() : juce::UnitTest ("MyFirstUnitTest") {}

    void runTest() override
    {
        beginTest ("My first test");

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
                juce::Timer::callAfterDelay(1000, [messageManager]() {
                    messageManager->stopDispatchLoop();
                });

                // Run the message loop until stopped by the timer
                messageManager->runDispatchLoop();
                
                std::cout << "component.resultText.getText() = " << component.resultText.getText() << std::endl;
            }

            juce::DeletedAtShutdown::deleteAll();
            messageManager.reset();
        }
    }
};

static MyFirstUnitTest myUnitTest;

//==============================================================================
int main()
{
    juce::UnitTestRunner runner;
    runner.runAllTests();
    return 0;
}
