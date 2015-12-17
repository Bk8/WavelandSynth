/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
 
*/
class WavelandSynthAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    WavelandSynthAudioProcessor();
    ~WavelandSynthAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset () override;
    void updateParameters();
    
    //==============================================================================
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {
        jassert(! isUsingDoublePrecision());
        process (buffer, midiMessages);
    }
    
    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
    {
        jassert(! isUsingDoublePrecision());
        process (buffer, midiMessages);
    }
    
    //==============================================================================
    bool hasEditor() const override                                             { return true; }
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override                                       { return JucePlugin_Name; }

    const String getInputChannelName (int channelIndex) const override          { return String (channelIndex + 1); }
    const String getOutputChannelName (int channelIndex) const override         { return String (channelIndex + 1); }
    
    bool isInputChannelStereoPair (int /*index*/) const override                { return true; }
    bool isOutputChannelStereoPair (int /*index*/) const override               { return true; }

    bool acceptsMidi() const override                                           { return true; }
    bool producesMidi() const override                                          { return true; }
    
    bool silenceInProducesSilenceOut() const override                           { return false; }
    double getTailLengthSeconds() const override                                { return 0.0; }

    //==============================================================================
    int getNumPrograms() override                                               { return 1; }
    int getCurrentProgram() override                                            { return 0; }
    void setCurrentProgram (int /*index*/) override                             {}
    const String getProgramName (int /*index*/) override                        {return "Default"; }
    void changeProgramName (int /*index*/, const String& /*name*/) override     {}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    // These properties are public so that our editor component can access them
    // A bit of a hacky way to do it, but it's only a demo! Obviously in your own
    // code you'll do this much more neatly..
    
    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState keyboardState;
    
    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    AudioPlayHead::CurrentPositionInfo lastPosInfo;
    
    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    
    // Our parameters

    AudioParameterFloat* bendAmountParam;
    AudioParameterFloat* detuneParam;
    AudioParameterFloat* balanceParam;
    AudioParameterFloat* cutoffParam;
    AudioParameterFloat* resonaceParam;
    

private:
    //==============================================================================
    template <typename FloatType>
    void process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages);
    
    
    Synthesiser synth;
    
    void initialiseSynth();
    void updateCurrentTimeInfoFromHost();
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavelandSynthAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
