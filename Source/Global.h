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
    
    constexpr static const float margin = 10.0f;
    constexpr static const float axisMargin = 25.0f;
    constexpr static const float pzAxisMargin = 30.0f;

    static const int numCoeffs = 12;
    
    static const int fftOrder = 8192;
    
    constexpr static const float bdCompDim = 40;
    constexpr static const float vertArrowLength = bdCompDim * 0.75;
    constexpr static const float gainHeight = bdCompDim * 0.75;
    constexpr static const float gainWidth = bdCompDim * 0.5;
    
    constexpr static const float gainFont = 16.0f;

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
    
    constexpr static const float arrowHeight = 5.0f;

};
