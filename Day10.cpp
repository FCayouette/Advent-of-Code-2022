import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day10.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}
	int part1 = 0, value, X = 1, count = 0;
	std::string part2, inst;
	
	auto Check = [&]()
	{
		if (count % 40 == 20)
			part1 += X * count;

		part2 += (std::abs((count % 40) - X) < 2 ? '#' : ' ');
		if (++count % 40 == 0)
			part2 += '\n';
	};
	
	while (in >> inst)
	{
		Check();
		if (inst[0] != 'n')
		{
			Check();
			in >> value;
			X += value;
		}
	}

	std::cout << std::format("Part 1: {}\nPart 2:\n{}", part1, part2);
	return 0;
}
