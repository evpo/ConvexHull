#include <string>

#include "assert.h"

#ifndef COLOR__
#define COLOR__

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

Color ParseColor(const std::string &rgba_str);

#endif