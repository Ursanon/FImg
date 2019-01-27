#include <iostream>
#include <chrono>
#include <algorithm>
#include <thread>

#include "GreyscaleDrawer.hpp"

namespace bk
{
	GreyscaleDrawer::GreyscaleDrawer(const RawImage<GreyscaleColor>& target, GeneticDrawerSettings settings, const char* output_dir)
		: GeneticDrawer<GreyscaleColor>(target, settings, output_dir)
	{
	}

	void GreyscaleDrawer::mutate()
	{
		for (size_t i = 0; i < settings_.speciments_count; ++i)
		{
			GreyscaleColor new_color;
			new_color.greyscale = generator_() % 255;

			size_t maxX = target_.get_width() - 1;
			size_t maxY = target_.get_height() - 1;

			size_t start_x = generator_() % maxX;
			size_t x_len = generator_() % (maxX - start_x) + 1;

			size_t start_y = generator_() % maxY;
			size_t y_len = generator_() % (maxY - start_y) + 1;

			size_t parent_index = generator_() % settings_.bests_count;
			RawImage<GreyscaleColor>* parentImage = current_bests_[parent_index];

			for (size_t j = 0; j < y_len; ++j)
			{
				for (size_t k = 0; k < x_len; ++k)
				{
					size_t pixel_x = start_y + j;
					size_t pixel_y = start_x + k;

					GreyscaleColor pixel_color = GreyscaleColor::combine(parentImage->get_pixel(pixel_x, pixel_y), new_color);
					speciments_[i]->set_pixel(pixel_x, pixel_y, pixel_color);
				}
			}
		}
	}

	void GreyscaleDrawer::evaluate()
	{
#ifdef BENCHMARK_TIME
		time_end = std::chrono::system_clock::now();
#endif
			Rating* rating = new Rating[settings_.speciments_count];
			
			std::vector<std::thread> threads;
			for (size_t i = 0; i < settings_.speciments_count; ++i)
			{
				threads.push_back(std::thread([&rating, this, i]() -> void
				{
					double diff = 0.f;

					size_t size = target_.get_size();
					for (size_t j = 0; j < size; ++j)
					{
						double a = speciments_[i]->get_pixel(j).greyscale;
						double b = target_.get_pixel(j).greyscale;
						diff += (a - b) * (a - b);
					}

					rating[i].index = i;
					rating[i].rate = diff;
				})
				);
			}

			for (auto&& t : threads)
				t.join();

#ifdef BENCHMARK_TIME
			time_end = std::chrono::system_clock::now();
			auto dur = time_end - time_start;
			auto secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
			std::cout << "\ncalc rating time: " << secs.count() << " [s]";
#endif

			//todo: benchmark time!
			sort_ranking(rating, settings_.speciments_count);

			for (size_t i = 0; i < settings_.bests_count; ++i)
			{
				current_bests_[i]->copy_pixels_from(*speciments_[rating[i].index]);
			}

			delete[] rating;
	}

#ifdef BENCHMARK_TIME
	static std::chrono::time_point<std::chrono::system_clock> time_start, time_end;
#endif
}