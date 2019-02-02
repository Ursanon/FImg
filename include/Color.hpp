#ifndef COLOR_HPP
#define COLOR_HPP

namespace bk
{
	struct GreyscaleColor
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

	public:
		uint8_t greyscale;
	};

	struct RGBColor
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

	public:
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};
}

#endif