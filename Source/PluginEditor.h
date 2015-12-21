/*
 ==============================================================================
 
 This file was auto-generated by the Jucer!
 
 It contains the basic startup code for a Juce application.
 
 ==============================================================================
 */

#ifndef __PLUGINEDITOR_H_4ACCBAA__
#define __PLUGINEDITOR_H_4ACCBAA__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/** This is the editor component that our filter will display.
 */
class WavelandSynthAudioProcessorEditor  : public AudioProcessorEditor,
private Timer
{
public:
    WavelandSynthAudioProcessorEditor (WavelandSynthAudioProcessor&);
    ~WavelandSynthAudioProcessorEditor();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
private:
    class ParameterSlider;
    
    MidiKeyboardComponent midiKeyboard;
    Label timecodeDisplayLabel, bendAmountLabel, detuneLabel, balanceLabel, cutoffLabel, resonaceLabel, keytrackLabel;
    Label volAttackLabel, volDecayLabel, volSustainLabel, volReleaseLabel;
    ScopedPointer<ParameterSlider> bendAmountSlider, detuneSlider,balanceSlider, cutoffSlider, resonaceSlider, keytrackSlider;
    ScopedPointer<ParameterSlider> volAttackSlider, volDecaySlider, volSustainSlider, volReleaseSlider;
    ScopedPointer<ResizableCornerComponent> resizer;
    ComponentBoundsConstrainer resizeLimits;
    
    AudioPlayHead::CurrentPositionInfo lastDisplayedPosition;
    
    //==============================================================================
    WavelandSynthAudioProcessor& getProcessor() const
    {
        return static_cast<WavelandSynthAudioProcessor&> (processor);
    }
    
    void updateTimecodeDisplay (AudioPlayHead::CurrentPositionInfo);
};


#endif  // __PLUGINEDITOR_H_4ACCBAA__
