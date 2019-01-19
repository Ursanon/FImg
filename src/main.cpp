#include <string>

#include "GeneticDrawer.hpp"

int main(int argc, char** args)
{
	const int width = 256;
	const int height = 256;
    const char * filePath = "./input/Raw-Lenna-256.raw";
    const char * outputDirectory = "./output/";

	bk::GreyscaleRawImage rawImage(filePath, width, height);
	std::string outputPath = std::string(outputDirectory).append("test.raw");
	rawImage.SaveToFile(outputPath.c_str());

    return 0;
}