#ifndef GENETIC_DRAWER_HPP
#define GENETIC_DRAWER_HPP

#include <string>
#include <vector>

#include "GreyscaleRawImage.hpp"

namespace bk
{
    struct GeneticDrawerSettings
    {
        GeneticDrawerSettings(uint32_t speciments, uint32_t parents, uint32_t bests)
            : speciments_count(speciments), parents_count(parents), bests_count(bests) 
        {
        }

        uint32_t speciments_count;
        uint32_t parents_count;
        uint32_t bests_count;
    };

    class GeneticDrawer
    {
        public:
            GeneticDrawer(const GreyscaleRawImage& target, GeneticDrawerSettings settings, const char* output_dir);
            ~GeneticDrawer();
            void start();

        private:
            void mutate();
            void cross_over();

        private:
            GeneticDrawerSettings settings_;
            std::string output_dir_;

			std::vector<GreyscaleRawImage> current_bests_;
			std::vector<GreyscaleRawImage> speciments_;

			GreyscaleRawImage target_;
    };
}

#endif