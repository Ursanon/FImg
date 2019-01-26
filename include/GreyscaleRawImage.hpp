#ifndef GREYSCALE_RAW_IMAGE_HPP
#define GREYSCALE_RAW_IMAGE_HPP

#include <cstdint>

#include "RawImage.hpp"

namespace bk
{
	class GreyscaleRawImage : public RawImage<GreyscaleColor>
	{
	public:
		GreyscaleRawImage();
		GreyscaleRawImage(const int& width, const int& height);

		void save_to_file(const char * path) override;
		void load_from_file(const char * path, const int& width, const int& height) override;
	};
}
#endif