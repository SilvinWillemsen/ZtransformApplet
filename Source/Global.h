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
    constexpr static const float initWidth = 1140.0f;
    constexpr static const float initHeight = 500.0f;
    constexpr static const float initHeightWithOffset = 477.0f;

    static const uint32 textColour = 0xff000000;
    static const uint32 backgroundColour = 0xffffffff;
    static const uint32 alternateColour = 0xffd3d3d3;
    
    constexpr static const float margin = 10.0f;
    constexpr static const float axisMargin = 30.0f;
    constexpr static const float pzAxisMargin = 30.0f;

    static const int numCoeffs = 12;
    
    static const int fftOrder = 8192;
    static const bool showPhaseValue = false;

    constexpr static const float bdCompDim = 40;
    constexpr static const float vertArrowLength = bdCompDim * 0.73;
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
    static double outputLimit (double val)
    {
        if (val < -1.0)
        {
//            std::cout << "Limiter used" << std::endl;
            val = -1.0;
            return val;
        }
        else if (val > 1.0)
        {
//            std::cout << "Limiter used" << std::endl;
            val = 1.0;
            return val;
        }
        return val;
    }
    
    static inline int sgn (double val) { return (0 < val) - (val < 0); };
    
    constexpr static const float arrowHeight = 5.0f;

};
