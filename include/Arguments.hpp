#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include <string>
#include <algorithm>

namespace bk
{
	struct Arguments
	{
	public:
		enum InputFormat
		{
			RGB24 = 0,
			Greyscale = 1,
		};

	public:
		InputFormat input_format;
		unsigned int specimens_count;
		unsigned int parents_count;
		unsigned int save_interval;
		unsigned int threads;
		unsigned int height;
		unsigned int width;

		std::string input_path;
		std::string output_dir;

		static Arguments parse_arguments(const int argc, const char** args);
	private:
		static InputFormat parse_format(const char* arg);
	};

} // namespace bk

#endif