#ifndef color_filtering_h__
#define color_filtering_h__

template <
	typename Color, 
	typename Renderer, 
	typename PointLoaderT = PointLoader<Color, Renderer>, 
	typename ColorEvaluatorT = ColorEvaluator<Color>
>
class ColorFilteredPointProvider
{
private:
	ColorEvaluatorT &evaluator_;
	PointLoaderT &point_loader_;
	const ColorFilteredPointProvider &operator=(const ColorFilteredPointProvider&);
public:
	ColorFilteredPointProvider(ColorEvaluatorT &evaluator, PointLoaderT &point_loader)
		:evaluator_(evaluator), point_loader_(point_loader)
	{
	}

	bool NextPoint()
	{
		while(point_loader_.NextPoint() && !evaluator_.TestColor(point_loader_.GetColor()));
		return point_loader_;
	}

	Point GetPoint() const
	{
		assert(*this);
		return point_loader_.GetPoint();
	}

	operator bool() const
	{
		return point_loader_;
	}


};

#endif