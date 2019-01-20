#ifndef GENETIC_DRAWER_HPP
#define GENETIC_DRAWER_HPP

#include <cstdint>

#include "RawImage.hpp"

namespace bk
{
    class GeneticDrawer
    {
        public:
            static void start(const RawImage& target);

        private:
            void mutate();
            void cross_over();
    };
}

#endif