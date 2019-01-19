#ifndef GENETIC_DRAWER_HPP
#define GENETIC_DRAWER_HPP

#include <cstdint>

namespace bk
{
    class RawImage
    {
        public:
            virtual void SaveToFile(const char * filePath) = 0;
        protected:
            virtual void LoadFromFile(const char * filePath) = 0;
    };

    class GreyscaleRawImage : public RawImage
    {
        public:
            GreyscaleRawImage(const char * filePath, const int& width, const int& height);
            ~GreyscaleRawImage();

            void SaveToFile(const char * filePath) override;

        private:
            void LoadFromFile(const char * filePath) override;

        public:
            int m_width;
            int m_height;
            size_t m_rawElements;
            uint8_t* m_image;
    };

    class GeneticDrawer
    {
        public:
            static void Start(const RawImage& target);
    };
}

#endif