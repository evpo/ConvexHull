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

typedef std::shared_ptr<Point> SharedPoint;

class PointQueue
{
public:
	SharedPoint NextPoint()
	{
		return NextPointInternal();
	}
private:
	virtual SharedPoint NextPointInternal() = 0;
};
#endif // common_h__