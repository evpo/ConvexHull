#include "renderer.h"
#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_renderer_base.h"
#include "agg_renderer_primitives.h"

#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_path_storage.h"
#include "agg_conv_stroke.h"
#include "agg_renderer_scanline.h"

#include "ch_exception.h"
#include "color.h"

using namespace std;
using namespace agg;

namespace {
	template<typename T>
	T GetPixelAggColor(Color color)
	{
		return T(color.r, color.g, color.b, color.a);
	}

	template<>
	gray8 GetPixelAggColor<gray8>(Color color)
	{
		return gray8(0x00);
	}

	template<>
	gray16 GetPixelAggColor<gray16>(Color color)
	{
		return gray16(0x00);
	}

};

template<class PixelFormat>
class Renderer: public IRenderer
{
private:
	agg::rendering_buffer agg_buffer_;
	agg::path_storage path_storage_;

public:
	Renderer(unsigned char *buffer, int width, int height, int pixel_length, int stride):
		agg_buffer_(buffer, width, height, stride), path_storage_()
	{
	}

	virtual void MoveTo(Point point)
	{
		path_storage_.move_to(point.x, point.y);
	}

	virtual void LineTo(Point point)
	{
		path_storage_.line_to(point.x, point.y);
	}

	virtual void Render(double stroke_width, Color color)
	{
		typedef agg::renderer_base<PixelFormat> RenbaseType;
		
		PixelFormat pixf(agg_buffer_);
		RenbaseType rbase(pixf);

		agg::rasterizer_scanline_aa<> ras;
        agg::scanline_u8 sl;
		
		agg::conv_stroke<path_storage> stroke(path_storage_);
		stroke.width(stroke_width);
		
		ras.add_path(stroke);
		agg::render_scanlines_aa_solid(ras, sl, rbase, GetPixelAggColor<RenbaseType::color_type>(color));
	}

};



unique_ptr<IRenderer> RendererFactory::Create(
	unsigned char *buffer, int width, int height, int pixel_length, int stride)
{
	switch(pixel_length)
	{
	case 1:
		return unique_ptr<IRenderer>(new Renderer<pixfmt_gray8>(buffer, width, height, pixel_length, stride));
	case 2:
		return unique_ptr<IRenderer>(new Renderer<pixfmt_gray16>(buffer, width, height, pixel_length, stride));
	case 3:
		return unique_ptr<IRenderer>(new Renderer<pixfmt_rgb24>(buffer, width, height, pixel_length, stride));
	case 4:
		return unique_ptr<IRenderer>(new Renderer<pixfmt_rgba32>(buffer, width, height, pixel_length, stride));
	default:
		throw ChException("Unknown pixel length");
	};
}