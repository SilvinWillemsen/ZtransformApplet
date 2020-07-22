/*
  ==============================================================================

    Global.h
    Created: 21 Jul 2020 11:22:09am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;
class Global
{
public:
    static const uint32 textColour = 0xff000000;
    static const uint32 backgroundColour = 0xffffffff;
    static const uint32 alternateColour = 0xffd3d3d3;
    
    static const int margin = 10;
    static const int axisMargin = 25;

    static const int numCoeffs = 10;
    
    static const int fftOrder = 8192;
    
    static double limit (double val, double min, double max)
    {
        if (val < min)
        {
            val = min;
            return val;
        }
        else if (val > max)
        {
            val = max;
            return val;
        }
        return val;
    }
    
};
