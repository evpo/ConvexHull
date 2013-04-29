#include "line.h"

void Line::Draw( Point source, Point target, PointLoader &point_loader )
{
	if(source.x == target.x || source.y == target.y)
	{
		DrawFlatLine(source, target, point_loader);
		return;
	}

	// find two middle points
	// |
	// | <- first
	//  | <- second
	//  |

	Point *left, *right;
	Point *top, *bottom;

	bool is_source_left = target.x > source.x;
	bool is_source_top = source.y > target.y;

	if(is_source_left)
	{
		left = &source;
		right = &target;
	}
	else
	{
		left = &target;
		right = &source;
	}

	if(is_source_top)
	{
		top = &source;
		bottom = &target;
	}
	else
	{
		top = &target;
		bottom = &source;
	}

	bool is_left_top = left->y > right->y;

	double left_neighbour, right_neighbour, top_neighbour, bottom_neighbour;

	double middle_x = left->x + (right->x - left->x)/2;
	double middle_y = bottom->y + (top->y - bottom->y)/2;

	left_neighbour = floor(middle_x);
	right_neighbour = ceil(middle_x);
	if(left_neighbour == right_neighbour)
	{
		right_neighbour ++;
	}

	top_neighbour = ceil(middle_y);
	bottom_neighbour = floor(middle_y);
	if(top_neighbour == bottom_neighbour)
	{
		if(is_left_top)
			bottom_neighbour--;
		else
			top_neighbour++;
	}

	Point left_friend, right_friend;
	

	left_friend.x = left_neighbour;
	right_friend.x = right_neighbour;

	if(is_left_top)
	{
		left_friend.y = top_neighbour;
		right_friend.y = bottom_neighbour;
	}
	else // left is at the bottom
	{
		left_friend.y = bottom_neighbour;
		right_friend.y = top_neighbour;
	}

	Draw(*left, left_friend, point_loader);
	Draw(right_friend, *right, point_loader);
}

void Line::DrawFlatLine( Point source, Point target, PointLoader &point_loader )
{
	double constant_component, other_component_target, other_component_source;
	bool vertical = source.x == target.x;
	constant_component = vertical ? source.x : source.y;
	other_component_source = vertical ? source.y : source.x;
	other_component_target = vertical ? target.y : target.x;

	double first = other_component_source < other_component_target ? 
		other_component_source : other_component_target;
	double last = other_component_source < other_component_target ? 
		other_component_target : other_component_source;

	for(double p = first; p <= last; p ++)
	{
		Point point;
		point.x = vertical ? constant_component : p;
		point.y = vertical ? p : constant_component;

		point_loader.Put(point);
	}
}

