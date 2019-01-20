#ifndef GREYSCALE_RAW_IMAGE_HPP
#define GREYSCALE_RAW_IMAGE_HPP

#include <cstdint>

#include "RawImage.hpp"

namespace bk
{
    class GreyscaleRawImage : public RawImage
    {
        public:
            GreyscaleRawImage();
            GreyscaleRawImage(const int& width, const int& height);
            
            ~GreyscaleRawImage();

            uint8_t* const get_image();
            const size_t get_size() const ;
            const int get_height() const ;
            const int get_width() const;

            void save_to_file(const char * path) override;
            void load_from_file(const char * path, const int& width, const int& height) override;

        private:
            int width_;
            int height_;
            size_t size_;
			uint8_t* image_ = nullptr;
    };
}
#endif