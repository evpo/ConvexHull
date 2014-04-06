#ifndef point_iterator_h__
#define point_iterator_h__

#include <iterator>
#include "assert.h"
// PointProvider access points:
// PointInfo PointProvider::NextPoint()
// bool PointProvider::operator bool()
// operator bool() returns false when EOF or points are not available
template<typename PointProvider, typename PointInfoExtractor>
class PointIterator
{
public:
	typedef typename PointInfoExtractor::point_info PointInfo;
	typedef char difference_type;
	typedef PointInfo value_type;
	typedef PointInfo &reference;
	typedef PointInfo *pointer;
	typedef std::input_iterator_tag iterator_category;

private:
	
	PointProvider *point_provider_;
	PointInfo value_;

public:
	// point_provider should be pointing to the starting point already
	PointIterator(PointProvider &point_provider):point_provider_(&point_provider)
	{
		if(point_provider_ != nullptr && !*point_provider_)
		{
			point_provider_ = nullptr;
		}

		if(point_provider_ != nullptr)
		{
			value_ = PointInfoExtractor::GetPointInfo(*point_provider_);
		}
	};

	PointIterator(const PointIterator &copy)
	{
		value_ = copy.value_;
		point_provider_ = copy.point_provider_;
	}

	PointIterator():point_provider_(nullptr){};

	const PointIterator &operator=(const PointIterator &rhs)
	{
		value_ = rhs.value_;
		point_provider_ = rhs.point_provider_;
	}

	PointIterator &operator++()
	{
		if(!point_provider_)
			return *this;

		if(point_provider_->NextPoint())
		{
			value_ = PointInfoExtractor::GetPointInfo(*point_provider_);
		}
		else
		{
			point_provider_ = nullptr;
		}

		return *this;
	}

	PointIterator operator++(int)
	{
		assert(point_provider_);
		PointIterator tmp = *this;
		++ *this;
		return tmp;
	}

	PointInfo &operator*()
	{
		assert(point_provider_);
		return value_;
	}

	PointInfo *operator->()
	{
		return &(this->operator*());
	}

	bool operator==(const PointIterator<PointProvider, PointInfoExtractor> &rhs) const
	{
		// one of point providers must be nullptr. Comparison of non null point providers is not supported
		assert(!point_provider_ || !rhs.point_provider_);

		return !point_provider_ && !rhs.point_provider_;
	}

	bool operator!=(const PointIterator<PointProvider, PointInfoExtractor> &rhs) const
	{
		return !(*this == rhs);
	}

};

#endif // point_iterator_h__
