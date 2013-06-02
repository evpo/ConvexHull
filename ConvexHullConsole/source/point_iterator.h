#include "point.h"

#ifndef point_iterator_h__
#define point_iterator_h__

// PointProvider access points:
// bool PointProvider::NextPoint(Point &point)
// operator bool() returns false when EOF or points are not available
template<typename PointProvider>
class PointIterator
{
public:
	typedef char difference_type;
	typedef Point value_type;
	typedef Point &reference;
	typedef Point *pointer;
	typedef std::output_iterator_tag iterator_category;

	PointIterator(PointProvider &point_provider):point_provider_(&point_provider)
	{
		if(point_provider_ != NULL && !*point_provider_)
		{
			point_provider_ = NULL;
		}

		if(point_provider_ != NULL)
			point_provider_->NextPoint(value_);

	};

	PointIterator(const PointIterator &copy)
	{
		value_ = copy.value_;
		point_provider_ = copy.point_provider_;
	}

	PointIterator():point_provider_(NULL){};

	const PointIterator &operator=(const PointIterator &rhs)
	{
		value_ = rhs.value_;
		point_provider_ = rhs.point_provider_;
	}

	PointIterator &operator++()
	{
		if(!point_provider_)
			return *this;

		if(!*point_provider_)
		{
			value_ = Point();
			point_provider_ = NULL;
			return *this;
		}

		point_provider_->NextPoint(value_);

		return *this;
	}

	PointIterator operator++(int)
	{
		PointIterator tmp = *this;
		++ *this;
		return tmp;
	}

	Point &operator*()
	{
		return value_;
	}

	bool operator==(const PointIterator<PointProvider> &rhs) const
	{
		return point_provider_ == rhs.point_provider_ && value_ == rhs.value_;
	}

	bool operator!=(const PointIterator<PointProvider> &rhs) const
	{
		return !(*this == rhs);
	}

private:
	PointProvider *point_provider_;
	Point value_;
};

//template <typename PointProvider>
//bool operator==(const PointIterator<PointProvider> &lhs, const PointIterator<PointProvider> &rhs)
//{
//	return lhs.point_provider_ == rhs.point_provider_ && lhs.value_ == rhs.value_;
//}

#endif // point_iterator_h__
