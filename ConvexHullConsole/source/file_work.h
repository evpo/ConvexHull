#include <string>
#include "ch_exception.h"
#include "color.h"

#ifndef FILE_WORK__
#define FILE_WORK__

void DrawHullInFile(const std::string &in_file, const std::string &out_file, double stroke_width, Color rgba);

#endif