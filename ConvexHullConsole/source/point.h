#include <memory>
#include <vector>

#ifndef point_h__
#define point_h__

struct Point;

typedef std::vector<Point> PointVector;
typedef std::shared_ptr<std::vector<Point> > SharedPointVector;

struct Point
{
	Point():x(0.), y(0.){}

	Point(double xp, double yp):x(xp),y(yp){}
	double x, y;

public:
	bool operator==(const Point &right) const
	{
		return x == right.x && y == right.y;
	}

	bool operator<(const Point &right) const
	{
		if(this->y < right.y)
			return true;

		if(this->y == right.y && this->x < right.x)
			return true;

		return false;
	}
};
#endif // point_h__