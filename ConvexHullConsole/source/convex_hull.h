#include "point.h"
#include <vector>
#include <list>
#include <memory>
#include "common.h"

#ifndef CONVEX_HULL_H__
#define CONVEX_HULL_H__

typedef std::list<SharedPoint> PointList;
typedef std::shared_ptr<std::list<SharedPoint> > SharedPointList;

class ConvexHull
{
public:
	ConvexHull(void);
	~ConvexHull(void);

	// Precondition: Points must be sorted by x coordinate
	SharedPointList FindHull(PointQueue &point_queue);

private:
	// more than zero if p1->p2 Points higher than p2->p3
	double compare_vectors(SharedPoint p1, SharedPoint p2, SharedPoint p3);
	void ProcessPoint(SharedPointList list, SharedPoint p, SharedPoint next_p, bool is_top);
};

#endif // CONVEX_HULL_H__
