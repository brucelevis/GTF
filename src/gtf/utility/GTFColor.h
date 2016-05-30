//
//  GTFColor.h
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

class GTFColor
{
public:
    
    
    GTFColor();
    //explicit GTFColor(float rgb);
    explicit GTFColor(float r, float g, float b, float a = 1.0f);
    explicit GTFColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    explicit GTFColor(unsigned int hexARGB);
    float* asF32A(float* color, bool includeAlpha = false);
    unsigned int asHEX( bool includeAlpha = false);
    unsigned char* asU8A(unsigned char* color, bool includeAlpha = false);

    //rgba
    float r;
    float g;
    float b;
    float a;
    
};

namespace GTFCOLORS
{
    static GTFColor const RED   {0xFF0000u};
    static GTFColor const GREEN {0x00FF00u};
    static GTFColor const BLUE  {0x0000FFu};
    static GTFColor const WHITE {0xFFFFFFu};
    static GTFColor const BLACK {0x000000u};
}

inline GTFColor const operator+(float n, GTFColor const & c) { return GTFColor(n + c.r, n + c.g, n + c.b, c.a); };
inline GTFColor const operator-(float n, GTFColor const & c) { return GTFColor(n - c.r, n - c.g, n - c.b, c.a); };
inline GTFColor const operator*(float n, GTFColor const & c) { return GTFColor(n * c.r, n * c.g, n * c.b, c.a); };
inline GTFColor const operator/(float n, GTFColor const & c) { return GTFColor(n / c.r, n / c.g, n / c.b, c.a); };

inline GTFColor const operator+(GTFColor const & c, float n) { return n + c; };
inline GTFColor const operator-(GTFColor const & c, float n) { return GTFColor(c.r - n, c.g - n, c.b - n, c.a); };
inline GTFColor const operator*(GTFColor const & c, float n) { return n * c; };
inline GTFColor const operator/(GTFColor const & c, float n) { return GTFColor(c.r / n, c.g / n, c.b / n, c.a); };

inline GTFColor const operator+(GTFColor const & ca, GTFColor const & cb) { return GTFColor(ca.r + cb.r, ca.g + cb.g, ca.b + cb.b, ca.a); };
inline GTFColor const operator-(GTFColor const & ca, GTFColor const & cb) { return GTFColor(ca.r - cb.r, ca.g - cb.g, ca.b - cb.b, ca.a); };
inline GTFColor const operator*(GTFColor const & ca, GTFColor const & cb) { return GTFColor(ca.r * cb.r, ca.g * cb.g, ca.b * cb.b, ca.a); };
inline GTFColor const operator/(GTFColor const & ca, GTFColor const & cb) { return GTFColor(ca.r / cb.r, ca.g / cb.g, ca.b / cb.b, ca.a); };

inline GTFColor const & operator+(GTFColor & c, float n) { c.r += n; c.g += n; c.b += n; return c; };
inline GTFColor const & operator-(GTFColor & c, float n) { c.r -= n; c.g -= n; c.b -= n; return c; };
inline GTFColor const & operator*(GTFColor & c, float n) { c.r *= n; c.g *= n; c.b *= n; return c; };
inline GTFColor const & operator/(GTFColor & c, float n) { c.r /= n; c.g /= n; c.b /= n; return c; };

inline GTFColor const & operator+=(GTFColor & ca, GTFColor const & cb) { ca.r += cb.r; ca.g += cb.g; ca.b += cb.b; return ca; };
inline GTFColor const & operator-=(GTFColor & ca, GTFColor const & cb) { ca.r -= cb.r; ca.g -= cb.g; ca.b -= cb.b; return ca; };
inline GTFColor const & operator*=(GTFColor & ca, GTFColor const & cb) { ca.r *= cb.r; ca.g *= cb.g; ca.b *= cb.b; return ca; };
inline GTFColor const & operator/=(GTFColor & ca, GTFColor const & cb) { ca.r /= cb.r; ca.g /= cb.g; ca.b /= cb.b; return ca; };

