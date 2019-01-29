#ifndef GREYSCALE_GENETIC_DRAWER_HPP
#define GREYSCALE_GENETIC_DRAWER_HPP

#include "GeneticDrawer.hpp"

namespace bk
{
    class GreyscaleDrawer : public GeneticDrawer<GreyscaleColor>
    {
        public:
            GreyscaleDrawer(const RawImage<GreyscaleColor>& target, Settings settings, const char* output_dir);

        private:
            virtual void mutate() override;
			virtual void evaluate() override;
    };
}

#endif