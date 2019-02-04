#ifndef COLOR_HPP
#define COLOR_HPP

#include <random>

namespace bk
{
	struct Color
	{
	};

	struct GreyscaleColor : Color
	{
		void combine(const GreyscaleColor& rhs)
		{
			this->greyscale = (this->greyscale + rhs.greyscale) >> 1;
		}

		void fill_with_generator(std::mt19937& generator)
		{
			greyscale = generator() & 255;
		}

		static GreyscaleColor peek_combined(const GreyscaleColor& lhs, const GreyscaleColor& rhs)
		{
			GreyscaleColor result(lhs);
			result.combine(rhs);

			return result;
		}

		static double compare(const GreyscaleColor& lhs, const GreyscaleColor& rhs)
		{
			return (rhs.greyscale - lhs.greyscale) * (rhs.greyscale - lhs.greyscale) / 255.0;
		}

	public:
		uint8_t greyscale;
	};

	struct RGBColor : Color
	{
		void combine(const RGBColor& rhs)
		{
			this->r = (this->r + rhs.r) >> 1;
			this->g = (this->g + rhs.g) >> 1;
			this->b = (this->b + rhs.b) >> 1;
		}

		void fill_with_generator(std::mt19937& generator)
		{
			r = generator() & 255;
			g = generator() & 255;
			b = generator() & 255;
		}

		static RGBColor peek_combined(const RGBColor& lhs, const RGBColor& rhs)
		{
			RGBColor result(lhs);
			result.combine(rhs);

			return result;
		}

		static double compare(const RGBColor& lhs, const RGBColor& rhs)
		{
			double r = rhs.r - lhs.r;
			double g = rhs.g - lhs.g;
			double bl = rhs.b - lhs.b;

			return (r * r + g * g + bl * bl) / 255.0 / 3.0;
		}

	public:
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};
}

#endif