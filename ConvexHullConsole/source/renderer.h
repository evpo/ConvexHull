#include <memory>
#include "point.h"


#ifndef RENDERER
#define RENDERER

class IRenderer
{
public:
	virtual void MoveTo(Point point) = 0;
	virtual void LineTo(Point point, bool last=false) = 0;
	virtual void Line(Point point1, Point point2, bool last = false) = 0;
	virtual ~IRenderer(){}
	
};

class RendererFactory
{
public:
	static std::unique_ptr<IRenderer> Create(
		unsigned char *buffer, int width, int height, int pixel_length, int stride);
};

#endif