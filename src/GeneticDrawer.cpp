#include <cstdio>

#include "GeneticDrawer.hpp"

namespace bk
{
    GeneticDrawer::GeneticDrawer(const GreyscaleRawImage& target, GeneticDrawerSettings settings, const char* output_dir)
        : target_(target), output_dir_(output_dir), settings_(settings)
    {
		current_bests_ = std::vector<GreyscaleRawImage>(settings.bests_count, GreyscaleRawImage(target.get_width(), target.get_height()));
		speciments_ = std::vector<GreyscaleRawImage>(settings.speciments_count, GreyscaleRawImage(target.get_width(), target.get_height()));
    }

    GeneticDrawer::~GeneticDrawer()
    {
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