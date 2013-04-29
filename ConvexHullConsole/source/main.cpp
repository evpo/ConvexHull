#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <ctime>
#include "common.h"
#include "point_loader.h"
#include "convex_hull.h"
#include "line.h"

using namespace std;

namespace {
	void PrintDescription()
	{
		cout << "Draws convex hull in an image file" << endl
			<< endl
			<< "Usage:" << endl
			<< "ConvexHullConsole.exe <PROCESSED_FILE.PNG> -o <OUTPUT_FILE.PNG>" << endl
			<< endl
			<< "Input: PROCESSED_FILE.PNG" << endl
			<< "Output: creates PROCESSED_FILE(HULL).PNG in the same directory" << endl;
	}
}

void PrintTimer(string what, clock_t begin)
{
	clock_t end = clock();
	float diff = static_cast<float>(end) - static_cast<float>(begin);
	cout << what << " = " << diff << endl;
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
		
		Line line;
		ConvexHull convex_hull;
		begin_clock = clock();
		SharedPointList point_list = convex_hull.FindHull(loader);
		PrintTimer("FindHull", begin_clock);

		if(point_list->size() >= 2)
		{
			PointList::const_iterator it = point_list->begin();
			PointList::const_iterator lastIt = it;
			++ it;
			for(; it != point_list->end(); ++ it)
			{
				line.Draw(**lastIt, **it, loader);
				lastIt = it;
			}
		}

		loader.Save();

		PrintTimer("Total ticks", total_begin);
		cout << "Points = " << loader.get_point_count() << endl;
		size_t convex_size = point_list->size();
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