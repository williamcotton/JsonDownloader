#include <JuceHeader.h>
#include "../../Source/JsonApiComponent.h"

class MyFirstUnitTest final : public juce::UnitTest
{
public:
    MyFirstUnitTest() : juce::UnitTest ("MyFirstUnitTest") {}

    void runTest()
    {
        beginTest ("My first test");
        {
            juce::MessageManager::callAsync([this]() {
                JsonApiComponent component;
                component.setSize(600, 400);
                expect (component.getWidth() == 600);
                expect (component.getHeight() == 300);
                // click the button
                // component.requestButton.triggerClick();

            });
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
