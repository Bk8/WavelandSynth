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
    :   attack (1.0),
        decay (0.5),
        sustain (0.5),
        release (1.0),
        envelopeState (idleState)
    {
    }
Envelope::~Envelope()
    {
    }
    
void Envelope::setSampleRate(float SRate)
{
    sampleRate = SRate;
}
    
void Envelope::setEnvelopeParams (float attackParam, float decayParam, float sustainParam, float releaseParam)
{
    attack = attackParam;
    decay = decayParam;
    sustain = sustainParam;
    release = releaseParam;
}

Envelope::envState Envelope::getEnvelopeState()
{
    return envelopeState;
}

void Envelope::setEnvelopeState( Envelope::envState newState)
{
    envelopeState = newState;
}
    
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
    
void Envelope::renderEnvelope ()
{
    double attackSlope = 1.0 / ((attack * sampleRate * 2.0) + 1.0);
    double decaySlope = (1.0 - sustain) / ((decay * sampleRate * 2.0) + 1.0);
    double releaseSlope = sustain / ((release * sampleRate * 2.0) + 1.0);
    
    
    switch (envelopeState)
    {
            
        case idleState:
            
            envlopeTick = 0.0;
            releasetick = 0.0;
            envelopeLevel = 0.0;
            break;
            
        case attackState:
            
            envelopeLevel += attackSlope;
            
            if (envelopeLevel >= 1.0)
            {
                envelopeState = decayState;
            }
            
            envlopeTick ++;
            
            break;
            
        case decayState:
                
            envelopeLevel -= decaySlope;
            
            if (envelopeLevel <= sustain)
            {
                envelopeState = sustainState;
            }
            
            envlopeTick ++;
            
            break;
            
        case sustainState:
            
            envelopeLevel = sustain;
            
            envlopeTick ++;
            
            break;
            
        case releaseState:
            
            envelopeLevel -= releaseSlope;
            
            envlopeTick ++;
            releasetick ++;
            
            break;
            
        default:
            break;
    }
}