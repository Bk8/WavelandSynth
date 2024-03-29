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
    :   attack (0.0f),
        decay (0.0f),
        sustain (1.0f),
        release (0.0f),
        envelopeLevel(0.0f),
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
    double attackSlope = 1.0f / ((attack * sampleRate * 2.0f) + 1.0f);
    double decaySlope = (1.0f - sustain) / ((decay * sampleRate * 2.0f) + 1.0f);
    double releaseSlope = sustain / ((release * sampleRate * 2.0f) + 1.0f);
    
    
    switch (envelopeState)
    {
            
        case idleState:
            
            envlopeTick = 0.0f;
            releasetick = 0.0f;
            envelopeLevel = 0.0f;
            break;
            
        case attackState:
            
            envelopeLevel += attackSlope;
            
            if (envelopeLevel >= 1.0f)
            {
                envelopeState = decayState;
                envelopeLevel = 1.0f;
            }
            
            ++envlopeTick;
            
            break;
            
        case decayState:
            
            if (envelopeLevel <= sustain)
            {
                envelopeState = sustainState;
                envelopeLevel = sustain;
                break;
            }
            else
            {
                envelopeLevel -= decaySlope;
                break;
            }
            
        case sustainState:
            
            envelopeLevel = sustain;
            
            ++envlopeTick;
            
            break;
            
        case releaseState:
            
            envelopeLevel -= releaseSlope;
            
            if (envelopeLevel < 0.0f)
            {
                envelopeLevel = 0.0f;
            }
            
            ++envlopeTick;
            ++releasetick;
            
            break;
            
        default:
            break;
    }
}