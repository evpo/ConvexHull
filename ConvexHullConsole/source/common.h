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
#endif // common_h__