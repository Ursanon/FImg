#ifndef GENETIC_DRAWER_HPP
#define GENETIC_DRAWER_HPP

#include <string>
#include <vector>
#include <random>
#include <thread>

#include "RawImage.hpp"

namespace bk
{
	template <typename TColor>
	class GeneticDrawer
	{
	public:
		struct Settings
		{
			Settings(uint32_t speciments, uint32_t bests, uint32_t dump_interval, uint32_t thread)
				: specimens_count(speciments), bests_count(bests), save_interval(dump_interval), thread_count(thread)
			{
				if (thread_count == 0)
				{
					thread_count = 1;
				}
			}

			uint32_t specimens_count;
			uint32_t bests_count;
			uint32_t save_interval;
			uint32_t thread_count;
		};

	protected:
		struct Rating
		{
			size_t index;
			double rate;
		};

	public:
		virtual void start();

		GeneticDrawer(const RawImage<TColor>& target, const Settings settings, const char* output_dir);
		virtual ~GeneticDrawer();
	protected:

		virtual void evaluate();
		
		virtual void mutate();
		virtual void cross_over();
		virtual void save_best_specimen(const uint64_t& current_generation);

	protected:
		const char * raw_image_extension_ = ".raw";
		std::mt19937 generator_ = std::mt19937(std::random_device()());

		Settings settings_;
		std::string output_dir_;

		std::vector<RawImage<TColor>*> current_bests_;
		std::vector<RawImage<TColor>*> specimens_;
		Rating* rating_;

		const RawImage<TColor>* target_;
	};

	typedef GeneticDrawer<RGBColor> RGBGeneticDrawer;
	typedef GeneticDrawer<GreyscaleColor> GreyscaleGeneticDrawer;

	template<typename TColor>
	void GeneticDrawer<TColor>::start()
	{
		bool in_progress = true;
		uint64_t generation_number = 0;

		while (in_progress)
		{
			cross_over();

			mutate();

			evaluate();

			++generation_number;

			if (generation_number % settings_.save_interval == 0)
			{
				save_best_specimen(generation_number);
			}
		}
	}

	template<typename TColor>
	GeneticDrawer<TColor>::GeneticDrawer(const RawImage<TColor>& target, const Settings settings, const char * output_dir)
		: target_(&target),
		output_dir_(output_dir),
		settings_(settings)
	{
		current_bests_.reserve(settings.bests_count);
		for (size_t i = 0; i < settings.bests_count; ++i)
		{
			current_bests_.push_back(new RawImage<TColor>(target.get_width(), target.get_height()));
		}

		specimens_.reserve(settings.specimens_count);
		for (size_t i = 0; i < settings.specimens_count; ++i)
		{
			specimens_.push_back(new RawImage<TColor>(target.get_width(), target.get_height()));
		}

		rating_ = new Rating[settings_.specimens_count];
	}

	template<typename TColor>
	GeneticDrawer<TColor>::~GeneticDrawer()
	{
		for (size_t i = 0; i < settings_.bests_count; ++i)
		{
			delete current_bests_[i];
		}

		for (size_t i = 0; i < settings_.specimens_count; --i)
		{
			delete specimens_[i];
		}

		delete[] rating_;
		rating_ = nullptr;
	}

	template<typename TColor>
	inline void GeneticDrawer<TColor>::evaluate()
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
					auto t = specimens_[i]->get_pixel(j);
					diff += t.compare(target_->get_pixel(j));
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

	template<typename TColor>
	inline void GeneticDrawer<TColor>::mutate()
	{
		size_t maxX = target_->get_width() - 1;
		size_t maxY = target_->get_height() - 1;

		auto single_mutate = [&](const size_t start, const size_t end) -> void
		{
			for (size_t i = start; i < end; ++i)
			{
				TColor new_color;
				new_color.fill_with_generator(generator_);

				size_t start_x = generator_() % maxX;
				size_t x_len = generator_() % (maxX - start_x) + 1;

				size_t start_y = generator_() % maxY;
				size_t y_len = generator_() % (maxY - start_y) + 1;

				size_t parent_index = generator_() % settings_.bests_count;
				RawImage<TColor>* parentImage = current_bests_[parent_index];

				for (size_t j = 0; j < y_len; ++j)
				{
					for (size_t k = 0; k < x_len; ++k)
					{
						size_t pixel_x = start_y + j;
						size_t pixel_y = start_x + k;

						TColor pixel_color = parentImage->get_pixel(pixel_x, pixel_y);
						specimens_[i]->set_pixel(pixel_x, pixel_y, (pixel_color.peek_combined(new_color)));
					}
				}
			}
		};

		size_t elements = settings_.specimens_count / settings_.thread_count;
		size_t rest = settings_.specimens_count % settings_.thread_count;

		size_t start_index = 0;
		size_t end_index = 0;

		std::vector<std::thread> threads;

		while (end_index < settings_.specimens_count)
		{
			end_index += elements;
			if (rest != 0)
			{
				end_index++;
				rest--;
			}

			threads.push_back(std::thread(single_mutate, start_index, end_index));

			start_index += elements;
		}

		size_t threads_count = threads.size();
		for (size_t i = 0; i < threads_count; ++i)
		{
			threads[i].join();
		}
	}

	template <typename TColor>
	void GeneticDrawer<TColor>::cross_over()
	{
		size_t part_gene_size = generator_() % target_->get_size();
		size_t rest_gene_size = target_->get_size() - part_gene_size;

		for (size_t i = 0; i < settings_.specimens_count; ++i)
		{
			int parent = i % settings_.bests_count;
			specimens_[i]->copy_pixels_from(*current_bests_[parent], 0, part_gene_size);
			specimens_[i]->copy_pixels_from(*current_bests_[parent], part_gene_size, rest_gene_size);
		}
	}

	template<typename TColor>
	void GeneticDrawer<TColor>::save_best_specimen(const uint64_t& current_generation)
	{
		printf("\nsaving : %llu generation...", current_generation);

		std::string output_path = output_dir_;
		output_path.append("/");
		output_path.append(std::to_string(current_generation));
		output_path.append(raw_image_extension_);

		current_bests_[0]->save_to_file(output_path.c_str());
	}
}

#endif