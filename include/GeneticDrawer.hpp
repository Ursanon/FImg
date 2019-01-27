#ifndef GENETIC_DRAWER_HPP
#define GENETIC_DRAWER_HPP

#include <string>
#include <vector>
#include <random>

#include "GreyscaleRawImage.hpp"

namespace bk
{
    struct GeneticDrawerSettings
    {
        GeneticDrawerSettings(uint32_t speciments, uint32_t bests)
            : speciments_count(speciments), bests_count(bests) 
        {
        }

        uint32_t speciments_count;
        uint32_t bests_count;
    };

	struct Rating
	{
		size_t index;
		double rate;
	};

    class GeneticDrawer
    {
        public:
            GeneticDrawer(const GreyscaleRawImage& target, const GeneticDrawerSettings settings, const char* output_dir);
            ~GeneticDrawer();
            void start();

        private:
            void mutate(int id);
			void evaluate();
            void cross_over();

			void sort_ranking(Rating * rating, size_t elements_count);

        private:
			std::mt19937 generator_;

            GeneticDrawerSettings settings_;
            std::string output_dir_;

			std::vector<GreyscaleRawImage*> current_bests_;
			std::vector<GreyscaleRawImage*> speciments_;

			GreyscaleRawImage target_;
    };
}

#endif