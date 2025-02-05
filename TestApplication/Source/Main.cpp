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
            std::unique_ptr<juce::MessageManager> messageManager(juce::MessageManager::getInstance());
            messageManager->setCurrentThreadAsMessageThread();

            {
                JsonApiComponent component;
                component.setSize(600, 400);
                expect (component.getWidth() == 600);
                expect (component.getHeight() == 400);
                component.requestButton.triggerClick();
                // wait for the button to be clicked
                std::cout << "component.resultText.getText() = " << component.resultText.getText() << std::endl;
                // expect (component.resultText.getText() == "Hello, World!");
            }


            // First stop the message loop
            messageManager->stopDispatchLoop();
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
