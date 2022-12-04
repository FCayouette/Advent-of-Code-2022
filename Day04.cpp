import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day04.exe inputFilename\n";
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
		int x, X, y, Y;
		sscanf_s(line.c_str(), "%d-%d,%d-%d", &x, &X, &y, &Y);

		if ((x <= y && X >= Y) || (y <= x && Y >= X)) ++part1;
		if (!(Y < x || X < y)) ++part2;
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}
