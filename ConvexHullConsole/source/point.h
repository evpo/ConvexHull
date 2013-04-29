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
};
#endif // point_h__