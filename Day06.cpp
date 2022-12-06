import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day06.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	int part1 = 0, part2 = 0;
	std::string line;
	in >> line;

	auto Scan = [&line](int& index, int window)
	{
		for (int i = index - 2; i >= index - window; --i)
			for (int j = i + 1; j < index; ++j)
				if (line[i] == line[j])
				{
					index += i - index + window + 1;
					return true;
				}
		return false;
	};

	part1 = 4;
	while (Scan(part1, 4)) {}
	part2 = part1 + 10;
	while (Scan(part2, 14)) {}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}