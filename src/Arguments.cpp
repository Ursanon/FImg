#include "Arguments.hpp"

namespace bk
{
	Arguments::InputFormat Arguments::parse_format(const char* arg)
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

	Arguments Arguments::parse_arguments(const int argc, const char** args)
	{
		if (argc < 8)
		{
			printf("Check your arguments\n");

			throw std::exception("Some of arguments are missing.");
		}

		Arguments arguments;

		arguments.input_format = parse_format(args[1]);
		arguments.input_path = args[2];
		arguments.output_dir = args[3];
		arguments.specimens_count = std::max<unsigned int>(strtoul(args[4], 0, 10), 1);
		arguments.parents_count = std::max<unsigned int>(strtoul(args[5], 0, 10), 1);
		arguments.save_interval = std::max<unsigned int>(strtoul(args[6], 0, 10), 1);
		arguments.width = std::max<unsigned int>(strtoul(args[7], 0, 10), 1);
		arguments.height = std::max<unsigned int>(strtoul(args[8], 0, 10), 1);
		arguments.threads = std::max<unsigned int>(strtoul(args[9], 0, 10), 1);

		return arguments;
	}
}