//
//  Color.h
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

namespace gtf
{
	class Color
	{
	public:


		Color();
		//explicit Color(float rgb);
		Color(float r, float g, float b, float a = 1.0f);
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		explicit Color(unsigned int hexARGB);
		float* asF32A(float* color, bool includeAlpha = false) const;
		unsigned int asHEXARGB(bool includeAlpha = false) const;
		unsigned int asHEXABGR(bool includeAlpha = false) const;
		unsigned char* asU8A(unsigned char* color, bool includeAlpha = false) const;

		//TODO: HSV, static helpers
		static Color fromU8RGB(unsigned char r, unsigned g, unsigned char b);


		//rgba
		float r;
		float g;
		float b;
		float a;

	};

	namespace Colors
	{
		static Color const RED{ 0xFF0000u };
		static Color const GREEN{ 0x00FF00u };
		static Color const BLUE{ 0x0000FFu };
		static Color const WHITE{ 0xFFFFFFu };
		static Color const BLACK{ 0x000000u };
	} //namespace Colors


	inline Color const operator+(float n, Color const & c) { return Color(n + c.r, n + c.g, n + c.b, c.a); };
	inline Color const operator-(float n, Color const & c) { return Color(n - c.r, n - c.g, n - c.b, c.a); };
	inline Color const operator*(float n, Color const & c) { return Color(n * c.r, n * c.g, n * c.b, c.a); };
	inline Color const operator/(float n, Color const & c) { return Color(n / c.r, n / c.g, n / c.b, c.a); };

	inline Color const operator+(Color const & c, float n) { return n + c; };
	inline Color const operator-(Color const & c, float n) { return Color(c.r - n, c.g - n, c.b - n, c.a); };
	inline Color const operator*(Color const & c, float n) { return n * c; };
	inline Color const operator/(Color const & c, float n) { return Color(c.r / n, c.g / n, c.b / n, c.a); };

	inline Color const operator+(Color const & ca, Color const & cb) { return Color(ca.r + cb.r, ca.g + cb.g, ca.b + cb.b, ca.a); };
	inline Color const operator-(Color const & ca, Color const & cb) { return Color(ca.r - cb.r, ca.g - cb.g, ca.b - cb.b, ca.a); };
	inline Color const operator*(Color const & ca, Color const & cb) { return Color(ca.r * cb.r, ca.g * cb.g, ca.b * cb.b, ca.a); };
	inline Color const operator/(Color const & ca, Color const & cb) { return Color(ca.r / cb.r, ca.g / cb.g, ca.b / cb.b, ca.a); };

	inline Color const & operator+(Color & c, float n) { c.r += n; c.g += n; c.b += n; return c; };
	inline Color const & operator-(Color & c, float n) { c.r -= n; c.g -= n; c.b -= n; return c; };
	inline Color const & operator*(Color & c, float n) { c.r *= n; c.g *= n; c.b *= n; return c; };
	inline Color const & operator/(Color & c, float n) { c.r /= n; c.g /= n; c.b /= n; return c; };

	inline Color const & operator+=(Color & ca, Color const & cb) { ca.r += cb.r; ca.g += cb.g; ca.b += cb.b; return ca; };
	inline Color const & operator-=(Color & ca, Color const & cb) { ca.r -= cb.r; ca.g -= cb.g; ca.b -= cb.b; return ca; };
	inline Color const & operator*=(Color & ca, Color const & cb) { ca.r *= cb.r; ca.g *= cb.g; ca.b *= cb.b; return ca; };
	inline Color const & operator/=(Color & ca, Color const & cb) { ca.r /= cb.r; ca.g /= cb.g; ca.b /= cb.b; return ca; };

} //namespace gtf