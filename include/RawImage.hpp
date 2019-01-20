#ifndef RAW_IMAGE_HPP
#define RAW_IMAGE_HPP

namespace bk
{
    class RawImage
    {
        public:
            virtual void save_to_file(const char * path) = 0;
        protected:
            virtual void load_from_file(const char * path, const int& width, const int& height) = 0;
    };
} // bk

#endif