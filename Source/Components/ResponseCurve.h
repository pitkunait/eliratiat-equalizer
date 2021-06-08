#include <JuceHeader.h>
#include "../PluginProcessor.h"


struct ResponseCurveComponent : public juce::Component,
                                juce::AudioProcessorParameter::Listener,
                                juce::Timer {

    VashEQAudioProcessor &audioProcessor;

    juce::Atomic<bool> parameterChanged{true};

    MonoChain monoChain;

    explicit ResponseCurveComponent(VashEQAudioProcessor &);

    ~ResponseCurveComponent() override;

    void paint(juce::Graphics &g) override;

    void parameterValueChanged(int parameterIndex, float newValue) override;

    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {};

    void timerCallback() override;

};