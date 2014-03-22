//usage
//PointLoader loader("c:\\home\\noname01.png");
//SharedPointVector loader.Load();
//if(!loader.Save(SharedPointVector points)) return false;

#include <string>
#include <memory>
#include <vector>
#include <iterator>
#include "point.h"
#include "common.h"
#include "renderer.h"

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
	void Render(double stroke_width, Color color);

	virtual bool NextPoint(Point &point);
	operator bool() const;
protected:
	
private:
	std::unique_ptr<IRenderer> renderer_;
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