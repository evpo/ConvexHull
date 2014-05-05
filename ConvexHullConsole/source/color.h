#include <string>

#include "assert.h"

#ifndef COLOR__
#define COLOR__

struct RgbaColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	RgbaColor(
		unsigned char rp,
		unsigned char gp,
		unsigned char bp,
		unsigned char ap)
		:r(rp),g(gp),b(bp),a(ap)
	{
	}

	RgbaColor(){}
};
struct GrayColor
{
	unsigned char v;
	unsigned char a;
	
	GrayColor(
		unsigned char vp,
		unsigned char ap
		)
		:v(vp), a(ap)
	{
	}

	GrayColor()
	{
	}
};

RgbaColor ParseColor(const std::string &rgba_str);

#endif