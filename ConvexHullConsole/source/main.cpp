#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <ctime>
#include <iterator>
#include "common.h"
#include "point_loader.h"
#include "point_iterator.h"
#include "convex_hull.h"
#include "line.h"

using namespace std;

namespace {
	void PrintDescription()
	{
		cout << "Draws the convex hull on an image." << endl
			<< endl
			<< "Usage:" << endl
			<< "ConvexHullConsole.exe <INPUT_FILE> -o <OUTPUT_FILE>" << endl
			<< endl
			<< "Remarks:" << endl
			<< "All non-white points are points."
			<< endl;
	}
}

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

void run_find_hull(PointLoader &point_loader, vector<Point> &convex_points)
{
	back_insert_iterator<vector<Point> > back_inserter(convex_points);
	FindHull(PointIterator<PointLoader>(point_loader), PointIterator<PointLoader>(), back_inserter);
}

int main(int argc, char **argv)
{
	if(argc != 4)
	{
		PrintDescription();
		return -1;
	}

	vector<string> cmd_params(argv + 1, argv + argc);
	string in_file, out_file;
	vector<string>::const_iterator it = find(cmd_params.begin(), cmd_params.end(), "-o");
	if(it == cmd_params.end() || (it + 1) == cmd_params.end())
	{
		cout << "Error: Output file parameter is missing" << endl;
		PrintDescription();
		return -1;
	}

	out_file = *(it + 1);
	cmd_params.erase(it, it + 2);
	assert(cmd_params.size() == 1);
	in_file = cmd_params[0];

	try
	{
		clock_t total_begin = clock();
		clock_t begin_clock;

		PointLoader loader(in_file, out_file);
		begin_clock = clock();
		loader.Load();
		PrintTimer("Loading the file", begin_clock);

		vector<Point> convex_points;

		//begin_clock = clock();
		run_find_hull(loader, convex_points);

		Line line;
		if(convex_points.size() >= 2)
		{
			vector<Point>::const_iterator it = convex_points.begin();
			vector<Point>::const_iterator lastIt = it;
			++ it;
			for(; it != convex_points.end(); ++ it)
			{
				line.Draw(*lastIt, *it, loader);
				lastIt = it;
			}
		}

		loader.Save();

		PrintTimer("Total ticks", total_begin);
		cout << "Points = " << loader.get_point_count() << endl;
		size_t convex_size = convex_points.size();
		if(convex_size > 0)
			-- convex_size;
		cout << "Convex Points = " << convex_size << endl;
	}
	catch (ChException& e)
	{
		cout << "Fatal error: " << e.what() << endl;
		return -1;
	}

	return 0;
}



