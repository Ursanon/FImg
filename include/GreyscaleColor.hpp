#ifndef GREYSCALE_COLOR_HPP
#define GREYSCALE_COLOR_HPP

#include "IColor.hpp"

namespace bk
{
	struct GreyscaleColorData
	{
		uint8_t greyscale;
	};

	struct GreyscaleColor : public IColor<GreyscaleColor, GreyscaleColorData>
	{
		virtual void combine(const GreyscaleColor& rhs) override
		{
			this->data.greyscale = (this->data.greyscale + rhs.data.greyscale) >> 1;
		}

		virtual void fill_with_generator(std::mt19937& generator) override
		{
			data.greyscale = generator() & 255;
		}

		virtual GreyscaleColor peek_combined(const GreyscaleColor& rhs) override
		{
			GreyscaleColor result(*this);
			result.combine(rhs);

			return result;
		}

		virtual double compare(const GreyscaleColor& rhs) override
		{
			return (rhs.data.greyscale - data.greyscale) * (rhs.data.greyscale - data.greyscale) / 255.0;
		}
	};
} // bk

#endif