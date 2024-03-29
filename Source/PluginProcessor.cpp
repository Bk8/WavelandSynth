/*
  ==============================================================================

    This file has the AudiPprocessor and VirtualAnalogVoice

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Envelope.h"
#include "Wavetable.h"

AudioProcessor* JUCE_CALLTYPE createPluginFilter();

//==============================================================================

class VitualAnalogSound : public SynthesiserSound
{
public:
    VitualAnalogSound() {}
    
    bool appliesToNote (int /*midiNoteNumber*/) override  { return true; }
    bool appliesToChannel (int /*midiChannel*/) override  { return true; }
};

//==============================================================================

class VitualAnalogVoice  : public SynthesiserVoice


{
public:
    VitualAnalogVoice()
    : angleDeltaOSC1 (0.0f),
      angleDeltaOSC2 (0.0f),
      sampleRate (SynthesiserVoice::getSampleRate()),
      balance(0.5f),
      cutoffKnob (0.5f),
      cutoffKeytrack (1.0f),
      resonace (0.5f),
      filterEnvAmt(1.0f),
      lfoRate (0.0f),
      vibratoAmount (0.0f),
      gain (0.7f),
      filter1DelayTap1 (0.0f),
      filter1DelayTap2 (0.0f),
      filter1Sum3 (0.0f),
      filter2DelayTap1 (0.0f),
      filter2DelayTap2 (0.0f),
      filter2Sum3 (0.0f),
      currentBend (0.0f),
      currentNote (0.0f),
      modWheel (0.0f),
      lfoCurrentAngle (0.0f),
      lfoAngleDelta (0.0f)
    
    {
        srand (static_cast <unsigned> (time(0)));
        volumeEnvelope.setSampleRate(sampleRate);
        filterEnvelope.setSampleRate(sampleRate);
        
    }
     
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<VitualAnalogSound*> (sound) != nullptr;
    }
    
    bool isVoiceActive() const override
    {
        return VAvoiceIsAcitve;
    }
    
    void setCutoffKnob (float cutParam)
    {
        cutoffKnob = cutParam;
    }

    void setResonance (float resoparam)
    {
        resonace = resoparam * 2.0f + 0.5f;
    }
    
    void setbendAmount (float bendParam)
    {
        bendAmount = bendParam * 2.0f;
    }

    void setdetune (float detuneParam)
    {
        detune = detuneParam * 0.1f;
    }

    void setbalance (float balParam)
    {
        balance = balParam;
    }

    void setKeytrack (float keyParam)
    {
        cutoffKeytrack = keyParam;
    }
    
    void setFilterEnvAmt (float fltENVAmtParam)
    {
        filterEnvAmt = fltENVAmtParam;
    }
    
    void setLfoRate (float lfoRateParam)
    {
        lfoRate = lfoRateParam * 10.0f;
    }
    
    void setVibratoAmt (float vibratoAmtParam)
    {
        vibratoAmount = vibratoAmtParam;
    }
    
    void setGain (float gainParam)
    {
        gain = gainParam * 0.1f ;
    }
    
    void updateAngleDeltas ()
    {
        lfoAngleDelta = (lfoRate * twoPi/sampleRate);
        
        voiceCurentNote = currentNote + currentBend * bendAmount + (wavetableMath.saw1ofAngle(lfoCurrentAngle) * modWheel);
        
        float cyclesPerSecondOCS1 = wavetableMath.mtf(voiceCurentNote + detune);
        float cyclesPerSecondOCS2 = wavetableMath.mtf(voiceCurentNote - detune);
        float cyclesPerSampleOSC1 = cyclesPerSecondOCS1 / sampleRate;
        float cyclesPerSampleOSC2 = cyclesPerSecondOCS2 / sampleRate;
        
        angleDeltaOSC1 = cyclesPerSampleOSC1 * twoPi;
        angleDeltaOSC2 = cyclesPerSampleOSC2 * twoPi;
        currentPitchInHertzOSC1 = float (cyclesPerSecondOCS1);
        currentPitchInHertzOSC2 = float (cyclesPerSecondOCS2);
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int currentPitchWheelPosition) override
    {

        VAvoiceIsAcitve = true;
        
        currentNote = midiNoteNumber;
        
        if (currentPitchWheelPosition <= 16383 && currentPitchWheelPosition >= 0)
        {
            currentBend = (currentPitchWheelPosition - 8192.0f)/8192.0f;
        }
        
        float startangleosc1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/twoPi));
        float startangleosc2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/twoPi));
        currentAngleOSC1 = startangleosc1;
        currentAngleOSC2 = startangleosc2;
        updateFilterParams();
        
        volumeEnvelope.startEnvelope();
        filterEnvelope.startEnvelope();
        level = velocity * gain;
        
        updateAngleDeltas();
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        volumeEnvelope.endEnvelope();
        filterEnvelope.endEnvelope();
        volumeEnvelope.renderEnvelope();
        filterEnvelope.renderEnvelope();
        
    }
    
    void pitchWheelMoved (int newValue) override
    {
        currentBend = (newValue - 8192.0f) / 8192.0f;
        updateFilterParams();
    }
    
    void controllerMoved (int controllerNumber, int newValue) override
    {
        if (controllerNumber == 1)
        {
            modWheel = (float) newValue/ 127.0f * vibratoAmount;
        }
    }
    
    void innitFilter ()
    {
        filter1Sum1 = 0.0f;
        filter1Sum2 = 0.0f;
        filter1Sum3 = 0.0f;
        filter1Product1 = 0.0f;
        filter1Product2 = 0.0f;
        filter1Product3  = 0.0f;
        filter1DelayTap1 = 0.0f;
        filter1DelayTap2 = 0.0f;
        
        filter2Sum1 = 0.0f;
        filter2Sum2 = 0.0f;
        filter2Sum3 = 0.0f;
        filter2Product1 = 0.0f;
        filter2Product2 = 0.0f;
        filter2Product3  = 0.0f;
        filter2DelayTap1 = 0.0f;
        filter2DelayTap2 = 0.0f;
    }
    
    void updateFilterParams ()
    {
        if (
            cutoffKnob != cutoffKnobPrev
            || cutoffKeytrack != cutoffKeytrackPrev
            || voiceCurentNote != voiceCurentNotePrev
            || (float) filterEnvelope.getenvelopeLevel() != filterEnvPrev
            )
            
            {
                float cutoffFromKeytrack = voiceCurentNote * cutoffKeytrack;
                float cutoffFromENV = (135.0f - cutoffFromKeytrack) * filterEnvelope.getenvelopeLevel() * filterEnvAmt;
                float cutoffFromKnob = (135.0f - cutoffFromKeytrack) * cutoffKnob;
                float cutoffNote = cutoffFromKnob + cutoffFromKeytrack + cutoffFromENV;
                if (cutoffNote > 135)
                {
                    cutoffNote = 135;
                }
                float cutoff = wavetableMath.mtf(cutoffNote);
                filterF = 2.0 * wavetableMath.saw1ofAngle(float_Pi * cutoff / (sampleRate * 4.0f) + float_Pi);;
            }
        
        cutoffKnobPrev = cutoffKnob * 1.0f;
        cutoffKeytrackPrev = cutoffKeytrack * 1.0f;
        voiceCurentNotePrev = voiceCurentNote * 1.0f;
        filterEnvPrev = (float) filterEnvelope.getenvelopeLevel();
        filterQ = 1.0f / resonace;
        
    }
    
    float filterSound (float inputSample)
    {
        for (int i = 0; i < 4; ++i)
            {
                filter1Sum1 = inputSample - filter1Product2 - filter1Sum3;
                filter1Product1 = filterF * filter1Sum1;
                filter1Sum2 = filter1Product1 + filter1DelayTap1;
                filter1DelayTap1 = filter1Sum2;
                filter1Product2 = filterQ * filter1DelayTap1;
                filter1Product3 = filterF * filter1DelayTap1;
                filter1Sum3 = filter1Product3 + filter1DelayTap2;
                filter1DelayTap2 = filter1Sum3;

                filter2Sum1 = filter1Sum3 - filter2Product2 - filter2Sum3;
                filter2Product1 = filterF * filter2Sum1;
                filter2Sum2 = filter2Product1 + filter2DelayTap1;
                filter2DelayTap1 = filter2Sum2;
                filter2Product2 = filterQ * filter2DelayTap1;
                filter2Product3 = filterF * filter2DelayTap1;
                filter2Sum3 = filter2Product3 + filter2DelayTap2;
                filter2DelayTap2 = filter2Sum3;
            }
        
        return filter2Sum3;
    }
    
    void setVolEnvelopeParams (float at, float de, float su, float re)
    {
        volumeEnvelope.setEnvelopeParams(at, de, su, re);
    }
    
    void setfilEnvelopeParams (float fat, float fde, float fsu, float fre)
    {
        filterEnvelope.setEnvelopeParams(fat, fde, fsu, fre);
    }
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        processBlock (outputBuffer, startSample, numSamples);
    }
    
    void renderNextBlock (AudioBuffer<double>& outputBuffer, int startSample, int numSamples) override
    {
        processBlock (outputBuffer, startSample, numSamples);
    }

private:
    
    template <typename FloatType>
    void processBlock (AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
    {
        if (angleDeltaOSC1 != 0.0f && angleDeltaOSC2 != 0.0f)
        {
            if (volumeEnvelope.getEnvelopeState() == Envelope::releaseState)
            {
                while (--numSamples >= 0)
                {
                    const FloatType currentSample =
                    static_cast<FloatType>( filterSound(((wavetableMath.sawOfAngle(currentAngleOSC1, currentPitchInHertzOSC1) * (balance) + wavetableMath.sawOfAngle(currentAngleOSC2, currentPitchInHertzOSC2) * (1.0f - balance)) * level * volumeEnvelope.getenvelopeLevel())));
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                    {
                        outputBuffer.addSample (i, startSample, currentSample);
                        volumeEnvelope.renderEnvelope();
                        filterEnvelope.renderEnvelope();
                    }
                    updateAngleDeltas();
                    
                    currentAngleOSC1 += angleDeltaOSC1;
                    
                    if (currentAngleOSC1 >= twoPi)
                        currentAngleOSC1 -= twoPi;
                    
                    currentAngleOSC2 += angleDeltaOSC2;
                    
                    if (currentAngleOSC2 >= twoPi)
                        currentAngleOSC2 -= twoPi;
                    
                    lfoCurrentAngle += lfoAngleDelta;
                    
                    if (lfoCurrentAngle >= twoPi)
                        lfoCurrentAngle -= twoPi;
                    
                    ++startSample;
                    
                    if (volumeEnvelope.getenvelopeLevel() <= 0.0f)
                    {
                        volumeEnvelope.setEnvelopeState(Envelope::idleState);
                        filterEnvelope.setEnvelopeState(Envelope::idleState);
                        clearCurrentNote();
                        angleDeltaOSC1 = 0.0f;
                        angleDeltaOSC2 = 0.0f;
                        currentAngleOSC1 = 0.0f;
                        currentAngleOSC2 = 0.0f;
                        modWheel = 0.0f;
                        VAvoiceIsAcitve = false;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    const FloatType currentSample = static_cast<FloatType> (filterSound((wavetableMath.sawOfAngle(currentAngleOSC1, currentPitchInHertzOSC1) * (balance) + wavetableMath.sawOfAngle(currentAngleOSC2, currentPitchInHertzOSC2) * (1.0f - balance)) * level * volumeEnvelope.getenvelopeLevel()));
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                    {
                        outputBuffer.addSample (i, startSample, currentSample);
                        volumeEnvelope.renderEnvelope();
                        filterEnvelope.renderEnvelope();
                    }
                    updateAngleDeltas();
                    
                    currentAngleOSC1 += angleDeltaOSC1;
                    
                    if (currentAngleOSC1 >= twoPi)
                        currentAngleOSC1 -= twoPi;
                    
                    currentAngleOSC2 += angleDeltaOSC2;
                    
                    if (currentAngleOSC2 >= twoPi)
                        currentAngleOSC2 -= twoPi;
                    
                    lfoCurrentAngle += lfoAngleDelta;
                    
                    if (lfoCurrentAngle >= twoPi)
                        lfoCurrentAngle -= twoPi;
                    
                    ++startSample;
                }
            }
        }
    }
    const float twoPi = 2 * float_Pi;
    float currentAngleOSC1, currentAngleOSC2, angleDeltaOSC1, angleDeltaOSC2, level, sampleRate;
    float voiceCurentNote, voiceCurentNotePrev, currentPitchInHertzOSC1, currentPitchInHertzOSC2;
    float bendAmount, detune, balance, cutoffKnob, cutoffKeytrack, resonace, filterEnvAmt, lfoRate, vibratoAmount, gain;
    float cutoffKnobPrev, cutoffKeytrackPrev, filterEnvPrev;
    float filterF, filterQ;
    float filter1DelayTap1, filter1DelayTap2, filter1Sum1, filter1Sum2, filter1Sum3, filter1Product1, filter1Product2, filter1Product3;
    float filter2DelayTap1, filter2DelayTap2, filter2Sum1, filter2Sum2, filter2Sum3, filter2Product1, filter2Product2, filter2Product3;
    float currentBend, currentNote, modWheel, lfoCurrentAngle, lfoAngleDelta;
    bool VAvoiceIsAcitve;
    Envelope volumeEnvelope {};
    Envelope filterEnvelope {};
    Wavetable wavetableMath {};
};


//==============================================================================

WavelandSynthAudioProcessor::WavelandSynthAudioProcessor()
    : lastUIWidth (600),
      lastUIHeight (400),
      bendAmountParam (nullptr),
      detuneParam(nullptr),
      balanceParam(nullptr),
      cutoffKnobParam(nullptr),
      resonaceParam(nullptr),
      keytrackParam(nullptr),
      filEnvAmtParam(nullptr),
      lfoRateParam(nullptr),
      vibratoAmtParam(nullptr),

      volEnvAttParam(nullptr),
      volEnvDecParam(nullptr),
      volEnvSusParam(nullptr),
      volEnvRelParam(nullptr),

      filEnvAttParam(nullptr),
      filEnvDecParam(nullptr),
      filEnvSusParam(nullptr),
      filEnvRelParam(nullptr),

      gainParam(nullptr)
{

    addParameter(bendAmountParam = new AudioParameterFloat ("bend", "Bend Amount", 0.0f, 1.0f, 0.5f));
    addParameter(detuneParam = new AudioParameterFloat ("detune", "Oscillator Detune", 0.0f, 1.0f, 1.0f));
    addParameter(balanceParam = new AudioParameterFloat ("balance","Oscillator Balance", 0.0f, 1.0f, 0.5f));
    addParameter(cutoffKnobParam = new AudioParameterFloat ("cutoff", "Filter Cutoff", 0.0f, 1.0f, 0.5f));
    addParameter(resonaceParam = new AudioParameterFloat ("resonace", "Filter Resonace", 0.0f, 1.0f, 0.5f));
    addParameter(keytrackParam = new AudioParameterFloat ("keytracking", "Filter Keytracking", 0.0f, 1.0f, 1.0f));
    addParameter(filEnvAmtParam = new AudioParameterFloat ("filtEnvAmt", "Filter Env Amt", 0.0f, 1.0f, 0.0f));
    addParameter(lfoRateParam = new AudioParameterFloat ("lfoRate", "Lfo Rate", 0.0f, 1.0f, 0.6f));
    addParameter(vibratoAmtParam = new AudioParameterFloat ("vibratoAmt", "Vibrato Amount", 0.0f, 1.0f, 0.5f));
    
    addParameter(volEnvAttParam = new AudioParameterFloat ("volAtt", "Volume Attack", 0.0f, 1.0f, 0.01f));
    addParameter(volEnvDecParam = new AudioParameterFloat ("volDec", "Volume Decay", 0.0f, 1.0f, 0.0f));
    addParameter(volEnvSusParam = new AudioParameterFloat ("volSus", "Volume Sustain", 0.0f, 1.0f, 1.0f));
    addParameter(volEnvRelParam = new AudioParameterFloat ("volRel", "Volume Release", 0.0f, 1.0f, 0.05f));
    
    addParameter(filEnvAttParam = new AudioParameterFloat ("filAtt", "Filter Attack", 0.0f, 1.0f, 0.0f));
    addParameter(filEnvDecParam = new AudioParameterFloat ("filDec", "Filter Decay", 0.0f, 1.0f, 0.0f));
    addParameter(filEnvSusParam = new AudioParameterFloat ("filSus", "Filter Sustain", 0.0f, 1.0f, 1.0f));
    addParameter(filEnvRelParam = new AudioParameterFloat ("filRel", "Filter Release", 0.0f, 1.0f, 0.01f));
    
    addParameter(gainParam = new AudioParameterFloat ("Gain","Synth Gain", 0.0f, 1.0f, 0.7f));

    initialiseSynth();
}

void WavelandSynthAudioProcessor::updateParameters()
{
    for (int i = 8; --i >=0;)
    {
        if ( VitualAnalogVoice* myVoice = dynamic_cast<VitualAnalogVoice*> ( synth.getVoice (i) ) )
        {
            myVoice->setbendAmount (*bendAmountParam);
            myVoice->setdetune (*detuneParam);
            myVoice->setbalance (*balanceParam);
            myVoice->setCutoffKnob (*cutoffKnobParam);
            myVoice->setResonance (*resonaceParam);
            myVoice->setKeytrack (*keytrackParam);
            myVoice->setFilterEnvAmt (*filEnvAmtParam);
            myVoice->setLfoRate(*lfoRateParam);
            myVoice->setVibratoAmt(*vibratoAmtParam);
            myVoice->setGain(*gainParam);
            myVoice->setVolEnvelopeParams (*volEnvAttParam, *volEnvDecParam, *volEnvSusParam, *volEnvRelParam);
            myVoice->setfilEnvelopeParams (*filEnvAttParam, *filEnvDecParam, *filEnvSusParam, *filEnvRelParam);
            myVoice->updateFilterParams();
        }
    }
}



WavelandSynthAudioProcessor::~WavelandSynthAudioProcessor()
{
}


void WavelandSynthAudioProcessor::initialiseSynth()
{
    const int numVoices = 8;
    
    //
    for (int i = numVoices; --i >= 0;)
    {
        synth.addVoice(new VitualAnalogVoice());
        if ( VitualAnalogVoice* myVoice = dynamic_cast<VitualAnalogVoice*> ( synth.getVoice (i) ) )
            {
                myVoice->setbendAmount (*bendAmountParam);
                myVoice->setdetune (*detuneParam);
                myVoice->setbalance (*balanceParam);
                myVoice->setCutoffKnob (*cutoffKnobParam);
                myVoice->setResonance (*resonaceParam);
                myVoice->setKeytrack (*keytrackParam);
                myVoice->setFilterEnvAmt (*filEnvAmtParam);
                myVoice->setGain(*gainParam);
                myVoice->setVolEnvelopeParams (*volEnvAttParam, *volEnvDecParam, *volEnvSusParam, *volEnvRelParam);
                myVoice->setfilEnvelopeParams (*filEnvAttParam, *filEnvDecParam, *filEnvSusParam, *filEnvRelParam);
                myVoice->innitFilter();
            }
    }
    //
    synth.addSound(new VitualAnalogSound());
    

    
}

//==============================================================================
void WavelandSynthAudioProcessor::prepareToPlay (double newSampleRate, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need...
    synth.setCurrentPlaybackSampleRate (newSampleRate);
    updateParameters();
    keyboardState.reset();
    reset();
}

void WavelandSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

void WavelandSynthAudioProcessor::reset()
{
}

template <typename FloatType>
void WavelandSynthAudioProcessor::process (AudioBuffer<FloatType>& buffer,
                                           MidiBuffer &midiMessages)
{
    const int numSamples = buffer.getNumSamples();
   
    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
    
    updateParameters();
    
    synth.renderNextBlock(buffer, midiMessages, 0, numSamples);

    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, numSamples);

}

//==============================================================================
AudioProcessorEditor* WavelandSynthAudioProcessor::createEditor()
{
    return new WavelandSynthAudioProcessorEditor (*this);
}

//==============================================================================
void WavelandSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:
    
    // Create an outer XML element..
    XmlElement xml ("MYPLUGINsetTINGS");
    
    //
    xml.setAttribute("uiWidth", lastUIWidth);
    xml.setAttribute("uiHeight", lastUIHeight);
    
    //
    for (int i = 0; i < getNumParameters(); ++i)
        if (AudioProcessorParameterWithID* p =
            dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
            xml.setAttribute(p->paramID, p->getValue());
    
    //
    copyXmlToBinary(xml, destData);
}

void WavelandSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    //
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        //
        if (xmlState->hasTagName("MYPLUGINsetTINGS"))
        {
            //
            lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);
            
            //
            for (int i = 0; i < getNumParameters(); ++i)
                if (AudioProcessorParameterWithID* p =
                    dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
                    p->setValueNotifyingHost((float) xmlState->getDoubleAttribute (p->paramID, p->getValue()));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavelandSynthAudioProcessor();
}

































