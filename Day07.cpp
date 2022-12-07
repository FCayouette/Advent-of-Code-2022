import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day07.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	size_t part1 = 0, part2 = 70000000;
	std::string line;
	std::vector<size_t> dirSizes;
	std::stack<int> stack;
	bool skip = false;
	while (skip || std::getline(in, line))
	{
		skip = false;

		if (line[2] == 'l')
		{
			skip = true;
			while (true)
			{
				if (!std::getline(in, line))
				{
					skip = false;
					break;
				}
				if (line[0] == '$')
					break;
				if (line[0] != 'd')
				{
					line.resize(line.find(' ')); 
					dirSizes[stack.top()] += stoi(line);
				}
			}
		}
		else
		{
			if (line[5] == '.')
			{
				size_t size = dirSizes[stack.top()];
				stack.pop();
				dirSizes[stack.top()] += size;
			}
			else
			{
				stack.push(dirSizes.size());
				dirSizes.push_back(0);
			}
		}
	}

	while (stack.size() > 1)
	{
		size_t size = dirSizes[stack.top()];
		stack.pop();
		dirSizes[stack.top()] += size;
	}
	
	size_t toFree = dirSizes[0] - 40000000;
	for (size_t s : dirSizes)
	{
		if (s <= 100000) part1 += s;
		if (s >= toFree && s < part2) part2 = s;
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}