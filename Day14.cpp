import std.core;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day14.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}

	int part1 = 0, part2 = 0;
	std::string line;

	std::vector <std::string> grid;
	int maxX = 501;
	while (std::getline(in, line))
	{
		int x=-1, y=-1, X, Y;
		auto p = line.find('-');
		while (p != std::string::npos)
		{
			std::string sub = line.substr(0, p - 1);
			line = line.substr(p + 3);
			sscanf_s(sub.c_str(), "%d,%d", &X, &Y);
			while (grid.size() < Y + 1)
			{
				std::string s;
				s.resize(maxX);
				grid.emplace_back(std::move(s));
			}

			if (maxX < X + 2)
			{
				maxX = X + 2;
				for (auto& s : grid)
					s.resize(maxX);
			}
			if (x >= 0)
			{
				if (x != X)
					for (int i = std::min(x, X); i <= std::max(x, X); ++i)
						grid[y][i] = '#';
				else
					for (int i = std::min(y, Y); i <= std::max(y, Y); ++i)
						grid[i][x] = '#';
			}
			x = X;
			y = Y;

			p = line.find('-');
		}
		sscanf_s(line.c_str(), "%d,%d", &X, &Y);
		while (grid.size() < Y + 1)
		{
			std::string s;
			s.resize(maxX);
			grid.emplace_back(std::move(s));
		}

		if (maxX < X + 2)
		{
			maxX = X + 2;
			for (auto& s : grid)
				s.resize(maxX);
		}
		if (x != X)
		{
			for (int i = std::min(x, X); i <= std::max(x, X); ++i)
				grid[y][i] = '#';
		}
		else
		{
			for (int i = std::min(y, Y); i <= std::max(y, Y); ++i)
				grid[i][x] = '#';
		}		
	}

	for (int i = 0; i < grid.size(); ++i)
		for (int j = 0; j < maxX; ++j)
			if (!grid[i][j])
				grid[i][j] = ' ';

	bool go = true;
	while (go)
	{
		int x, y, X = 500, Y = 0;
		do 
		{
			x = X; y = Y;
			if (y + 1 == grid.size())
			{
				go = false;
				break;
			}
			if (grid[y + 1][x] == ' ')
				Y = y + 1;
			else
			{
				if (grid[y + 1][x - 1] == ' ')
				{
					Y = y + 1;
					X = x - 1;
				}
				else if (grid[y + 1][x + 1] == ' ')
				{
					Y = y + 1;
					X = x + 1;
				}
				else grid[y][x] = 'o';
			}
		} while (y != Y);
		if (go)
			++part1;
	}

	part2 = part1;
	std::string a;
	a.resize(maxX);
	for (int i = 0; i < maxX; ++i)
		a[i] = ' ';
	grid.push_back(a);
	go = true;
	while (go)
	{
		int x, y, X = 500, Y = 0;
		do
		{
			x = X; y = Y;
			if (y + 1 == grid.size())
			{
				grid[y][x] = 'o';
				break;
			}
			if (grid[y + 1][x] == ' ')
				Y = y + 1;
			else
			{
				if (x + 1 == grid[0].size())
					for (auto& s : grid)
						for (int i = 0; i < 20; ++i)
							s += ' ';
				
				if (grid[y + 1][x - 1] == ' ')
				{
					Y = y + 1;
					X = x - 1;
				}
				else if (grid[y + 1][x + 1] == ' ')
				{
					Y = y + 1;
					X = x + 1;
				}
				else
				{
					grid[y][x] = 'o';
					if (y == 0 && x == 500)
						go = false;
				}
			}
		} while (y != Y);
		++part2;
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, part2);
	return 0;
}