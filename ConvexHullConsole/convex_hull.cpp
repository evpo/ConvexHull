#include "convex_hull.h"

#include <algorithm>
#include <sstream>
#include <list>
#include <windows.h>
#include "point.h"



using namespace std;

namespace {
	PointList::const_iterator AddToList(PointList &list, SharedPoint point, bool is_front)
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

	void Progress(SharedPointList list, PointList::const_iterator &it, bool is_front)
	{
		if(is_front)
		{
			++ it;
			return;
		}

		// is back
		if(it != list->begin())
			-- it;
		else
			it = list->end();
	}
}

ConvexHull::ConvexHull(void)
{
}


ConvexHull::~ConvexHull(void)
{
}

void ConvexHull::ProcessPoint(SharedPointList list, SharedPoint p, SharedPoint next_p, bool is_top)
{
	bool is_front = !is_top;

	PointList::const_iterator p1 = GetFromTop(*list, is_front);
	PointList::const_iterator p2 = AddToList(*list, p, is_front);
	SharedPoint p3 = next_p;

	while(p1 != list->end() &&
		((is_top && compare_vectors(*p1, *p2, p3) <= 0) || (!is_top && compare_vectors(*p1, *p2, p3) >= 0) ))
	{
		PointList::const_iterator deleted_it = p2;
		Progress(list, p2, is_front);
		list->erase(deleted_it);
		Progress(list, p1, is_front);
	}
}

// See comments in the header file
SharedPointList ConvexHull::FindHull(PointQueue &point_queue)
{
	SharedPointList list = make_shared<PointList>();
	
	SharedPoint p0 = point_queue.NextPoint();
	
	if(p0.get() != nullptr)
		list->push_back(p0);
	else
		return list;

	SharedPoint p = point_queue.NextPoint();
	SharedPoint next_p = point_queue.NextPoint();

	for(; next_p.get() != nullptr; p = next_p, next_p = point_queue.NextPoint())
	{
		ProcessPoint(list, p, next_p, true);
		ProcessPoint(list, p, next_p, false);
	}

	if(p.get() != nullptr)
		list->push_back(p);
	list->push_back(list->front());

	return list;
}

double ConvexHull::compare_vectors( SharedPoint p1, SharedPoint p2, SharedPoint p3 )
{
	if(p1->y == p2->y && p2->y == p3->y && 
		(p1->x < p2->x && p2->x < p3->x || p1->x > p2->x && p2->x > p3->x ))
		return 0; // point to the same direction up or down

	return (p2->x - p1->x) - (p3->x - p2->x) * (p2->y - p1->y) / (p3->y - p2->y);
}