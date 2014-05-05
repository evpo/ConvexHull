#ifndef RENDERER_H__
#define RENDERER_H__

#include <memory>
#include <vector>
#include <tuple>
#include <limits>

#pragma warning(push, 3)
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
#pragma warning(pop)

#include "point.h"
#include "ch_exception.h"

namespace {
	template<typename AggColor, typename Color>
	struct GetPixelAggColorImpl
	{
		static AggColor Get(Color color)
		{
			return AggColor(color.r, color.g, color.b, color.a);
		}

		static Color GetFromAggColor(AggColor agg_color)
		{
			return Color(agg_color.r, agg_color.g, agg_color.b, agg_color.a);
		}
	};

	template<typename Color>
	struct GetPixelAggColorImpl<agg::gray8, Color>
	{
		static agg::gray8 Get(Color color)
		{
			return agg::gray8(color.v, color.a);
		}

		static Color GetFromAggColor(agg::gray8 agg_color)
		{
			return Color(agg_color.v, agg_color.a);
		}
	};


	template<typename Color>
	struct GetPixelAggColorImpl<agg::gray16, Color>
	{
		// ratio = 0xFFFF/0xFF
		static const unsigned max_16_ = 0xFFFFU;
		static const unsigned max_8_ = 0xFFU;

		static agg::gray16 Get(Color color)
		{
			return agg::gray16(color.v * (max_16_/max_8_), color.a * (max_16_/max_8_));
		}

		static Color GetFromAggColor(agg::gray16 agg_color)
		{

			return Color(
				static_cast<unsigned char>(agg_color.v / (max_16_/max_8_)), 
				static_cast<unsigned char>(agg_color.a / (max_16_/max_8_))
				);
		}
	};
};

template<typename PixelFormat, typename Color>
class Renderer
{
private:
	agg::rendering_buffer agg_buffer_;
	PixelFormat pixel_format_;
	agg::path_storage path_storage_;

public:
	Renderer(unsigned char *buffer, int width, int height, int stride):
		agg_buffer_(buffer, width, height, stride), pixel_format_(agg_buffer_), path_storage_()
	{
	}

	void MoveTo(Point point)
	{
		path_storage_.move_to(point.x, point.y);
	}

	void LineTo(Point point)
	{
		path_storage_.line_to(point.x, point.y);
	}

	Color GetPixelColor(int x, int y) const
	{
		return GetPixelAggColorImpl<PixelFormat::color_type, Color>::GetFromAggColor(
			pixel_format_.pixel(x, y));
	}

	void Render(double stroke_width, Color color)
	{
		typedef agg::renderer_base<PixelFormat> RenbaseType;
		
		RenbaseType rbase(pixel_format_);

		agg::rasterizer_scanline_aa<> ras;
        agg::scanline_u8 sl;
		
		agg::conv_stroke<agg::path_storage> stroke(path_storage_);
		stroke.width(stroke_width);
		
		ras.add_path(stroke);
		agg::render_scanlines_aa_solid(
			ras, sl, rbase, GetPixelAggColorImpl<RenbaseType::color_type, Color>::Get(color));
	}

};


#endif

