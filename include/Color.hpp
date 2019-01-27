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
}

#endif