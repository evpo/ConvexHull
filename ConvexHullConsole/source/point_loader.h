//usage
//PointLoader loader("c:\\home\\noname01.png");
//SharedPointVector loader.Load();
//if(!loader.Save(SharedPointVector points)) return false;


#ifndef POINT_LOADER_H_
#define POINT_LOADER_H_

#include <string>
#include <memory>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <tuple>

#include "windows.h"

#include "stb_image.h"
#include "stb_image_write.h"

#include "point.h"
#include "ch_exception.h"
#include "renderer.h"
#include "point_loader_nextpoint.h"
#include "point_iterator.h"

template<typename Color, typename Renderer>
class PointLoader
{
public:
	int get_point_count() const
	{
		return 0;
	}

	PointLoader(unsigned char *data, int x_pixels, int y_pixels, int pixel_length, Renderer *renderer);
	void CollectAllPoints(std::vector<Point> &collection);
	void Save(const std::string &file_name);
	void Put(Point point);
	void MoveTo(Point point);
	void LineTo(Point point);
	void Render(double stroke_width, Color color);
	bool NextPoint();
	Point GetPoint() const;
	operator bool() const;
protected:
	
private:
	Renderer *renderer_;
	unsigned char *data_;
	bool eof_;
	int x_pixels_;
	int y_pixels_;
	int pixel_length_;
	int position_;
	int point_count_;

	Point ConvertPosition2Point(int position, int x_pixels) const;
	int ConvertPoint2Position(Point point, int x_pixels) const;

	PointLoader *operator=(const PointLoader &);
	PointLoader(const PointLoader &);
};

template<typename Color, typename Renderer>
PointLoader<Color, Renderer>::PointLoader(unsigned char *data, int x_pixels, int y_pixels, int pixel_length, Renderer *renderer)
	:data_(data), x_pixels_(x_pixels), y_pixels_(y_pixels), 
	pixel_length_(pixel_length), position_(0), renderer_(renderer), eof_(false)
{
	NextPoint();
}

template<typename Color, typename Renderer>
Point PointLoader<Color, Renderer>::ConvertPosition2Point( int position, int x_pixels ) const
{
	// example: x_pixels = 2; kPixelLength = 8
	// position = 16; it must return (0, 1)

	// position = 16
	// y = 16 / (2 * 8) = 1
	// x = 16 / 8 - 2 * 1 = 2 - 2 = 0

	// position = 24
	// y = 24 / (2 * 8) = 1
	// x = 24 / 8 - 2 * 1 = 3 - 2 = 1

	Point point;
	point.y = position / (x_pixels * pixel_length_);
	point.x = position / pixel_length_ - x_pixels * point.y;

	return point;
}

template<typename Color, typename Renderer>
int PointLoader<Color, Renderer>::ConvertPoint2Position(Point point, int x_pixels) const
{
	// see the above comments for values
	// (1,1)
	// row_offset = 1 * 2 * 8 = 16
	// return = 16 + 1 * 8 = 24

	int row_offset = (int)point.y * x_pixels * pixel_length_;
	return row_offset + (int)point.x * pixel_length_;
}

//TODO: this for non AGG rendering. It will need to move out to another class when we support both implementations
template<typename Color, typename Renderer>
void PointLoader<Color, Renderer>::Put( Point point )
{
	if(!loaded_)
		throw ChException("Put: Image is not loaded");

	int position = ConvertPoint2Position(point, x_pixels_);
	data_[position] = 0x00; // this will make the point red
	data_[position + 1] = 0xFF;
	data_[position + 2] = 0xFF;
}

template<typename Color, typename Renderer>
void PointLoader<Color, Renderer>::MoveTo(Point point)
{
	renderer_->MoveTo(point);
}

template<typename Color, typename Renderer>
void PointLoader<Color, Renderer>::LineTo(Point point)
{
	renderer_->LineTo(point);
}

template<typename Color, typename Renderer>
void PointLoader<Color, Renderer>::Render(double stroke_width, Color color)
{
	renderer_->Render(stroke_width, color);
}

template<typename Color, typename Renderer>
bool PointLoader<Color, Renderer>::NextPoint()
{
	return NextPointInternal(data_, x_pixels_, y_pixels_, pixel_length_, &position_, &eof_);
}

template<typename Color, typename Renderer>
Point PointLoader<Color, Renderer>::GetPoint() const
{
	assert(*this);
	return Point(ConvertPosition2Point(position_ - pixel_length_, x_pixels_));
}

template<typename Color, typename Renderer>
PointLoader<Color, Renderer>::operator bool() const
{
	return !eof_;
}

template<typename Color, typename Renderer>
void PointLoader<Color, Renderer>::CollectAllPoints( std::vector<Point> &collection )
{
	PointIterator<PointLoader, Point> > it(*this);
	PointIterator<PointLoader, Point> > end;
	std::back_insert_iterator<std::vector<Point> > inserter(collection);
	std::copy(it, end, inserter);
}

#endif POINT_LOADER_H_