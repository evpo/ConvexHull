#include "point.h"
#include "point_loader.h"

#ifndef line_h__
#define line_h__

class Line
{
public:
	void Draw(Point source, Point target, PointLoader &point_loader);

private:
	void DrawFlatLine(Point source, Point target, PointLoader &point_loader);
};

#endif // line_h__
