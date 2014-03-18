//usage
//PointLoader loader("c:\\home\\noname01.png");
//SharedPointVector loader.Load();
//if(!loader.Save(SharedPointVector points)) return false;

#include <string>
#include <memory>
#include <vector>
#include <iterator>
#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgb.h"
#include "agg_renderer_base.h"
#include "agg_renderer_primitives.h"
#include "point.h"
#include "common.h"

#ifndef POINT_LOADER_H_
#define POINT_LOADER_H_

class PointLoader
{
public:
	int get_point_count()
	{
		return point_count_;
	}

	PointLoader(std::string file_name, std::string target_file);
	~PointLoader();
	void Load();
	void CollectAllPoints(std::vector<Point> &collection);
	void Save();
	void Put(Point point);
	void MoveTo(Point point);
	void LineTo(Point point);

	virtual bool NextPoint(Point &point);
	operator bool() const;
protected:
	
private:
	typedef agg::pixfmt_rgb24 pixfmt_type;
	typedef agg::renderer_base<agg::pixfmt_rgb24> renbase_type;
	enum {bytes_per_pixel = 3};

	agg::rendering_buffer agg_buffer_;
	std::unique_ptr<pixfmt_type> pixf_;
	std::unique_ptr<renbase_type> rbase_;
	std::unique_ptr<agg::renderer_primitives<renbase_type>> rprimitives_;

	std::string file_name_, target_file_;
	unsigned char *data_;
	bool loaded_, eof_;
	int x_pixels_;
	int y_pixels_;
	int pixel_length_;
	int position_;
	int point_count_;

	Point ConvertPosition2Point(int position, int x_pixels);
	int ConvertPoint2Position(Point point, int x_pixels);

	PointLoader *operator=(const PointLoader &);
	PointLoader(const PointLoader &);
};

#endif POINT_LOADER_H_