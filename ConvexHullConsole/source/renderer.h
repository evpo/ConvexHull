#include <memory>
#include <vector>
#include "point.h"
#include "common.h"


#ifndef RENDERER
#define RENDERER

class IRenderer
{
public:
	virtual void MoveTo(Point point) = 0;
	virtual void LineTo(Point point) = 0;
	virtual void Render(double stroke_width, Color color) = 0;
	virtual ~IRenderer(){}
	
};

class RendererFactory
{
public:
	static std::unique_ptr<IRenderer> Create(
		unsigned char *buffer, int width, int height, int pixel_length, int stride);
};

#endif