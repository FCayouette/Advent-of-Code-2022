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

	std::array<int, 3> best = { 0, 0, 0 };
	auto AddReset = [&best](int& x) 
	{
		if (x > best[2])
		{
			best[2] = x;
			if (best[2] > best[1])
			{
				std::swap(best[2], best[1]);
				if (best[1] > best[0])
					std::swap(best[1], best[0]);
			}
		}
		x = 0;
	};

	std::string line;
	int elf = 0;
	while (std::getline(in, line))
	{
		if (line.empty())
			AddReset(elf);
		else
			elf += stoi(line);
	}
	AddReset(elf);

	std::cout << std::format("Part 1: {}\nPart 2: {}", best[0], best[0] + best[1] + best[2]) << std::endl;
	return 0;
}
