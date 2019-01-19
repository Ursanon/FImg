#include <cstdio>

#include "GeneticDrawer.hpp"

namespace bk
{
    GreyscaleRawImage::GreyscaleRawImage(const char * filePath, const int& width, const int& height)
        :  m_width(width), m_height(height), m_rawElements(width * height)
    {
        m_image = new uint8_t[m_rawElements];
		LoadFromFile(filePath);
    }

    GreyscaleRawImage::~GreyscaleRawImage()
    {
        delete[] m_image;
        m_image = nullptr;
    }

    void GreyscaleRawImage::SaveToFile(const char * filePath)
    {
        const char* fileMode = "wb";
        const int elementsCount = 1;

        printf("save under: %s!", filePath);

        FILE * file = fopen(filePath, fileMode);
        if(file == nullptr)
        {
            printf("Cant save: file null!");
        }
        
        for(size_t i = 0; i < m_rawElements; ++i)
        {
            fwrite(&m_image[i], sizeof(uint8_t), elementsCount, file);
        }
        fclose(file);
    }

    void GreyscaleRawImage::LoadFromFile(const char * filePath)
    {
        const char* fileMode = "rb";
        const int elementsCount = 1;

        FILE * file = fopen(filePath, fileMode);
        if(file == nullptr)
        {
            printf("Cant load: file null!");
        }
        
        for(size_t i = 0; i < m_rawElements; ++i)
        {
            fread(&m_image[i], sizeof(uint8_t), elementsCount, file);
        }
        fclose(file);
    }

    void GeneticDrawer::Start(const RawImage& target)
    {

    }
}