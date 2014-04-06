#ifndef color_evaluator_h__
#define color_evaluator_h__

#include <vector>
#include "agg_pixfmt_gray.h"
#include "color.h"
#include "point_iterator.h"
#include "point_info_extractor.h"

unsigned char GetColorRank(RgbaColor color)
{
	double rank = 0.299*color.r + 0.587*color.g + 0.114*color.b;
	assert(rank <= 255.0);
	return static_cast<unsigned char>(rank);

	//agg::gray8 agg_gray(agg::rgba(color.r, color.g, color.b, color.a));
	//return agg_gray.v;
}

unsigned char GetColorRank(GreyColor color)
{
	return color.v;
}

template<typename Color>
class ColorEvaluator
{
private:
	static const int array_length_ = 1 << (sizeof(unsigned char) * 8);
	unsigned char threshold_rank_;
	std::vector<bool> bitarray_;
public:

	template<typename Iterator>
	ColorEvaluator(Iterator begin, Iterator end)
		:threshold_rank_(0), bitarray_(array_length_)
	{
		int color_count = 0;

		Iterator it = begin;
		for( ;it != end; ++ it)
		{
			unsigned char rank = GetColorRank(*it);
			if(bitarray_[rank])
				continue;

			bitarray_[rank] = true;
			color_count ++;
		}

		assert(color_count <= array_length_);
		
		int i = 0;
		for(; color_count > 0; ++i)
		{
			if(!bitarray_[i])
				continue;
			color_count -= 2;
		}
		threshold_rank_ = (i > 0) ? i - 1 : 0xFF;
	}

	bool TestColor(Color color)
	{
		return GetColorRank(color) <= threshold_rank_;
	}

};

#endif