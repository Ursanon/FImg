#ifndef COLOR_HPP
#define COLOR_HPP

namespace bk
{
	struct Color
	{
	};

	struct GreyscaleColor : Color
	{
	public:
		void combine(const GreyscaleColor& rhs)
		{
			this->greyscale = (this->greyscale + rhs.greyscale) >> 1;
		}

		static GreyscaleColor combine(const GreyscaleColor& lhs, const GreyscaleColor& rhs)
		{
			GreyscaleColor result;
			result.greyscale = (lhs.greyscale + rhs.greyscale) >> 1;

			return result;
		}

	public:
		uint8_t greyscale;
	};

	struct RGBColor : Color
	{
	public:
		void combine(const RGBColor& rhs)
		{
			this->r = (this->r + rhs.r) >> 1;
			this->g = (this->g + rhs.g) >> 1;
			this->b = (this->b + rhs.b) >> 1;
		}

		static RGBColor combine(const RGBColor& lhs, const RGBColor& rhs)
		{
			RGBColor result;
			result.r = (lhs.r + rhs.r) >> 1;
			result.g = (lhs.g + rhs.g) >> 1;
			result.b = (lhs.b + rhs.b) >> 1;

			return result;
		}

	public:
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};
}

#endif