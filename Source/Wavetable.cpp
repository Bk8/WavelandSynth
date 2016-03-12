/*
  ==============================================================================

    Saw.cpp
    Created: 21 Dec 2015 2:13:12pm
    Author:  John Caplan

  ==============================================================================
*/

#include "Wavetable.h"

float Wavetable::sawOfAngle (float angle, int pitchInHertz)
{
    
    int pitchInt = static_cast<int>(pitchInHertz);
            
    if (pitchInt <= 49 && pitchInt >= 0)
    {
        return saw200ofAngle(angle);
    }
    else if (pitchInt <= 99 && pitchInt >= 50)
    {
        float toStartofCaseF;
        toStartofCaseF = (pitchInHertz - 50.0f) / 49.0f;
        return (saw100ofAngle(angle) * toStartofCaseF) + (saw200ofAngle(angle) * (1-toStartofCaseF));
    }
    else if (pitchInt <= 199 && pitchInt >= 100)
    {
        float toStartofCase0;
        toStartofCase0 = (pitchInHertz - 100.0f) / 99.0f;
        return (saw50ofAngle(angle) * toStartofCase0) + (saw100ofAngle(angle) * (1-toStartofCase0));
    }
    else if (pitchInt <= 399 && pitchInt >= 200)
    {
        float toStartofCase1;
        toStartofCase1 = (pitchInHertz - 200.0f) / 199.0f;
        return (saw24ofAngle(angle) * toStartofCase1) + (saw50ofAngle(angle) * (1-toStartofCase1));
    }
    else if (pitchInt <= 799 && pitchInt >= 400)
    {
        float toStartofCase2;
        toStartofCase2 = (pitchInHertz - 400.0f) / 399.0f;
        return (saw12ofAngle(angle) * toStartofCase2) + (saw24ofAngle(angle) * (1-toStartofCase2));
    }
    else if (pitchInt <= 1599 && pitchInt >= 800)
    {
        float toStartofCase3;
        toStartofCase3 = (pitchInHertz - 800.0f) / 799.0f;
        return (saw6ofAngle(angle) * toStartofCase3) + (saw12ofAngle(angle) * (1-toStartofCase3));
    }
    else if (pitchInt <= 2499 && pitchInt >= 1600)
    {
        float toStartofCase4;
        toStartofCase4 = (pitchInHertz - 1600.0f) / 899.0f;
        return (saw4ofAngle(angle) * toStartofCase4) + (saw6ofAngle(angle) * (1-toStartofCase4));
    }
    else if (pitchInt <= 4999 && pitchInt >= 2500)
    {
        float toStartofCase5;
        toStartofCase5 = (pitchInHertz - 2500.0f) / 2499.0f;
        return (saw2ofAngle(angle) * toStartofCase5) + (saw4ofAngle(angle) * (1-toStartofCase5));
    }
    else if (pitchInt <= 9999 && pitchInt >= 5000)
    {
        float toStartofCase6;
        toStartofCase6 = (pitchInHertz - 5000.0f) / 4999.0f;
        return (saw1ofAngle(angle) * toStartofCase6) + (saw2ofAngle(angle) * (1-toStartofCase6));
    }
    else if (pitchInt <= 44100 && pitchInt >= 10000)
    {
        return saw1ofAngle(angle);
    }
    else
    {
        return saw1ofAngle(angle);
    }
}

float Wavetable::saw1ofAngle (float angle)
{
    float index;
    int indexIntPart;
    float indexDecimelPart;
    float difference;
    index = (angle / twoPi * 2048);
    indexIntPart = static_cast<int>(index);
    indexDecimelPart = index - indexIntPart;
    difference = SAW1[indexIntPart +1] - SAW1[indexIntPart];
    
    return SAW1[indexIntPart] + (difference * indexDecimelPart);
}

float Wavetable::mtf(float notenumber)
{
    if (notenumber < 0 || notenumber > 148 || std::isnan(notenumber))
    {
        return 1.0;
        
    }
    float index;
    int indexIntPart;
    float indexDecimelPart;
    float difference;
    index = notenumber;
    indexIntPart = static_cast<int>(index);
    indexDecimelPart = index - indexIntPart;
    difference = MTF[indexIntPart +1] - MTF[indexIntPart];
    
    return MTF[indexIntPart] + (difference * indexDecimelPart);
}
