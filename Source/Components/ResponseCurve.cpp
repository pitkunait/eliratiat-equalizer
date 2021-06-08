#include "ResponseCurve.h"


ResponseCurveComponent::ResponseCurveComponent(VashEQAudioProcessor &p) : audioProcessor(p) {
    const auto &params = audioProcessor.getParameters();
    for (auto param: params) {
        param->addListener(this);
    }
    startTimerHz(60);
}

ResponseCurveComponent::~ResponseCurveComponent() {
    const auto &params = audioProcessor.getParameters();
    for (auto param: params) {
        param->removeListener(this);
    }
}

void ResponseCurveComponent::paint(juce::Graphics &g) {
    using namespace juce;
    g.fillAll(Colours::black);

    auto responseArea = getLocalBounds();
    auto width = responseArea.getWidth();
    auto &lowcut = monoChain.get<ChainPositions::LowCut>();
    auto &peak = monoChain.get<ChainPositions::Peak>();
    auto &highcut = monoChain.get<ChainPositions::HighCut>();

    auto sampleRate = audioProcessor.getSampleRate();

    std::vector<double> mags;
    mags.resize(width);

    for (int i = 0; i < width; ++i) {
        double mag = 1.f;
        auto freq = mapToLog10(double(i) / double(width), 20.0, 20000.0);
        if (!monoChain.isBypassed<ChainPositions::Peak>()) {
            mag *= peak.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }

        if (!monoChain.isBypassed<ChainPositions::LowCut>()) {
            if (!lowcut.isBypassed<0>())
                mag *= lowcut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!lowcut.isBypassed<1>())
                mag *= lowcut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!lowcut.isBypassed<2>())
                mag *= lowcut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!lowcut.isBypassed<3>())
                mag *= lowcut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }

        if (!monoChain.isBypassed<ChainPositions::HighCut>()) {
            if (!highcut.isBypassed<0>())
                mag *= highcut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!highcut.isBypassed<1>())
                mag *= highcut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!highcut.isBypassed<2>())
                mag *= highcut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!highcut.isBypassed<3>())
                mag *= highcut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }

        mags[i] = Decibels::gainToDecibels(mag);
    }

    Path responseCurve;
    const double outputMin = responseArea.getBottom();
    const double outputMax = responseArea.getY();
    auto map = [outputMin, outputMax](double input) {
        return jmap(input, -24.0, 24.0, outputMin, outputMax);
    };
    responseCurve.startNewSubPath(float(responseArea.getX()), float(map(mags.front())));

    for (size_t i = 1; i < mags.size(); ++i) {
        responseCurve.lineTo(float(responseArea.getX() + i), float(map(mags[i])));
    }

    g.setColour(Colours::orange);
    g.drawRoundedRectangle(responseArea.toFloat(), 4.f, 1.f);

    g.setColour(Colours::white);
    g.strokePath(responseCurve, PathStrokeType(2.f));

}

void ResponseCurveComponent::parameterValueChanged(int parameterIndex, float newValue) {
    parameterChanged.set(true);
}

void ResponseCurveComponent::timerCallback() {
    if (parameterChanged.compareAndSetBool(false, true)) {
        auto chainSettings = getChainSettings(audioProcessor.apvts);
        auto sampleRate = audioProcessor.getSampleRate();

        auto peakCoefficients = makePeakFilter(chainSettings, sampleRate);
        updateCoefficients(monoChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);

        auto lowCutCoefficients = makeLowCutFilter(chainSettings, sampleRate);
        auto highCutCoefficients = makeHighCutFilter(chainSettings, sampleRate);

        updateCutFilter(monoChain.get<ChainPositions::LowCut>(), lowCutCoefficients, chainSettings.lowCutSlope);
        updateCutFilter(monoChain.get<ChainPositions::HighCut>(), highCutCoefficients, chainSettings.highCutSlope);

        repaint();
    }
}
