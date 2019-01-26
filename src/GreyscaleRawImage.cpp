#include <cstdint>
#include <iostream>

#include "GreyscaleRawImage.hpp"

namespace bk
{
	GreyscaleRawImage::GreyscaleRawImage()
	{
	}

	GreyscaleRawImage::GreyscaleRawImage(const int& width, const int& height)
		: RawImage(width, height)
	{
	}

	void GreyscaleRawImage::save_to_file(const char * file_path)
	{
		const char* file_mode = "wb";
		const int elements_count = 1;

		FILE * file = fopen(file_path, file_mode);
		if (file == nullptr)
		{
			//todo: add error handling
			printf("Cant save: file null!");
			return;
		}

		fwrite(image_, sizeof(GreyscaleColor), size_, file);

		fclose(file);

		printf("saved under: %s!", file_path);
	}

	void GreyscaleRawImage::load_from_file(const char * path, const int& width, const int& height)
	{
		const char* file_mode = "rb";

		width_ = width;
		height_ = height;

		size_t newSize = width * height;
		if (size_ != newSize)
		{
			size_ = newSize;

			if (image_ != nullptr)
			{
				delete[] image_;
			}
			image_ = new GreyscaleColor[size_];
		}

		FILE * file = fopen(path, file_mode);
		if (file == nullptr)
		{
			//todo: add error handling
			printf("Cant load: file null!");
			return;
		}

		fread(image_, sizeof(GreyscaleColor), size_, file);

		fclose(file);
	}
}