#ifndef GREYSCALE_RAW_IMAGE_HPP
#define GREYSCALE_RAW_IMAGE_HPP

#include <cstdint>

#include "RawImage.hpp"

namespace bk
{
    class GreyscaleRawImage : public RawImage
    {
        public:
            GreyscaleRawImage(const char * path, const int& width, const int& height);
            ~GreyscaleRawImage();

            void save_to_file(const char * path) override;
            
        private:
            void load_from_file(const char * path) override;

        public:
            int width_;
            int height_;
            size_t size_;
            uint8_t* image_;
    };
}
#endif