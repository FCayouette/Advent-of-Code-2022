import std.core;

template <int I>
constexpr bool HasSame(const std::array<char, I>& data)
{
	for (int i = 0; i < I - 1; ++i)
		for (int j = i + 1; j < I; ++j)
			if (data[i] == data[j])
				return true;
	return false;
}

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

	std::array<char, 4> start;
	std::array<char, 14> message;

	for (; part1 < 3; ++part1)
		message[part1] = start[part1] = line[part1];

	do
	{
		message[part1 % 14] = start[part1 % 4] = line[part1];
		++part1;
	} while (HasSame(start));

	part2 = part1;
	for (; part2 < 13; ++part2)
		message[part2] = line[part2];

	do
	{
		message[part2 % 14] = line[part2];
		++part2;
	} while (HasSame(message));


	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}