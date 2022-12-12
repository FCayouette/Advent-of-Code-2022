import std.core;

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	T x, y;
};

using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day12.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	int part1 = 0, part2, maxY = 0;
	std::string line;
	std::vector<std::string> map;
	Point start, end;

	while (in >> line)
	{
		if (auto p = line.find('S');
			p != std::string::npos)
		{
			start = Point(p, maxY);
			line[p] = 'a';
		}
		if (auto p = line.find('E');
			p != std::string::npos)
		{
			end = Point(p, maxY);
			line[p] = 'z';
		}
		map.emplace_back(std::move(line));
		++maxY;
	}
	int maxX = map.front().size();

	std::set<Point> visited, current, next;
	current.insert(start);
	visited = current;
	
	auto Step = [&map, &visited, &next](const Point& d, const Point& s)
	{
		if ((map[d.y][d.x] <= map[s.y][s.x] + (char)1) && visited.insert(d).second)
			next.insert(d);
	};

	while (true)
	{
		++part1;
		for (const Point& p : current)
		{
			if (p.x != maxX -1)
				Step(p + directions[0], p);
			if (p.y != maxY -1)
				Step(p + directions[1], p);
			if (p.x != 0)
				Step(p + directions[2], p);
			if (p.y != 0)
				Step(p + directions[3], p);
		}

		std::swap(current, next);
		next.clear();
		if (visited.find(end) != visited.cend())
			break;
	}

	part2 = part1;
	for (int i = 0; i < map.size(); ++i)
		for (int j = 0; j < map[i].size(); ++j)
			if (map[i][j] == 'a')
			{
				current.clear();
				current.insert(Point(j, i));
				visited = current;
				int potential = 0;

				while (++potential < part2)
				{
					for (const Point& p : current)
					{
						if (p.x != maxX - 1)
							Step(p + directions[0], p);
						if (p.y != maxY - 1)
							Step(p + directions[1], p);
						if (p.x != 0)
							Step(p + directions[2], p);
						if (p.y != 0)
							Step(p + directions[3], p);
					}

					std::swap(current, next);
					next.clear();
					if (visited.find(end) != visited.cend())
					{
						part2 = potential;
						break;
					}
				}
			}
	
	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, part2);
	return 0;
}