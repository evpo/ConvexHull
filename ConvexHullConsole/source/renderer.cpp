#include "renderer.h"
#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_renderer_base.h"
#include "agg_renderer_primitives.h"
#include "common.h"

using namespace std;
using namespace agg;
//typedef agg::pixfmt_rgb24 pixfmt_type;
	//typedef agg::renderer_base<agg::pixfmt_rgb24> renbase_type;
	//enum {bytes_per_pixel = 3};

	//agg::rendering_buffer agg_buffer_;
	//std::unique_ptr<pixfmt_type> pixf_;
	//std::unique_ptr<renbase_type> rbase_;
	//std::unique_ptr<agg::renderer_primitives<renbase_type>> rprimitives_;

	//agg_buffer_.attach(data_, x_pixels_, y_pixels_, x_pixels_ * pixel_length_);
	//pixf_ = unique_ptr<pixfmt_type>(new pixfmt_type(agg_buffer_));
	//rbase_ = unique_ptr<renbase_type>(new renbase_type(*pixf_));
	//rprimitives_ = unique_ptr<agg::renderer_primitives<renbase_type> >(
	//	new agg::renderer_primitives<renbase_type>(*rbase_));

	//rprimitives_->line_to(
	//	rprimitives_->coord(point.x),
	//	rprimitives_->coord(point.y),
	//	last
	//	);

		//rprimitives_->line(
		//	rprimitives_->coord(point1.x),
		//	rprimitives_->coord(point1.y),
		//	rprimitives_->coord(point2.x),
		//	rprimitives_->coord(point2.y),
		//	last
		//);

template<class PixelFormat>
class Renderer: public IRenderer
{
private:
	typedef renderer_base<PixelFormat> RenbaseType;
	
	rendering_buffer agg_buffer_;
	unique_ptr<PixelFormat> pixf_;
	unique_ptr<RenbaseType> rbase_;
	unique_ptr<renderer_primitives<RenbaseType>> rprimitives_;

public:
	Renderer(unsigned char *buffer, int width, int height, int pixel_length, int stride)
	{
		agg_buffer_.attach(buffer, width, height, stride);
		pixf_ = unique_ptr<PixelFormat>(new PixelFormat(agg_buffer_));
		rbase_ = unique_ptr<RenbaseType>(new RenbaseType(*pixf_));
		rprimitives_ = unique_ptr<renderer_primitives<RenbaseType> >(
			new renderer_primitives<RenbaseType>(*rbase_));
		rprimitives_->line_color(renderer_primitives<RenbaseType>::color_type(0x00, 0xFF, 0xFF));
	}

	virtual void MoveTo(Point point)
	{
		rprimitives_->move_to(
			rprimitives_->coord(point.x),
			rprimitives_->coord(point.y)
		);
	}

	virtual void LineTo(Point point, bool last=false)
	{
		rprimitives_->line_to(
			rprimitives_->coord(point.x),
			rprimitives_->coord(point.y),
			last
		);
	}

	virtual void Line(Point point1, Point point2, bool last = false)
	{
		rprimitives_->line(
			rprimitives_->coord(point1.x),
			rprimitives_->coord(point1.y),
			rprimitives_->coord(point2.x),
			rprimitives_->coord(point2.y),
			last
		);
	}

};

unique_ptr<IRenderer> RendererFactory::Create(
	unsigned char *buffer, int width, int height, int pixel_length, int stride)
{
	switch(pixel_length)
	{
	case 3:
		return unique_ptr<IRenderer>(new Renderer<pixfmt_rgb24>(buffer, width, height, pixel_length, stride));
	case 4:
		return unique_ptr<IRenderer>(new Renderer<pixfmt_rgba32>(buffer, width, height, pixel_length, stride));
	default:
		throw ChException("Unknown pixel length");
	};
}