#include <stdexcept>
#include <memory>
#include "point.h"

#ifndef common_h__
#define common_h__

class ChException : public std::runtime_error
{
public:
	ChException(const std::string &message):runtime_error(message){}
};

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

#endif // common_h__