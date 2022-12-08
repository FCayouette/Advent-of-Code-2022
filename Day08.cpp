import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day08.exe inputFilename\n";
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
	std::vector<std::string> grid;
	while (in >> line)
		grid.emplace_back(std::move(line));

	for (int y = 0, maxY = grid.size(), maxX = grid[0].size(); y < maxY; ++y)
		for (int x = 0; x < maxX; ++x)
		{
			if (x == 0 || y == 0 || x == maxX - 1 || y == maxY - 1)
				++part1;
			else
			{
				char v = grid[y][x];
				int left = 1, right = 1, up = 1, down = 1;
				while (grid[y][x - left] < v && x-left > 0)
					++left;
				while (grid[y][x + right] < v && x + right < maxX -1)
					++right;
				while (grid[y - up][x] < v && y-up > 0)
					++up;
				while (grid[y + down][x] < v && y + down < maxY -1)
					++down;
				
				int score = left * right * up * down;
				if (score > part2)
					part2 = score;

				if (std::all_of(grid[y].cbegin(), grid[y].cbegin() + x, [v](char c) { return c < v; }) ||
					std::all_of(grid[y].cbegin() + x + 1, grid[y].cend(), [v](char c) { return c < v; }))
					++part1;
				else
				{
					bool tall = true;
					for (int i = 0; i < y && tall; ++i)
						tall = grid[i][x] < v;
					if (tall)
						++part1;
					else
					{
						tall = true;
						for (int i = y + 1; i < maxY && tall; ++i)
							tall = grid[i][x] < v;
						if (tall)
							++part1;
					}
				}
			}
		}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}