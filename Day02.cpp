import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day02.exe inputFilename" << std::endl;
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
	while (std::getline(in, line))
	{
		int a = line[0]-'A', b = line[2]-'X';
		part1 += b + 1 + 3 * ((4 + b - a) % 3);
		part2 += 3 * b + (a + b + 2) % 3 + 1;
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, part2) << std::endl;
	return 0;
}

