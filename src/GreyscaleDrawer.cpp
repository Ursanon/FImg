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

	void GreyscaleDrawer::evaluate()
	{
		size_t elements = settings_.specimens_count / settings_.thread_count;
		size_t rest = settings_.specimens_count % settings_.thread_count;

		size_t start_index = 0;
		size_t end_index = 0;

		auto rate = [&](const size_t start, const size_t end) -> void
			{
				for (size_t i = start; i < end; ++i)
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
				}
			};

		std::vector<std::thread> threads;

		while(end_index < settings_.specimens_count)
		{
			end_index += elements;
			if (rest != 0)
			{
				end_index++;
				rest--;
			}

			threads.push_back(std::thread(rate, start_index, end_index));

			start_index += elements;
		}

		size_t threads_count = threads.size();
		for (size_t i = 0; i < threads_count; ++i)
		{
			threads[i].join();
		}

		std::sort(rating_, rating_ + settings_.specimens_count, [](const Rating& a, const Rating& b) -> bool { return a.rate < b.rate; });

		for (size_t i = 0; i < settings_.bests_count; ++i)
		{
			current_bests_[i]->copy_pixels_from(*specimens_[rating_[i].index]);
		}
	}

	RGBDrawer::RGBDrawer(const RawImage<RGBColor>& target, Settings settings, const char * output_dir)
		: GeneticDrawer<RGBColor>(target, settings, output_dir)
	{
	}

	void RGBDrawer::evaluate()
	{
		size_t elements = settings_.specimens_count / settings_.thread_count;
		size_t rest = settings_.specimens_count % settings_.thread_count;

		size_t start_index = 0;
		size_t end_index = 0;
		
		auto rate = [&](const size_t start, const size_t end) -> void
		{
			for (size_t i = start; i < end; ++i)
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

					diff += (r * r + g * g + bl * bl) / (255.0 * size);
				}

				rating_[i].index = i;
				rating_[i].rate = diff;
			}
		};

		std::vector<std::thread> threads;

		while (end_index < settings_.specimens_count)
		{
			end_index += elements;
			if (rest != 0)
			{
				end_index++;
				rest--;
			}

			threads.push_back(std::thread(rate, start_index, end_index));

			start_index += elements;
		}

		size_t threads_count = threads.size();
		for (size_t i = 0; i < threads_count; ++i)
		{
			threads[i].join();
		}

		std::sort(rating_, rating_ + settings_.specimens_count, [](const Rating& a, const Rating& b) -> bool { return a.rate < b.rate; });

		for (size_t i = 0; i < settings_.bests_count; ++i)
		{
			current_bests_[i]->copy_pixels_from(*specimens_[rating_[i].index]);
		}
	}
}