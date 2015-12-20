/*
  ==============================================================================

    Envelope.h
    Created: 20 Dec 2015 3:38:11am
    Author:  John Caplan

  ==============================================================================
*/

#ifndef ENVELOPE_H_INCLUDED
#define ENVELOPE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Envelope
{
public:
    Envelope ();
    ~Envelope();
    
    void setSampleRate(float SRate);
    
    void SetEnvelopeParams (float attackParam, float decayParam, float sustainParam, float releaseParam);
    
    enum envState
    {
        idleState,
        attackState,
        decayState,
        sustainState,
        releaseState
    };
    
    float getAttack();
    
    float getDecay();
    
    float getSustain();
    
    float getRelease();
    
    float getenvelopeLevel();
    
    void startEnvelope();
    
    void endEnvelope();
    
    float renderEnvelope ();
    
private:
    float sampleRate, attack, decay, sustain, release, envelopeLevel;
    int envlopeTick, releasetick;
    
};



#endif  // ENVELOPE_H_INCLUDED
