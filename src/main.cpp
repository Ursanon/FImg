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

bool argument_exists(char ** begin, char ** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

//unsigned int parse_numberic_argument(char* begin)
//{
//	char** end = &begin;
//	while (*end++);
//
//	return strtoul(begin, end, 10);
//}

Arguments collect_arguments(int argc, char* args[])
{
	if (argc < 7)
	{
		printf("\n!! wrong args !!\n");
		throw;
	}

	Arguments arguments;
	
	arguments.input_format = (argument_exists(args, args + argc, "-g")) 
		? InputFormat::Greyscale 
		: InputFormat::RGB24;

	arguments.input_path = args[1];
	arguments.output_dir = args[2];

	arguments.specimens_count = strtoul(args[3], 0, 10);
	arguments.parents_count = strtoul(args[4], 0, 10);
	arguments.save_interval = strtoul(args[5], 0, 10);
	arguments.width = strtoul(args[6], 0, 10);
	arguments.height = strtoul(args[7], 0, 10);
	arguments.threads = strtoul(args[8], 0, 10);

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

int main(int argc, char* argv[])
{
	Arguments arguments = collect_arguments(argc, argv);
	
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