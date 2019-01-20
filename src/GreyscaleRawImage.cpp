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
		delete[] image_;
        image_ = new uint8_t[size_];
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

	const size_t & GreyscaleRawImage::get_size()
	{
		return size_;
	}

    const int& GreyscaleRawImage::get_height()
    {
        return width_;
    }

    const int& GreyscaleRawImage::get_width()
    {
        return height_;
    }

    void GreyscaleRawImage::save_to_file(const char * file_path)
    {
        const char* file_mode = "wb";
        const int elements_count = 1;

        printf("save under: %s!", file_path);

		FILE * file = fopen(file_path, file_mode);
        if(file == nullptr)
        {
            printf("Cant save: file null!");
			return;
        }
        
        for(size_t i = 0; i < size_; ++i)
        {
            fwrite(&image_[i], sizeof(uint8_t), elements_count, file);
        }
        fclose(file);
    }

    void GreyscaleRawImage::load_from_file(const char * path, const int& width, const int& height)
    {
		GreyscaleRawImage(width, height);

        const char* file_mode = "rb";
        const int elements_count = 1;

		FILE * file = fopen(path, file_mode);
        if(file == nullptr)
        {
            printf("Cant load: file null!");
			return;
        }
        
        for(size_t i = 0; i < size_; ++i)
        {
            fread(&image_[i], sizeof(uint8_t), elements_count, file);
        }
        fclose(file);
    }
}