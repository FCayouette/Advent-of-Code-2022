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

	int part1 = 0, part2 = 0;
	std::string line;
	std::vector<std::string> grid;
	while (in >> line)
		grid.emplace_back(std::move(line));

	for (int y = 0; y < grid.size(); ++y)
		for (int x = 0; x < grid[y].size(); ++x)
		{
			if (x == 0 || y == 0 || x == grid[y].size() - 1 || y == grid.size() - 1)
				++part1;
			else
			{
				int left = 1, right = 1, up = 1, down = 1;

				while (grid[y][x - left] < grid[y][x])
				{
					if (x - left == 0)
						break;
					++left;
				}
				while (grid[y][x + right] < grid[y][x])
				{
					if (x + right == grid[y].size() - 1)
						break;
					++right;
				}
				while (grid[y - up][x] < grid[y][x])
				{
					if (y - up == 0)
						break;
					++up;
				}
				while (grid[y + down][x] < grid[y][x])
				{
					if (y + down == grid.size() - 1)
						break;
					++down;
				}

				int score = left * right * up * down;
				if (score > part2)
					part2 = score;

				char v = grid[y][x];
				if (std::all_of(grid[y].cbegin(), grid[y].cbegin() + x, [v](char c) {return c < v; }) ||
					std::all_of(grid[y].cbegin() + x + 1, grid[y].cend(), [v](char c) {return c < v; }))
				{
					++part1;
					continue;
				}
				bool tall = true;
				for (int i = 0; i < y && tall; ++i)
					tall = grid[i][x] < v;
				if (tall)
					++part1;
				else
				{
					tall = true;
					for (int i = y + 1; i < grid.size() && tall; ++i)
						tall = grid[i][x] < v;
					if (tall)
						++part1;
				}
			}
		}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}