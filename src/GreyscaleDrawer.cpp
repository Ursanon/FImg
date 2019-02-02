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
		std::vector<std::thread> threads;

		int rest = settings_.specimens_count % settings_.thread_count;
		int elements = settings_.specimens_count / settings_.thread_count;

		int start = 0;
		int end = 0;

		auto rate = [this](size_t start__, size_t end__) -> void
			{
				for (size_t i = start__; i < end__; ++i)
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

		while(end < settings_.specimens_count)
		{
			end += elements;
			if (rest != 0)
			{
				end++;
				rest--;
			}

			threads.push_back(std::thread(rate, start, end));

			start += elements;
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
		std::vector<std::thread> threads;

		int rest = settings_.specimens_count % settings_.thread_count;
		int elements = settings_.specimens_count / settings_.thread_count;

		int start = 0;
		int end = 0;

		auto rate = [this](size_t start__, size_t end__) -> void
		{
			for (size_t i = start__; i < end__; ++i)
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

		while (end < settings_.specimens_count)
		{
			end += elements;
			if (rest != 0)
			{
				end++;
				rest--;
			}

			threads.push_back(std::thread(rate, start, end));

			start += elements;
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