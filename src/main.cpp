#include <string>

#include "GreyscaleDrawer.hpp"

int main(int argc, char** args)
{
	const int specimens_count = 330;
	const int parents_count = 2;
	const int width = 256;
	const int height = 256;
	const char * input_path = "./input/Raw-Lenna-256.raw";
	const char * output_dir = "./output/";

	bk::RawImage<bk::GreyscaleColor>* image = new bk::RawImage<bk::GreyscaleColor>(width, height);
	image->load_from_file(input_path, width, height);

	bk::GeneticDrawerSettings settings(specimens_count, parents_count);
	bk::GreyscaleDrawer drawer(*image, settings, output_dir);

	drawer.start();

	return 0;
}