#ifndef GENETIC_DRAWER_HPP
#define GENETIC_DRAWER_HPP

#include <string>
#include <vector>
#include <random>

#include "RawImage.hpp"

namespace bk
{
	struct GeneticDrawerSettings
	{
		GeneticDrawerSettings(uint32_t speciments, uint32_t bests)
			: specimens_count(speciments), bests_count(bests)
		{
		}

		uint32_t specimens_count;
		uint32_t bests_count;
	};

	struct Rating
	{
		size_t index;
		double rate;
	};

	template <typename TColor>
	class GeneticDrawer
	{
	public:
		virtual void start();

	protected:
		GeneticDrawer(const RawImage<TColor>& target, const GeneticDrawerSettings settings, const char* output_dir);
		~GeneticDrawer();

		virtual void mutate() = 0;
		virtual void evaluate() = 0;
		virtual void cross_over();

		void sort_ranking(Rating * rating, size_t elements_count);

	protected:
		std::mt19937 generator_ = std::mt19937(std::random_device()());

		GeneticDrawerSettings settings_;
		std::string output_dir_;

		std::vector<RawImage<TColor>*> current_bests_;
		std::vector<RawImage<TColor>*> specimens_;

		const RawImage<TColor>* target_;
	};

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

			//todo: extract to method
			if (generation_number % 100 == 0)
			{
				printf("\nsaving : %llu generation...", generation_number);

				std::string output_path = output_dir_;
				output_path.append("/");
				output_path.append(std::to_string(generation_number));
				output_path.append(".raw");

				current_bests_[0]->save_to_file(output_path.c_str());
			}

			++generation_number;
		}
	}

	template<typename TColor>
	GeneticDrawer<TColor>::GeneticDrawer(const RawImage<TColor>& target, const GeneticDrawerSettings settings, const char * output_dir)
		: target_(&target),
		output_dir_(output_dir),
		settings_(settings)
	{
		//todo: remove vectors?
		current_bests_ = std::vector<RawImage<TColor>*>();
		for (size_t i = 0; i < settings.bests_count; ++i)
		{
			current_bests_.push_back(new RawImage<TColor>(target.get_width(), target.get_height()));
		}

		//todo: remove vectors?
		specimens_ = std::vector<RawImage<TColor>*>();
		for (size_t i = 0; i < settings.specimens_count; ++i)
		{
			specimens_.push_back(new RawImage<TColor>(target.get_width(), target.get_height()));
		}
	}

	template<typename TColor>
	GeneticDrawer<TColor>::~GeneticDrawer()
	{
		for (size_t i = 0; i < settings_.bests_count; ++i)
		{
			delete current_bests_[i];
			current_bests_[i] = nullptr;
		}

		for (size_t i = 0; i < settings_.specimens_count; ++i)
		{
			delete specimens_[i];
			specimens_[i] = nullptr;
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
	inline void GeneticDrawer<TColor>::sort_ranking(Rating * rating, size_t elements_count)
	{
		Rating temp;
		for (size_t i = 1; i < elements_count; ++i)
		{
			temp = rating[i];

			int j = i - 1;
			while (temp.rate < rating[j].rate && j >= 0)
			{
				rating[j + 1] = rating[j];
				j--;
			}
			rating[j + 1] = temp;
		}
	}
}

#endif