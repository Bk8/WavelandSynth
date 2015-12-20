/*
  ==============================================================================

    Envelope.cpp
    Created: 20 Dec 2015 3:38:11am
    Author:  John Caplan

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Envelope.h"

Envelope::Envelope ()
    :   attack (0.0),
        decay (0.0),
        sustain (1.0),
        release (0.0)
    {
    }
Envelope::~Envelope()
    {
    }
    
void Envelope::setSampleRate(float SRate)
{
    sampleRate = SRate;
}
    
void Envelope::SetEnvelopeParams (float attackParam, float decayParam, float sustainParam, float releaseParam)
{
    attack = attackParam;
    decay = decayParam;
    sustain = sustainParam;
    release = releaseParam;
}

Envelope::envState envelopeState = Envelope::idleState;
    
float Envelope::getAttack()
{
    return attack;
}
    
float Envelope::getDecay()
{
    return decay;
}
    
float Envelope::getSustain()
{
    return sustain;
}
    
float Envelope::getRelease()
{
    return release;
}
    
float Envelope::getenvelopeLevel()
{
    return envelopeLevel;
}
    
void Envelope::startEnvelope()
{
    envelopeState = attackState;
    envlopeTick = 0;
}
    
void Envelope::endEnvelope()
{
    envelopeState = releaseState;
    releasetick = 0;
}
    
float Envelope::renderEnvelope ()
{
    float attackSlope = 1.0 / (attack * sampleRate * 2.0);
    float decaySlope = (1.0 - sustain) / (decay * sampleRate * 2.0);
    float releaseSlope = sustain / (release * sampleRate * 2.0);
    
    
    switch (envelopeState)
    {
            
        case idleState:
            envlopeTick = 0;
            releasetick = 0;
            return 0.0;
            break;
            
        case attackState:
            
            envelopeLevel += attackSlope;
            
            if (envlopeTick > attack * sampleRate * 10)
            {
                envelopeState = decayState;
            }
            
            envlopeTick ++;
            
            return envelopeLevel;
            break;
            
        case decayState:
                
            envelopeLevel -= decaySlope;
            
            if (envelopeLevel <= sustain)
            {
                envelopeState = decayState;
            }
            
            envlopeTick ++;
            
            return envelopeLevel;
            break;
            
        case sustainState:
            
            envelopeLevel = sustain;
            
            envlopeTick ++;
            
            return envelopeLevel;
            break;
            
        case releaseState:
            
            envelopeLevel -= releaseSlope;
            
            if (envelopeLevel <= 0.0)
            {
                envelopeState = idleState;
            }
            
            envlopeTick ++;
            releasetick ++;
            
            return envelopeLevel;
            break;
            
        default: return 0.0;
            break;
    }
}