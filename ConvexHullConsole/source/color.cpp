#include "color.h"
#include <sstream>
#include "ch_exception.h"

RgbaColor ParseColor(const std::string &rgba_str)
{
	int length = rgba_str.length();
	if(length != 6 && length != 8)
		throw ChException("Invalid color"); 
	
	RgbaColor ret_val;
	unsigned bytes;
	std::istringstream stm(rgba_str);
	stm >> std::hex >> bytes;
	if(length == 8)
	{
		ret_val.a = static_cast<unsigned char>(bytes & 0x000000FF);
		bytes = bytes >> 8;
	}
	else
	{
		ret_val.a = 0xFF;
	}
	ret_val.b = static_cast<unsigned char>(bytes & 0x000000FF);
	bytes = bytes >> 8;
	ret_val.g = static_cast<unsigned char>(bytes & 0x000000FF);
	bytes = bytes >> 8;
	ret_val.r = static_cast<unsigned char>(bytes & 0x000000FF);
	assert(bytes >> 8 == 0);
	return ret_val;
}
