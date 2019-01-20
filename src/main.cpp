#include <string>

#include "GeneticDrawer.hpp"
#include "GreyscaleRawImage.hpp"

int main(int argc, char** args)
{
	const int width = 256;
	const int height = 256;
    const char * input_path = "./input/Raw-Lenna-256.raw";
    const char * output_dir = "./output/";

	bk::GreyscaleRawImage image(input_path, width, height);
	
	
	std::string outputPath = std::string(output_dir).append("test.raw");
	image.save_to_file(outputPath.c_str());

    return 0;
}