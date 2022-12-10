import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day09.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}
	int part1 = 0;
	std::string inst;
	int value, X = 1, count = 0;
	
	auto Strength = [&part1, &X, &count]()
	{
		if (count % 40 == 20)
			part1 += X * count;
	};

	auto CRT = [&X, &count]()
	{
		std::cout << (std::abs((count % 40) - X) < 2 ? '#' : ' ');
		if (++count % 40 == 0)
			std::cout << '\n';
	};
	std::cout << "Part 2:\n";
	while (in >> inst)
	{
		if (inst[0] == 'n')
		{
			CRT();
			Strength();
		}
		else
		{
			in >> value;
			CRT();
			Strength();
			CRT();
			Strength();
			X += value;
		}
	}

	std::cout << std::format("Part 1: {}\n", part1);
	return 0;
}
