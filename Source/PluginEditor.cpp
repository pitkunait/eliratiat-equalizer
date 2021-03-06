#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VashEQAudioProcessorEditor::VashEQAudioProcessorEditor(VashEQAudioProcessor &p)
        : AudioProcessorEditor(&p),
          audioProcessor(p),
          responseCurveComponent(audioProcessor),
          peakFreqSliderAttachment(audioProcessor.apvts, "Peak Freq", peakFreqSlider),
          peakGainSliderAttachment(audioProcessor.apvts, "Peak Gain", peakGainSlider),
          peakQualitySliderAttachment(audioProcessor.apvts, "Peak Quality", peakQualitySlider),
          lowCutFreqSliderAttachment(audioProcessor.apvts, "LowCut Freq", lowCutFreqSlider),
          highCutFreqSliderAttachment(audioProcessor.apvts, "HighCut Freq", highCutFreqSlider),
          lowCutSlopeSliderAttachment(audioProcessor.apvts, "LowCut Slope", lowCutSlopeSlider),
          highCutSlopeSliderAttachment(audioProcessor.apvts, "HighCut Slope", highCutSlopeSlider) {

    for (auto *comp: getComps()) {
        addAndMakeVisible(comp);
    }

    setSize(600, 400);
}

VashEQAudioProcessorEditor::~VashEQAudioProcessorEditor() {}

//==============================================================================
void VashEQAudioProcessorEditor::paint(juce::Graphics &g) {
    juce::Image jura = juce::ImageCache::getFromMemory(BinaryData::jura_png, BinaryData::jura_pngSize);
    g.drawImageAt(jura, 0, 136);
}

void VashEQAudioProcessorEditor::resized() {
    auto bounds = getLocalBounds();
    auto responseArea = bounds.removeFromTop(bounds.getHeight() * 0.33);
    responseCurveComponent.setBounds(responseArea);

    auto lowCutArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    lowCutSlopeSlider.setBounds(lowCutArea.removeFromTop(lowCutArea.getHeight() * 0.33));
    lowCutFreqSlider.setBounds(lowCutArea);

    auto highCutArea = bounds.removeFromRight(bounds.getWidth() * 0.5);
    highCutSlopeSlider.setBounds(highCutArea.removeFromTop(highCutArea.getHeight() * 0.33));
    highCutFreqSlider.setBounds(highCutArea);

    peakFreqSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.33));
    peakGainSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.5));
    peakQualitySlider.setBounds(bounds);

}


std::vector<juce::Component *> VashEQAudioProcessorEditor::getComps() {
    return {
            &peakFreqSlider,
            &peakGainSlider,
            &peakQualitySlider,
            &lowCutFreqSlider,
            &highCutFreqSlider,
            &lowCutSlopeSlider,
            &highCutSlopeSlider,
            &responseCurveComponent
    };
}

