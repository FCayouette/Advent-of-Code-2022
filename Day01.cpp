import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day01.exe inputFilename" << std::endl;
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	std::string line;
	int part1 = 0, elf = 0;
	std::vector<int> elves;
	while (std::getline(in, line))
	{
		if (line.empty())
		{
			part1 = std::max(part1, elf);
			elves.push_back(elf);
			elf = 0;
		}
		else
			elf += stoi(line);
	}

	std::sort(elves.begin(), elves.end(), std::greater<>());

	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, std::accumulate(elves.cbegin(), elves.cbegin()+3, 0)) << std::endl;
	return 0;
}
