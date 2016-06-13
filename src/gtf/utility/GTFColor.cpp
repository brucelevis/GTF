//
//  GTFColor.cpp
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFColor.h"
#include <cmath>

GTFColor::GTFColor()
: r(0.0f)
, g(0.0f)
, b(0.0f)
, a(1.0f)
{
    
}

/*GTFColor::GTFColor(float rgb)
{
    r = g = b = rgb;
    a = 1.0f;
}*/

GTFColor::GTFColor(float red, float green, float blue, float alpha)
{
    r = red;
    g = green;
    b = blue;
    a = alpha;
}

GTFColor::GTFColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    r = red / 255.0f;
    g = green / 255.0f;
    b = blue / 255.0f;
    a = alpha / 255.0f;
}

GTFColor::GTFColor(unsigned int hexARGB)
{
    a = (hexARGB >> 24) / 255.0f;
    r = ((hexARGB >> 16) & 0xFF) / 255.0f;
    g = ((hexARGB >> 8) & 0xFF) / 255.0f;
    b = (hexARGB & 0xFF) / 255.0f;
}

float* GTFColor::asF32A(float* color, bool includeAlpha) const
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    
    if (includeAlpha)
        color[3] = a;
    
    return color;
}

unsigned int GTFColor::asHEXARGB( bool includeAlpha) const
{
    unsigned int uiR = (unsigned int)(r * 255);
    unsigned int uiG = (unsigned int)(g * 255);
    unsigned int uiB = (unsigned int)(b * 255);
    unsigned int uiA = (unsigned int)(a * 255);
    
    return (includeAlpha * uiA << 24) + (uiR << 16) + (uiG << 8) + uiB;
}

unsigned int GTFColor::asHEXABGR( bool includeAlpha) const
{
    unsigned int uiR = (unsigned int)(r * 255);
    unsigned int uiG = (unsigned int)(g * 255);
    unsigned int uiB = (unsigned int)(b * 255);
    unsigned int uiA = (unsigned int)(a * 255);
    
    return (includeAlpha * uiA << 24) | (uiB << 16) | (uiG << 8) | uiR;
}

unsigned char* GTFColor::asU8A(unsigned char* color, bool includeAlpha) const
{
    color[0] = (unsigned char)floor(r * 255);
    color[1] = (unsigned char)floor(g * 255);
    color[2] = (unsigned char)floor(b * 255);
    
    if (includeAlpha)
        color[3] = (unsigned char)floor(a * 255);
    
    return color;
}

GTFColor GTFColor::fromU8RGB(unsigned char r, unsigned g, unsigned char b)
{
    return GTFColor(r/255.0f, g/255.0f, b/255.0f, 1.0f);
}
