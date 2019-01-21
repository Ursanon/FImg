#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <thread>

#include "GeneticDrawer.hpp"

namespace bk
{
	GeneticDrawer::GeneticDrawer(const GreyscaleRawImage& target, GeneticDrawerSettings settings, const char* output_dir)
		: target_(target), output_dir_(output_dir), settings_(settings)
	{
		current_bests_ = std::vector<GreyscaleRawImage*>();
		for (int i = 0; i < settings.bests_count; ++i)
		{
			current_bests_.push_back(new GreyscaleRawImage(target.get_width(), target.get_height()));
			bestRating_[i].index = i;
			bestRating_[i].rate = 255;
		}

		speciments_ = std::vector<GreyscaleRawImage*>();
		for (int i = 0; i < settings.speciments_count; ++i)
		{
			speciments_.push_back(new GreyscaleRawImage(target.get_width(), target.get_height()));
		}

	}

	GeneticDrawer::~GeneticDrawer()
	{
		for (int i = 0; i < settings_.bests_count; ++i)
		{
			delete current_bests_[i];
			current_bests_[i] = nullptr;
		}

		for (int i = 0; i < settings_.speciments_count; ++i)
		{
			delete speciments_[i];
			speciments_[i] = nullptr;
		}
	}

	void GeneticDrawer::start()
	{
		bool in_progress = true;
		uint64_t generation_number = 0;
#ifdef BENCHMARK_TIME

		std::chrono::time_point<std::chrono::system_clock> time_start, time_end;
		std::chrono::duration<float> elapsed_time;
#endif

		while (in_progress)
		{
#ifdef BENCHMARK_TIME
			time_start = std::chrono::system_clock::now();
			{
#endif

#ifdef BENCHMARK_TIME
			}

			time_end = std::chrono::system_clock::now();
			auto dur = time_end - time_start;
			auto secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
			std::cout << "\nmutation time: " << secs.count() << " [s]";


			time_start = std::chrono::system_clock::now();
			{
#endif
#ifdef BENCHMARK_TIME

			}
			time_end = std::chrono::system_clock::now();
			dur = time_end - time_start;
			secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
			std::cout << "\nEvaluate time: " << secs.count() << " [s]";


			time_start = std::chrono::system_clock::now();
			{
#endif

			cross_over();
#ifdef BENCHMARK_TIME

			}
			time_end = std::chrono::system_clock::now();
			dur = time_end - time_start;
			secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
			std::cout << "\ncross time: " << secs.count() << " [s]";
#endif
			std::vector<std::thread> threads;
			for (int i = 0; i < settings_.speciments_count; ++i)
			{
				int id = i;
				threads.push_back(std::thread(&GeneticDrawer::mutate, this, id));
					//mutate(i);
			}

			for (auto&& t : threads)
			{
				t.join();
			}

			evaluate();

			if (generation_number % 10000 == 0)
			{
				printf("\nsaving : %u generation...", generation_number);

				std::string output_path = output_dir_;
				output_path.append(std::to_string(generation_number));
				output_path.append(".raw");

				current_bests_[0]->save_to_file(output_path.c_str());
			}

			++generation_number;
		}
	}

	void GeneticDrawer::mutate(int id)
	{
		std::random_device rand;
		uint8_t new_color = rand() % 255;

		size_t maxX = target_.get_width() - 1;
		size_t maxY = target_.get_height() - 1;

		size_t start_x = rand() % maxX;
		size_t x_len = rand() % (maxX - start_x) / 3;

		size_t start_y = rand() % maxY;
		size_t y_len = rand() % (maxY - start_y) / 3;

		size_t speciments_size = speciments_.size();

		size_t parent = 0;
		for (size_t j = 0; j < y_len; ++j)
		{
			for (size_t k = 0; k < x_len; ++k)
			{
				size_t index = (start_y + j)*target_.get_width() + k + start_x;

				uint8_t * image = speciments_[id]->get_image();
				uint8_t current_color = *(current_bests_[parent]->get_image() + index);
				*(image + index) = (current_color + new_color >> 1);
			}
			parent++;
			if(parent >= settings_.bests_count)
			{
				parent = 0;
			}
		}
	}

	void GeneticDrawer::evaluate()
	{
		rate();
	}

	void GeneticDrawer::cross_over()
	{
		size_t size = target_.get_size();
		size_t otherParent = rand() % (settings_.bests_count - 1) + 1;

		for (size_t i = 0; i < target_.get_size(); ++i)
		{
			uint8_t * image = current_bests_[0]->get_image();
			uint8_t current_color = *(current_bests_[otherParent]->get_image() + i);
			*(image + i) = (*(image + i) + current_color >> 1);
		}
	}

	static std::chrono::time_point<std::chrono::system_clock> time_start, time_end;
	
	void GeneticDrawer::rate()
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
				//calculate rating
				double diff = 0.f;

				size_t size = target_.get_size();
				for (size_t j = 0; j < size; ++j)
				{
					double a = target_.get_image()[j];
					double b = speciments_[i]->get_image()[j];
					diff += (a-b) * (a-b);
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

		sort_ranking(rating, settings_.speciments_count);
		
		sort_ranking(bestRating_, settings_.bests_count);

		for (size_t i = 0, j = 0; i < settings_.speciments_count; ++i)
		{
			if (bestRating_[j].rate > rating[i].rate)
			{
				memcpy(current_bests_[bestRating_[j].index]->get_image(), speciments_[rating[i].index]->get_image(), sizeof(uint8_t) * current_bests_[j]->get_size());
				bestRating_[j].rate = rating[i].rate;
			}
			else if (j <= settings_.bests_count)
			{
				j++;
				i--;
			}
			else
			{
				j = 0;
			}
		}

		delete[] rating;
	}

	//insertion sort is good enought for small (< ~9k items) array   
	void GeneticDrawer::sort_ranking(Rating * rating, size_t elements_count)
	{
		for (size_t i = 1; i < elements_count; ++i)
		{
			Rating temp = rating[i];

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