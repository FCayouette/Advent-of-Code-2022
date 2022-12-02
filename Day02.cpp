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

	std::string a, b;
	while (in >> a >> b)
	{
		int score = b[0] - 'W';
		int diff = a[0] - (b[0] - ('X' - 'A'));
		if (!diff)
			score += 3;
		else {
			if (a[0] == 'A')
				score += b[0] == 'Y' ? 6 : 0;
			else if (a[0] == 'B')
				score += b[0] == 'Z' ? 6 : 0;
			else score += b[0] == 'X' ? 6 : 0;
		}
		part1 += score;

		score = 3 * (b[0] - 'X');
		if (score == 3)
			score += a[0] - 'A' + 1;
		else if (!score)
		{
			if (a[0] == 'B') score += 1;
			else score += (a[0] == 'C') ? 2 : 3;
		}
		else
		{
			if (a[0] == 'B') score += 3;
			else score += (a[0] == 'A') ? 2 : 1;
		}
		part2 += score;
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, part2) << std::endl;
	return 0;
}

