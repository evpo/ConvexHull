#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <assert.h>
#include <ctime>
#include <iterator>
#include "common.h"
#include "file_work.h"

using namespace std;

namespace {
	void PrintDescription()
	{
		cout << "Draws the convex hull on an image." << endl
			<< endl
			<< "Usage:" << endl
			<< "  ConvexHullConsole.exe <INPUT_FILE> -o <OUTPUT_FILE> [-c <RGBA>] [-w <STROKE_WIDTH>]" << endl
			<< endl
			<< "-c <RGBA> - Color components in HEX" << endl
			<< "  RRGGBBAA RGB and alpha A" << endl
			<< "  Alpha examples: FF - non transparent, 7F - half transparent and 00 - invisible" << endl
			<< "  Example: -c 00FF00FF for non transparent green" << endl
			<< "  Default: FF00007F" << endl
			<< endl
			<< "-w <STROKE_WIDTH> - decimal value." << endl 
			<< "  Example: 1.5" << endl
			<< "  Default: 2.0" << endl
			<< endl
			<< "Remarks:" << endl
			<< "  All non-white points in the source image are points for convex hull." << endl;
	}
}


bool ParseAndRemove(vector<string> &params, const string &switch_name, string &out_value)
{
	auto it = find(params.begin(), params.end(), switch_name);
	if(it != params.end() && (it + 1)!=params.end())
	{
		out_value = *(it + 1);
		params.erase(it, it + 2);
		return true;
	}
	else
	{
		return false;
	}
}

int main(int argc, char **argv)
{
	if(argc < 4)
	{
		PrintDescription();
		return -1;
	}

	Color rgba = {0xFF, 0x00, 0x00, 0x7F};
	double stroke_width = 2.0;

	string in_file, out_file;
	string rgba_str, width_str;

	vector<string> cmd_params(argv + 1, argv + argc);
	
	// Output file parameter
	if(!ParseAndRemove(cmd_params, "-o", out_file))
	{
		cout << "Error: Output file parameter is missing" << endl;
		PrintDescription();
		return -1;
	}

	// Color parameter
	if(ParseAndRemove(cmd_params, "-c", rgba_str))
	{
		unsigned bytes;
		istringstream stm(rgba_str);
		stm >> std::hex >> bytes;
		rgba.a = static_cast<unsigned char>(bytes & 0x000000FF);
		bytes = bytes >> 8;
		rgba.b = static_cast<unsigned char>(bytes & 0x000000FF);
		bytes = bytes >> 8;
		rgba.g = static_cast<unsigned char>(bytes & 0x000000FF);
		bytes = bytes >> 8;
		rgba.r = static_cast<unsigned char>(bytes & 0x000000FF);
		assert(bytes >> 8 == 0);
	}

	if(ParseAndRemove(cmd_params, "-w", width_str))
	{
		istringstream stm(width_str);
		stm >> stroke_width;
	}

	if(cmd_params.size() != 1)
	{
		cout << "Error: Invalid parameters" << endl;
		PrintDescription();
		return -1;
	}

	in_file = cmd_params[0];

	try
	{
		DrawHullInFile(in_file, out_file, stroke_width, rgba);
	}
	catch (ChException& e)
	{
		cout << "Fatal error: " << e.what() << endl;
		return -1;
	}

	return 0;
}



