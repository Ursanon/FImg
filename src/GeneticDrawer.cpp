#include <cstdio>

#include "GeneticDrawer.hpp"

namespace bk
{
    GeneticDrawer::GeneticDrawer(const GreyscaleRawImage& target, GeneticDrawerSettings settings, const char* output_dir)
        : target_(target), output_dir_(output_dir), settings_(settings)
    {
        current_bests_ = new GreyscaleRawImage[settings_.speciments_count];
    }

    GeneticDrawer::~GeneticDrawer()
    {
        delete[] current_bests_;
        current_bests_ = nullptr;

        delete[] speciments_;
        speciments_ = nullptr;
    }

    void GeneticDrawer::start()
    {
        bool in_progress = true;
        uint64_t generation_number = 0;
        while(in_progress)
        {
            mutate();

            cross_over();

            ++generation_number;
            
            if(generation_number % 50000 == 0)
            {
                printf("\nsaving : %u generation...", generation_number);
            }
        }
    }

    void GeneticDrawer::mutate()
    {

    }

    void GeneticDrawer::cross_over()
    {

    }
}