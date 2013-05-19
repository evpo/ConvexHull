#include "point.h"
#include <vector>
#include <list>
#include <memory>
#include <utility>
#include "common.h"

#ifndef CONVEX_HULL_H__
#define CONVEX_HULL_H__

void ProcessPoint(std::list<Point> &list, const Point &p, const Point &next_p, bool is_top);

template<typename InItFirst, typename InItLast>
bool PushPair(InItFirst &in_first, InItLast &in_last, std::pair<Point, Point> &point_pare)
{
	if(in_first == in_last)
		return false;

	point_pare.first = point_pare.second;
	point_pare.second = *in_first;
	++ in_first;
	
	return true;
}

template<typename InItFirst, typename InItLast, typename OutIt>
void FindHull(InItFirst in_first, InItLast in_last, OutIt out_it)
{
	std::list<Point> list;
	std::pair<Point, Point> pr;
	int point_count = 0;
	
	while(point_count < 3 && PushPair(in_first, in_last, pr))
	{
		++ point_count;
		// First point is always in convex hull
		if(point_count == 1)
			list.push_back(pr.second);
	}

	// Special case: If only 2 points, the second point is also in the hull.
	if(point_count == 2)
		list.push_back(pr.second);

	bool result = (point_count == 3);
	-- point_count;
	while(result)
	{
		++ point_count;
		ProcessPoint(list, pr.first, pr.second, true);
		ProcessPoint(list, pr.first, pr.second, false);
		result = PushPair(in_first, in_last, pr);
	}

	if(point_count > 2)
	{
		list.push_back(pr.second);
		list.push_back(list.front());
	}

	std::copy(list.begin(), list.end(), out_it);
}

#endif // CONVEX_HULL_H__
