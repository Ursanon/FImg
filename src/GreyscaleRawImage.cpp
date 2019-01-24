#include <cstdint>
#include <iostream>

#include "GreyscaleRawImage.hpp"

namespace bk
{
    GreyscaleRawImage::GreyscaleRawImage()
    {
    }
    
    GreyscaleRawImage::GreyscaleRawImage(const int& width, const int& height)
        :  width_(width), height_(height), size_(width * height)
    {
		image_ = new uint8_t[size_] {};
    }

    GreyscaleRawImage::~GreyscaleRawImage()
    {
        delete[] image_;
        image_ = nullptr;
    }

	uint8_t* const GreyscaleRawImage::get_image()
	{
		return image_;
	}

	const size_t GreyscaleRawImage::get_size() const
	{
		return size_;
	}

	const int GreyscaleRawImage::get_height() const
    {
        return width_;
    }

	const int GreyscaleRawImage::get_width() const
    {
        return height_;
    }

    void GreyscaleRawImage::save_to_file(const char * file_path)
    {
        const char* file_mode = "wb";
        const int elements_count = 1;

		FILE * file = fopen(file_path, file_mode);
        if(file == nullptr)
        {
			//todo: add error handling
            printf("Cant save: file null!");
			return;
        }
        
		fwrite(image_, sizeof(uint8_t), size_, file);
		
        fclose(file);

        printf("saved under: %s!", file_path);
    }

	void GreyscaleRawImage::load_from_file(const char * path, const int& width, const int& height)
    {
		width_ = width;
		height_ = height;
		size_ = width * height;

		if (image_ != nullptr)
		{
			delete[] image_;
		}
		image_ = new uint8_t[size_];

        const char* file_mode = "rb";
        const int elements_count = 1;

		FILE * file = fopen(path, file_mode);
        if(file == nullptr)
        {
			//todo: add error handling
            printf("Cant load: file null!");
			return;
        }
        
        fread(image_, sizeof(uint8_t), size_, file);
        
        fclose(file);
    }
}