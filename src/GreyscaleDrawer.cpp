#include <iostream>
#include <chrono>
#include <algorithm>
#include <thread>

#include "GreyscaleDrawer.hpp"

namespace bk
{
	GreyscaleDrawer::GreyscaleDrawer(const RawImage<GreyscaleColor>& target, Settings settings, const char* output_dir)
		: GeneticDrawer<GreyscaleColor>(target, settings, output_dir)
	{
	}

	void GreyscaleDrawer::mutate()
	{
		size_t maxX = target_->get_width() - 1;
		size_t maxY = target_->get_height() - 1;

		for (size_t i = 0; i < settings_.specimens_count; ++i)
		{
			GreyscaleColor new_color;
			new_color.greyscale = generator_() % 255;

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
					specimens_[i]->set_pixel(pixel_x, pixel_y, pixel_color);
				}
			}
		}
	}

	void GreyscaleDrawer::evaluate()
	{
#ifdef BENCHMARK_TIME
		time_end = std::chrono::system_clock::now();
#endif			
		std::vector<std::thread> threads;
		for (size_t i = 0; i < settings_.specimens_count; ++i)
		{
			threads.push_back(std::thread([this, i]() -> void
			{
				double diff = 0.f;

				size_t size = target_->get_size();
				for (size_t j = 0; j < size; ++j)
				{
					double a = specimens_[i]->get_pixel(j).greyscale;
					double b = target_->get_pixel(j).greyscale;
					diff += (b - a) * (b - a);
				}

				rating_[i].index = i;
				rating_[i].rate = diff;
			})
			);
		}

		for (auto&& t : threads)
			t.join();

		std::sort(rating_, rating_ + settings_.specimens_count, [](const Rating& a, const Rating& b) -> bool { return a.rate < b.rate; });

		for (size_t i = 0; i < settings_.bests_count; ++i)
		{
			current_bests_[i]->copy_pixels_from(*specimens_[rating_[i].index]);
		}
	}

#ifdef BENCHMARK_TIME
	static std::chrono::time_point<std::chrono::system_clock> time_start, time_end;
#endif
	RGBDrawer::RGBDrawer(const RawImage<RGBColor>& target, Settings settings, const char * output_dir)
		: GeneticDrawer<RGBColor>(target, settings, output_dir)
	{
	}

	void RGBDrawer::mutate()
	{
		size_t maxX = target_->get_width() - 1;
		size_t maxY = target_->get_height() - 1;

		for (size_t i = 0; i < settings_.specimens_count; ++i)
		{
			RGBColor new_color;
			new_color.r = generator_() % 255;
			new_color.g = generator_() % 255;
			new_color.b = generator_() % 255;

			size_t start_x = generator_() % maxX;
			size_t x_len = generator_() % (maxX - start_x) + 1;

			size_t start_y = generator_() % maxY;
			size_t y_len = generator_() % (maxY - start_y) + 1;

			size_t parent_index = generator_() % settings_.bests_count;
			RawImage<RGBColor>* parentImage = current_bests_[parent_index];

			for (size_t j = 0; j < y_len; ++j)
			{
				for (size_t k = 0; k < x_len; ++k)
				{
					size_t pixel_x = start_y + j;
					size_t pixel_y = start_x + k;

					RGBColor pixel_color = RGBColor::combine(parentImage->get_pixel(pixel_x, pixel_y), new_color);
					specimens_[i]->set_pixel(pixel_x, pixel_y, pixel_color);
				}
			}
		}
	}

	void RGBDrawer::evaluate()
	{
		std::vector<std::thread> threads;
		for (size_t i = 0; i < settings_.specimens_count; ++i)
		{
			threads.push_back(std::thread([this, i]() -> void
			{
				double diff = 0.f;

				size_t size = target_->get_size();
				for (size_t j = 0; j < size; ++j)
				{
					RGBColor a = specimens_[i]->get_pixel(j);
					RGBColor b = target_->get_pixel(j);

					double r = b.r - a.r;
					double g = b.g - a.g;
					double bl = b.b - a.b;

					diff += (r * r + g * g + bl * bl)/ (255.0 * size);
				}

				rating_[i].index = i;
				rating_[i].rate = diff;
			})
			);
		}

		for (auto&& t : threads)
			t.join();

		std::sort(rating_, rating_ + settings_.specimens_count, [](const Rating& a, const Rating& b) -> bool { return a.rate < b.rate; });

		for (size_t i = 0; i < settings_.bests_count; ++i)
		{
			current_bests_[i]->copy_pixels_from(*specimens_[rating_[i].index]);
		}
	}
}