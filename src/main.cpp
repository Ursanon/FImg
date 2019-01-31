#include <string>

#include "GreyscaleDrawer.hpp"
#include "RawImage.hpp"

int main(int argc, char** args)
{
	const int specimens_count = 330;
	const int parents_count = 2;
	const int save_interval = 100;
	const int width = 256;
	const int height = 256;
	//const char * input_path = "./input/Raw-Lenna-256-greyscale.raw";
	const char * input_path = "./input/Raw-Lenna-256-rgb.raw";
	const char * output_dir = "./output/";

	bk::RawImage<bk::RGBColor>* image = new bk::RawImage<bk::RGBColor>(width, height);
	//bk::GreyscaleRawImage* image = new bk::GreyscaleRawImage(width, height);
	bool image_loaded = image->load_from_file(input_path, width, height);
	if (!image_loaded)
	{
		printf("Can't load image from: %s", input_path);
		return -1;
	}

	std::string output_path = output_dir;
	output_path.append("/");
	output_path.append("input");
	output_path.append(".raw");
	image->save_to_file(output_path.c_str());

	//bk::GreyscaleDrawer::Settings settings(specimens_count, parents_count, save_interval);
	bk::RGBDrawer::Settings settings(specimens_count, parents_count, save_interval);
	//bk::GreyscaleDrawer drawer(*image, settings, output_dir);
	bk::RGBDrawer drawer(*image, settings, output_dir);

	drawer.start();

	return 0;
}