#ifndef point_info_extractor_h__
#define point_info_extractor_h__

#include "color.h"
#include "point.h"

template<typename PointLoaderT>
struct PointExtractor
{
	typedef Point point_info;
	static point_info GetPointInfo(const PointLoaderT &loader)
	{
		return loader.GetPoint();
	}
};

template<typename PointLoaderT, typename Color>
struct ColorExtractor
{
	typedef Color point_info;
	static point_info GetPointInfo(const PointLoaderT &loader)
	{
		return loader.GetColor();
	}
};
#endif