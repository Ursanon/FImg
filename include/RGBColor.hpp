#ifndef RGB_COLOR_HPP
#define RGB_COLOR_HPP

#include "IColor.hpp"

namespace bk
{
	struct RGBColorData
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	struct RGBColor : IColor<RGBColor, RGBColorData>
	{
		virtual void combine(const RGBColor &rhs) override
		{
			this->data.r = (this->data.r + rhs.data.r) >> 1;
			this->data.g = (this->data.g + rhs.data.g) >> 1;
			this->data.b = (this->data.b + rhs.data.b) >> 1;
		}

		virtual void fill_with_generator(std::mt19937 &generator) override
		{
			data.r = generator() & 255;
			data.g = generator() & 255;
			data.b = generator() & 255;
		}

		virtual RGBColor peek_combined(const RGBColor &rhs) override
		{
			RGBColor result(*this);
			result.combine(rhs);

			return result;
		}

		virtual double compare(const RGBColor &rhs) override
		{
			double rc = rhs.data.r - data.r;
			double gc = rhs.data.g - data.g;
			double bl = rhs.data.b - data.b;

			return (rc * rc + gc * gc + bl * bl) / 255.0 / 3.0;
		}
	};
} // namespace bk

#endif