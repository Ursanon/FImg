#include <string>
#include <type_traits>

#include "RawImage.hpp"
#include "Arguments.hpp"
#include "GeneticDrawer.hpp"

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

template <typename TColor, typename TColorData, typename TDrawer>
void start_drawing(bk::Arguments args)
{
	static_assert(std::is_base_of<bk::IColor<TColor, TColorData>, TColor>::value, "TColor must derive from IColor interface");
	static_assert(std::is_base_of<bk::GeneticDrawer<TColor>, TDrawer>::value, "TDrawer must derive from GeneticDrawer");

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
	printf("1st param: -g/-rgb\n");
	printf("2nd param: input file path\n");
	printf("3rd param: output directory\n");
	printf("4th param: specimens count [unsigned int] > 0\n");
	printf("5th param: parents count [unsigned int] > 0\n");
	printf("6th param: output save interval [unsigned int] > 0\n");
	printf("7th param: image with [unsigned int] > 0\n");
	printf("8th param: image height [unsigned int] > 0\n");
	printf("9th param: threads count [unsigned int] > 0\n");
}

int main(const int argc, const char** argv)
{
	if (is_argument_present<std::string>(argv, (argv + argc), { "--help", "-h" }))
	{
		show_help();
		return 0;
	}

	bk::Arguments arguments;
	try
	{
		arguments = bk::Arguments::parse_arguments(argc, argv);
	}
	catch (const std::exception& exception)
	{
		printf("%s", exception.what());
		return -1;
	}
	
	switch (arguments.input_format)
	{
	case bk::Arguments::InputFormat::Greyscale:
			start_drawing<bk::GreyscaleColor, bk::GreyscaleColorData, bk::GreyscaleGeneticDrawer>(arguments);
		break;
	case bk::Arguments::InputFormat::RGB24:
			start_drawing<bk::RGBColor, bk::RGBColorData, bk::RGBGeneticDrawer>(arguments);
		break; 
	}

	return 0;
}