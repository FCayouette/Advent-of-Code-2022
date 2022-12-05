import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day05.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	std::string line, part1, part2, move, f, to;

	std::vector<std::vector<char>> stacks, backup;
	std::deque<std::string> configuration;
	while (std::getline(in, line))
	{
		if (line[1] != '1')
			configuration.emplace_front(std::move(line));
		else
		{
			int count = (line.size()+1)/4;
			stacks.resize(count);
			for (const std::string& s : configuration)
				for (int i = 0; i < count; ++i)
				{
					if (i * 4 + 1 >= s.length()) break;
					if (char c = s[i * 4 + 1]; 
						c >= 'A' && c <='Z')
						stacks[i].push_back(c);
				}
			break;
		}
	}
	backup = stacks;
	int count, from, dest;
	while (in >> move >> count >> f >> from >> to >> dest) 
	{
		--from;
		--dest;
		for (int i = 0; i < count; ++i)
		{
			stacks[dest].push_back(stacks[from].back());
			stacks[from].pop_back();
		}

		for (int i = 0, index = backup[from].size() - count; i < count; ++i)
			backup[dest].push_back(backup[from][index + i]);
		backup[from].resize(backup[from].size() - count);
	}
	for (const auto& s : stacks)
		part1 += s.back();
	for (const auto& s : backup)
		part2 += s.back();

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}