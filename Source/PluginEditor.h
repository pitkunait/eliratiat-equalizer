#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/ResponseCurve.h"

struct CustomRotarySlider : juce::Slider {
    CustomRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                        juce::Slider::TextEntryBoxPosition::NoTextBox) {

    }
};

//==============================================================================
/**
*/
class VashEQAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    explicit VashEQAudioProcessorEditor(VashEQAudioProcessor &);

    ~VashEQAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;


private:
    VashEQAudioProcessor &audioProcessor;

    CustomRotarySlider peakFreqSlider,
            peakGainSlider,
            peakQualitySlider,
            lowCutFreqSlider,
            highCutFreqSlider,
            lowCutSlopeSlider,
            highCutSlopeSlider;

    ResponseCurveComponent responseCurveComponent;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment peakFreqSliderAttachment,
            peakGainSliderAttachment,
            peakQualitySliderAttachment,
            lowCutFreqSliderAttachment,
            highCutFreqSliderAttachment,
            lowCutSlopeSliderAttachment,
            highCutSlopeSliderAttachment;

    std::vector<juce::Component *> getComps();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VashEQAudioProcessorEditor)
};
