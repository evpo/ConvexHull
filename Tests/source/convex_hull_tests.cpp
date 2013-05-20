#include <vector>
#include <iterator>
#include <algorithm>
#include <set>
#include "gtest/gtest.h"
#include "point.h"
#include "convex_hull.h"

using namespace std;

namespace {
class FindHullFxt : public ::testing::Test
{
public:
	FindHullFxt():out_it_(output_)
	{
	}

protected:
	vector<Point> output_;
	vector<Point> input_;
	back_insert_iterator<vector<Point> > out_it_;

	vector<Point> SetDifferenceBetweenInputOutput()
	{
		vector<Point> difference;
		back_insert_iterator<vector<Point> > inserter(difference);
		set<Point> in_set(input_.begin(), input_.end()), 
			out_set(output_.begin(), output_.end());

		set_difference(in_set.begin(), in_set.end(), out_set.begin(), out_set.end(), inserter);
		return difference;
	}

	bool Contains(Point p)
	{
		return find(output_.begin(), output_.end(), p) != output_.end();
	}
};


TEST_F(FindHullFxt, NonePoints)
{
	FindHull(input_.begin(), input_.end(), out_it_);
	ASSERT_EQ(0U, output_.size());
}

TEST_F(FindHullFxt, OnePoint)
{
	input_.push_back(Point(1.,1.));
	FindHull(input_.begin(), input_.end(), out_it_);
	ASSERT_EQ(1, output_.size());
	EXPECT_EQ(1., output_[0].x);
	EXPECT_EQ(1., output_[0].y);
}

TEST_F(FindHullFxt, TwoPoints)
{
	input_.push_back(Point(1.,1.));
	input_.push_back(Point(2.,2.));
	FindHull(input_.begin(), input_.end(), out_it_);
	ASSERT_GE(output_.size(), 2U);
	ASSERT_LE(output_.size(), 3U);
	
	ASSERT_EQ(0U, SetDifferenceBetweenInputOutput().size());
}

TEST_F(FindHullFxt, ThreePoints)
{
	input_.push_back(Point(1.,1.));
	input_.push_back(Point(2.,2.));
	input_.push_back(Point(4.,2.));
	FindHull(input_.begin(), input_.end(), out_it_);
	ASSERT_EQ(0U, SetDifferenceBetweenInputOutput().size());
}

TEST_F(FindHullFxt, TenPointsSquare)
{
	for(double y = 0.; y < 10; ++ y)
	for(double x = 0.; x < 10; ++ x)
	{
		input_.push_back(Point(x, y));
	}

	FindHull(input_.begin(), input_.end(), out_it_);

	ASSERT_EQ(5U, output_.size());

	EXPECT_TRUE(Contains(Point(0., 0.)));
	EXPECT_TRUE(Contains(Point(0., 9.)));
	EXPECT_TRUE(Contains(Point(9., 9.)));
	EXPECT_TRUE(Contains(Point(9., 0.)));
}

}