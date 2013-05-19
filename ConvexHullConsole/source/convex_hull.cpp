#include "convex_hull.h"

#include <algorithm>
#include <sstream>
#include <list>
#include <windows.h>
#include "point.h"

using namespace std;

typedef list<Point> PointList;

namespace {
	PointList::const_iterator AddToList(PointList &list, const Point &point, bool is_front)
	{
		if(is_front)
		{
			list.push_front(point);
			return list.begin();
		}
		else
		{
			list.push_back(point);
			PointList::const_iterator it = list.end();
			-- it;
			return it;
		}
	}

	PointList::const_iterator GetFromTop(PointList &list, bool is_front)
	{
		if(is_front)
		{
			return list.begin();
		}
		else
		{
			PointList::const_iterator it = list.end();
			-- it;
			return it;
		}
	}

	void Progress(PointList &list, PointList::const_iterator &it, bool is_front)
	{
		if(is_front)
		{
			++ it;
			return;
		}

		// is back
		if(it != list.begin())
			-- it;
		else
			it = list.end();
	}

	double compare_vectors( const Point &p1, const Point &p2, const Point &p3 )
	{
		if(p1.y == p2.y && p2.y == p3.y && 
			(p1.x < p2.x && p2.x < p3.x || p1.x > p2.x && p2.x > p3.x ))
			return 0.; // point to the same direction up or down

		return (p2.x - p1.x) - (p3.x - p2.x) * (p2.y - p1.y) / (p3.y - p2.y);
	}
}

void ProcessPoint(PointList &list, const Point &p, const Point &next_p, bool is_top)
{
	bool is_front = !is_top;

	PointList::const_iterator p1 = GetFromTop(list, is_front);
	PointList::const_iterator p2 = AddToList(list, p, is_front);
	Point p3 = next_p;

	while(p1 != list.end() &&
		((is_top && compare_vectors(*p1, *p2, p3) <= 0) || (!is_top && compare_vectors(*p1, *p2, p3) >= 0) ))
	{
		PointList::const_iterator deleted_it = p2;
		Progress(list, p2, is_front);
		list.erase(deleted_it);
		Progress(list, p1, is_front);
	}
}

