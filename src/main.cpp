#include <string>

#include "GreyscaleDrawer.hpp"

int main(int argc, char** args)
{
	const int width = 256;
	const int height = 256;
	const char * input_path = "./input/Raw-Lenna-256.raw";
	const char * output_dir = "./output/";

	//const int width = std::atoi(args[1]);
	//const int height = std::atoi(args[2]);
	//const char * input_path = args[3];
	//const char * output_dir = args[3];

	bk::RawImage<bk::GreyscaleColor> image;
	image.load_from_file(input_path, width, height);

	bk::GeneticDrawerSettings settings(350, 2);
	bk::GreyscaleDrawer drawer(image, settings, output_dir);

	drawer.start();

	return 0;
}