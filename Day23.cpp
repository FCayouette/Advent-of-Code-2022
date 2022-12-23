import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr bool operator==(const Coord& p) const { return x == p.x && y == p.y; }
	T x, y;
};

using Point = Coord<int>;

constexpr std::array<char, 4> coords = { 'N', 'S', 'W', 'E', };
constexpr std::array<Point, 4> directions = { Point(-1, 0), Point(1, 0), Point(0,-1), Point(0, 1)};

constexpr std::array<Point, 8> adjacents = { Point(-1, -1), Point(-1, 0), Point(-1, 1), Point(0,-1), Point(0, 1), Point(1, -1), Point(1, 0), Point(1, 1) };
constexpr std::array<Point, 3> North = { Point(-1, -1), Point(-1, 0), Point(-1, 1) };
constexpr std::array<Point, 3> South = { Point(1, -1), Point(1, 0), Point(1, 1) };
constexpr std::array<Point, 3> East = { Point(-1, -1), Point(0, -1), Point(1, -1) };
constexpr std::array<Point, 3> West = { Point(-1, 1), Point(0,1), Point(1,1) };
constexpr std::array<std::array<Point, 3>, 4> DirChecks = {North, South, East, West};

struct Elf
{
	constexpr Elf() = default;
	constexpr Elf(const Point& p) : pos(p) {}
	Point pos, proposed;
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day23.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}

	int part1 = 0, part2 = 0, count = 0;
	std::string line;

	std::set<Point> proposed, conflicts;
	std::vector<Point> positions;
	std::vector<Elf> elves;

	while (in >> line)
	{
		for (int i = 0; i < line.size(); ++i)
			if (line[i] == '#')
			{
				Point p(count, i);
				elves.emplace_back(p);
				positions.push_back(p);
			}
		++count;
	}

	auto HasNoAdj = [&positions](const Point& p)
	{
		for (const Point& delta : adjacents)
			if (std::binary_search(ALLc(positions), p+delta))
				return false;
		return true;
	};

	int round = 0;
	while (!part2)
	{
		proposed.clear();
		conflicts.clear();

		for (Elf& e : elves)
		{
			if (HasNoAdj(e.pos))
				e.proposed = e.pos;
			else
			{
				bool found = false;
				for (int i = 0; i < 4; ++i)
				{
					int dir = (round + i) % 4;
					if (std::none_of(ALLc(DirChecks[dir]), [pos = e.pos, &positions](const Point& delta) { return std::binary_search(ALLc(positions), pos + delta); }))
					{
						found = true;
						e.proposed = e.pos + directions[dir];
						if (!proposed.insert(e.proposed).second)
						{
							conflicts.insert(e.proposed);
							e.proposed = e.pos;
						}
						break;
					}
				}
				if (!found)
					e.proposed = e.pos;
			}
		}
		bool hasMoved = false;
		for (Elf& e : elves)
			if (e.pos != e.proposed && conflicts.find(e.proposed) == conflicts.cend())
			{
				e.pos = e.proposed;
				hasMoved = true;
			}
		
		positions.clear();
		for (const Elf& e : elves)
			positions.push_back(e.pos);
		std::sort(ALL(positions));

		if (++round == 10)
		{
			int minX = elves.front().pos.x, maxX = minX;
			int minY = elves.front().pos.y, maxY = minY;
			for (const Elf& e : elves)
			{
				minX = std::min(e.pos.x, minX);
				minY = std::min(e.pos.y, minY);
				maxX = std::max(e.pos.x, maxX);
				maxY = std::max(e.pos.y, maxY);
			}
			part1 = (maxX - minX + 1) * (maxY - minY + 1) - elves.size();
		}
		if (!hasMoved)
		{
			part2 = round;
			break;
		}
	}

    std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}