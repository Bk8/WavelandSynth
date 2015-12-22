/*
  ==============================================================================

    Saw.cpp
    Created: 21 Dec 2015 2:13:12pm
    Author:  John Caplan

  ==============================================================================
*/

#include "Saw.h"

float Saw::sawOfAngle (float angle, int pitchInHertz)
{
    
    switch (static_cast<int>(pitchInHertz)) {
            
        case 0 ... 49:
            return saw200ofAngle(angle);
            break;
            
        case 50 ... 99:
            float toStartofCaseF;
            toStartofCaseF = (pitchInHertz - 50.0f) / 49.0f;
            return (saw100ofAngle(angle) * toStartofCaseF) + (saw200ofAngle(angle) * (1-toStartofCaseF));
            break;
            
        case 100 ... 199:
            float toStartofCase0;
            toStartofCase0 = (pitchInHertz - 100.0f) / 99.0f;
            return (saw50ofAngle(angle) * toStartofCase0) + (saw100ofAngle(angle) * (1-toStartofCase0));
            break;
            
        case 200 ... 399:
            float toStartofCase1;
            toStartofCase1 = (pitchInHertz - 200.0f) / 199.0f;
            return (saw24ofAngle(angle) * toStartofCase1) + (saw50ofAngle(angle) * (1-toStartofCase1));
            break;
            
        case 400 ... 799:
            float toStartofCase2;
            toStartofCase2 = (pitchInHertz - 400.0f) / 399.0f;
            return (saw12ofAngle(angle) * toStartofCase2) + (saw24ofAngle(angle) * (1-toStartofCase2));
            break;
            
        case 800 ... 1599:
            float toStartofCase3;
            toStartofCase3 = (pitchInHertz - 800.0f) / 799.0f;
            return (saw6ofAngle(angle) * toStartofCase3) + (saw12ofAngle(angle) * (1-toStartofCase3));
            break;
            
        case 1600 ... 2499:
            float toStartofCase4;
            toStartofCase4 = (pitchInHertz - 1600.0f) / 899.0f;
            return (saw4ofAngle(angle) * toStartofCase4) + (saw6ofAngle(angle) * (1-toStartofCase4));
            break;
            
        case 2500 ... 4999:
            float toStartofCase5;
            toStartofCase5 = (pitchInHertz - 2500.0f) / 2499.0f;
            return (saw2ofAngle(angle) * toStartofCase5) + (saw4ofAngle(angle) * (1-toStartofCase5));
            break;
            
        case 5000 ... 9999:
            float toStartofCase6;
            toStartofCase6 = (pitchInHertz - 5000.0f) / 4999.0f;
            return (saw1ofAngle(angle) * toStartofCase6) + (saw2ofAngle(angle) * (1-toStartofCase6));
            break;
            
        case 10000 ... 44000:
            return saw1ofAngle(angle);
            break;
            
        default:
            return saw1ofAngle(angle);
            break;
    }
    //return saw200ofAngle(angle);
}

float Saw::saw1ofAngle (float angle)
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

float Saw::mtf(float notenumber)
{
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
