#include <string>
#include <type_traits>

#include "GeneticDrawer.hpp"
#include "RawImage.hpp"

enum InputFormat
{
	RGB24 = 0,
	Greyscale = 1,
};

struct Arguments
{
	InputFormat input_format;
	unsigned int specimens_count;
	unsigned int parents_count;
	unsigned int save_interval;
	unsigned int threads;
	unsigned int height;
	unsigned int width;

	std::string input_path;
	std::string output_dir;
};

template <typename T>
bool is_argument_present(const char **begin, const char **end, std::initializer_list<T> args)
{
	for (auto&& arg : args)
	{
		if (std::find(begin, end, arg) != end)
		{
			return true;
		}
	}

	return false;
}

InputFormat parse_format(const char* arg)
{
	if (arg == "-g")
	{
		return InputFormat::Greyscale;
	}
	else if (arg == "-rgb")
	{
		return InputFormat::RGB24;
	}

	throw std::exception("Can't parse input format.");
}

Arguments parse_arguments(const int argc, const char* args[])
{	
	if (argc < 8)
	{
		printf("\n Check your arguments\n");

		throw std::exception("Some of arguments missing.");
	}

	Arguments arguments;

	arguments.input_format = parse_format(args[1]);
	arguments.input_path = args[2];
	arguments.output_dir = args[3];
	arguments.specimens_count = strtoul(args[4], 0, 10);
	arguments.parents_count = strtoul(args[5], 0, 10);
	arguments.save_interval = strtoul(args[6], 0, 10);
	arguments.width = strtoul(args[7], 0, 10);
	arguments.height = strtoul(args[8], 0, 10);
	arguments.threads = strtoul(args[9], 0, 10);

	return arguments;
}

template <typename TColor, typename TColorData, typename TDrawer>
void start_drawing(Arguments args)
{
	static_assert(std::is_base_of<bk::IColor<TColor, TColorData>, TColor>::value, "xD");
	static_assert(std::is_base_of<bk::GeneticDrawer<TColor>, TDrawer>::value, "xD2");

	bk::RawImage<TColor>* image = new bk::RawImage<TColor>(args.width, args.height);
	bool image_loaded = image->load_from_file(args.input_path.c_str(), args.width, args.height);
	if (!image_loaded)
	{
		printf("Can't load image from: %s", args.input_path.c_str());
		return;
	}

	TDrawer::Settings settings(args.specimens_count, args.parents_count, args.save_interval, args.threads);
	TDrawer drawer(*image, settings, args.output_dir.c_str());

	drawer.start();
}

void show_help()
{
	printf("\n==========\nGenetic Drawer\n==========\n");
	printf("input parameters:\n");
	printf("1st param: -g/-rgb");
	printf("2nd param: input file path");
	printf("3rd param: output directory");
	printf("4th param: specimens count [unsigned int] > 0");
	printf("5th param: parents count [unsigned int] > 0");
	printf("6th param: output save interval [unsigned int] > 0");
	printf("7th param: image with [unsigned int] > 0");
	printf("8th param: image height [unsigned int] > 0");
	printf("9th param: threads count [unsigned int] > 0");
}

int main(const int argc, const char* argv[])
{
	if (is_argument_present<std::string>(argv, (argv + argc), { "--help", "-h" }))
	{
		show_help();
		return 0;
	}

	Arguments arguments;
	try
	{
		arguments = parse_arguments(argc, argv);
	}
	catch (const std::exception& exception)
	{
		printf("%s", exception.what());
		return -1;
	}
	
	switch (arguments.input_format)
	{
	case InputFormat::Greyscale:
		{
			start_drawing<bk::GreyscaleColor, bk::GreyscaleColorData, bk::GreyscaleGeneticDrawer>(arguments);
		}
		break;
	case InputFormat::RGB24:
		{
			start_drawing<bk::RGBColor, bk::RGBColorData, bk::RGBGeneticDrawer>(arguments);
		}
		break; 
	}

	return 0;
}