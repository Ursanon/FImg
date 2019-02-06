#ifndef COLOR_HPP
#define COLOR_HPP

#include <random>
#include <type_traits>

namespace bk
{
	template <typename TColor, typename TData>
	struct IColor
	{
		static_assert(std::is_pod<TData>::value, "TData must be POD!");

		virtual void combine(const TColor& rhs) = 0;
		virtual TColor peek_combined(const TColor& rhs) = 0;
		virtual double compare(const TColor& rhs) = 0;
		virtual void fill_with_generator(std::mt19937& generator) = 0;

		TData* get_data()
		{
			return &data;
		}

	protected:
		TData data;
	};
}

#endif