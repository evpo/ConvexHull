#include <vector>
#include <iterator>
#include <algorithm>
#include "gtest/gtest.h"
#include "point.h"
#include "point_iterator.h"

using namespace std;

template <typename It>
class FakePointProvider
{
public:
	FakePointProvider(It begin, It end)
	{
		back_insert_iterator<vector<Point> > back_inserter(points_);
		copy(begin, end, back_inserter);
		it_ = points_.begin();
	}

	bool NextPoint(Point &point)
	{
		if(!*this)
			return false;

		point = *it_;
		it_ ++;
		return true;
	}

	operator bool() const
	{
		return it_ != points_.end();
	}

private:
	vector<Point> points_;
	vector<Point>::const_iterator it_;
};

TEST(point_iterator, empty_provider)
{
	typedef FakePointProvider<Point*> FakeProvider;
	Point *points = NULL;
	FakeProvider provider(points, points);
	
	PointIterator<FakeProvider> it(provider);
	PointIterator<FakeProvider> end_it;

	ASSERT_EQ(end_it, it);
}

TEST(point_iterator, providing_one_point)
{
	typedef FakePointProvider<Point*> FakeProvider;
	Point points[] = {
		Point(1.0, 2.0)
	};
	FakeProvider provider(&points[0], &points[1]);

	PointIterator<FakeProvider> it(provider);
	PointIterator<FakeProvider> end_it;

	ASSERT_NE(end_it, it);
	
	ASSERT_EQ(1.0, (*it).x);
	ASSERT_EQ(2.0, (*it).y);
	
	it ++;
	ASSERT_EQ(end_it, it);
}

TEST(point_iterator, providing_many_points)
{
	typedef FakePointProvider<Point*> FakeProvider;
	Point points[] = {
		Point(1.0, 2.0), 
		Point(3.0, 4.0), 
		Point(5.0, 6.0), 
		Point(7.0, 8.0), 
		Point(9.0, 10.0)
	};

	FakeProvider provider(&points[0], &points[5]);

	PointIterator<FakeProvider> it(provider);
	PointIterator<FakeProvider> end_it;

	ASSERT_NE(end_it, it);
	ASSERT_EQ(1.0, (*it).x);
	ASSERT_EQ(2.0, (*it).y);

	it ++;
	ASSERT_NE(end_it, it);
	ASSERT_EQ(3.0, (*it).x);
	ASSERT_EQ(4.0, (*it).y);

	it ++;
	ASSERT_NE(end_it, it);
	ASSERT_EQ(5.0, (*it).x);
	ASSERT_EQ(6.0, (*it).y);

	it ++;
	ASSERT_NE(end_it, it);
	ASSERT_EQ(7.0, (*it).x);
	ASSERT_EQ(8.0, (*it).y);

	it ++;
	ASSERT_NE(end_it, it);
	ASSERT_EQ(9.0, (*it).x);
	ASSERT_EQ(10.0, (*it).y);

	it ++;
	ASSERT_EQ(end_it, it);
}
