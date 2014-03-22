#include "file_work.h"
#include <vector>
#include <iostream>
#include <ctime>
#include "assert.h"
#include "point_loader.h"
#include "convex_hull.h"
#include "point_iterator.h"

using namespace std;

#ifdef PRINT_TIMING
void PrintTimer(string what, clock_t begin)
{
	clock_t end = clock();
	float diff = static_cast<float>(end) - static_cast<float>(begin);
	cout << what << " = " << diff << endl;
}
#else
void PrintTimer(string what, clock_t begin)
{

}
#endif

void RunFindHull(PointLoader &point_loader, vector<Point> &convex_points)
{
	back_insert_iterator<vector<Point> > back_inserter(convex_points);
	FindHull(PointIterator<PointLoader>(point_loader), PointIterator<PointLoader>(), back_inserter);
}

void DrawHullInFile(const std::string &in_file, const std::string &out_file, double stroke_width, Color rgba)
{
	clock_t total_begin = clock();
	clock_t begin_clock;

	PointLoader loader(in_file, out_file);
	begin_clock = clock();
	loader.Load();
	PrintTimer("Loading the file", begin_clock);

	vector<Point> convex_points;

	//begin_clock = clock();
	RunFindHull(loader, convex_points);

	//Line line;
	if(convex_points.size() >= 2)
	{
		auto it = convex_points.begin();
		loader.MoveTo(*it);
		++ it;
		for(; it != convex_points.end(); ++ it)
		{
			loader.LineTo(*it);
		}
	}

	loader.Render(stroke_width, rgba);
	loader.Save();

	PrintTimer("Total ticks", total_begin);
	cout << "Points = " << loader.get_point_count() << endl;
	size_t convex_size = convex_points.size();
	if(convex_size > 0)
		-- convex_size;
	cout << "Convex Points = " << convex_size << endl;
}