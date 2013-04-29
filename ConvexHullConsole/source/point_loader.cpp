#include "point_loader.h"
#include <sstream>
#include <algorithm>
#include "Windows.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "common.h"

using namespace std;

PointLoader::PointLoader(string file_name, string target_file):
	file_name_(file_name),target_file_(target_file), loaded_(false), position_(0), eof_(false), point_count_(0)
{
}

void PointLoader::Load(){
	// 8 bytes each pixel. First three bytes are for RGB. Example: 00 00 00 are for a black dot
	data_ = stbi_load(file_name_.c_str(), &x_pixels_, &y_pixels_, &pixel_length_, 0);
	loaded_ = true;
	if(data_ == NULL)
	{
		const char *reason = stbi_failure_reason();
		OutputDebugString(reason);
		throw ChException(string("stbi error on loading the image:") + reason);
	}

	if(x_pixels_ == 0 || y_pixels_ == 0)
	{
		throw ChException("Image file has no pixels");
	}
}

Point PointLoader::ConvertPosition2Point( int position, int x_pixels )
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

int PointLoader::ConvertPoint2Position(Point point, int x_pixels)
{
	// see the above comments for values
	// (1,1)
	// row_offset = 1 * 2 * 8 = 16
	// return = 16 + 1 * 8 = 24

	int row_offset = (int)point.y * x_pixels * pixel_length_;
	return row_offset + (int)point.x * pixel_length_;
}

PointLoader::~PointLoader()
{
	stbi_image_free(data_);
}

void PointLoader::Put( Point point )
{
	if(!loaded_)
		throw ChException("Put: Image is not loaded");

	int position = ConvertPoint2Position(point, x_pixels_);
	data_[position] = 0x00; // this will make the point red
	data_[position + 1] = 0xFF;
	data_[position + 2] = 0xFF;
}

void PointLoader::Save()
{
	int result = stbi_write_png(
		target_file_.c_str(), 
		x_pixels_, y_pixels_, pixel_length_, 
		data_, x_pixels_ * pixel_length_);
	
	if(!result)
		throw ChException("PointLoader::Save: Cannot save image.");
}

SharedPointVector PointLoader::CollectPoints()
{
	SharedPointVector point_vector = make_shared<vector<Point> >();
	SharedPoint pt = NextPointInternal();
	for(;pt.get() != nullptr; pt = NextPointInternal())
	{
		point_vector->push_back(*pt);
	}
	return point_vector;

}

SharedPoint PointLoader::NextPointInternal()
{
	const int kCharsInPixel = 3;
	bool is_white = true;

	for(; !eof_ && is_white; position_ += pixel_length_)
	{
		unsigned char *end = data_ + position_ + kCharsInPixel;
		is_white = find_if(
			data_ + position_, 
			end, 
			[](unsigned char p){return p != 0xFF;}) == end;

		eof_ = !(position_ < (x_pixels_ * y_pixels_ * pixel_length_));
	}

	if(eof_)
		return SharedPoint();

	++ point_count_;

	return SharedPoint(
		new Point(ConvertPosition2Point(position_ - pixel_length_, x_pixels_))
		);
}



