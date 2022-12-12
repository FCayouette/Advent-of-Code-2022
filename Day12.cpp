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

	int part1 = 0, part2 = 0, maxY = 0, dist = 0;
	std::string line;
	std::vector<std::string> map;
	std::vector<std::vector<char>> visitMap;
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
		visitMap.emplace_back(line.size(), 0);
		map.emplace_back(std::move(line));
		++maxY;
	}
	int maxX = map.front().size();

	std::vector<Point> current, next;
	current.push_back(start);
	visitMap[start.y][start.x] = 1;

	auto Step = [&map, &visitMap, &next](const Point& d, const Point& s)
	{
		if ((map[d.y][d.x] <= map[s.y][s.x] + (char)1) && !visitMap[d.y][d.x])
		{
			next.push_back(d);
			visitMap[d.y][d.x] = 1;
		}
	};

	while (!visitMap[end.y][end.x])
	{
		++part1;
		for (const Point& p : current)
		{
			if (p.x != maxX -1)
				Step(p + Point(1, 0), p);
			if (p.y != maxY -1)
				Step(p + Point(0, 1), p);
			if (p.x != 0)
				Step(p + Point(-1, 0), p);
			if (p.y != 0)
				Step(p + Point(0, -1), p);
		}
		std::swap(current, next);
		next.clear();
	}

	current.clear();
	current.push_back(end);

	for (auto& row : visitMap)
		std::fill(row.begin(), row.end(), 0);
	visitMap[end.y][end.x] = 1;
	
	auto StepBack = [&map, &visitMap, &next](const Point& d, const Point& s)
	{
		if ((map[d.y][d.x] + (char)1 >= map[s.y][s.x]) && !visitMap[d.y][d.x])
		{
			next.push_back(d);
			visitMap[d.y][d.x] = 1;
			if (map[d.y][d.x] == 'a')
				return true;
		}
		return false;
	};
	
	while (!part2)
	{
		++dist;
		for (const Point& p : current)
		{
			if (p.x != maxX - 1 && StepBack(p + Point(1, 0), p))
				part2 = dist;
			if (p.y != maxY - 1 && StepBack(p + Point(0, 1), p))
				part2 = dist;
			if (p.x != 0 && StepBack(p + Point(-1, 0), p))
				part2 = dist;
			if (p.y != 0 && StepBack(p + Point(0, -1), p))
				part2 = dist;
		}
		std::swap(current, next);
		next.clear();
	}
	
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}