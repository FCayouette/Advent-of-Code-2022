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
constexpr std::array<Point, 5> moves = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1), Point(0, 0) };

template <typename T>
constexpr bool Within(T min, T val, T max) { return min <= val && val <= max; }
template <typename T>
struct Boundaries
{
	constexpr Boundaries(int x, int y, int X, int Y) : minX(x), minY(y), maxX(X), maxY(Y) {}
	constexpr bool Inside(const Coord<T>& p) const { return Within(minX, p.x, maxX) && Within(minY, p.y, maxY); }
	T minX, minY, maxX, maxY;
};


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day24.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}
	
	int part1 = 0, part2 = 0, maxX = 0, maxY;
	std::string line;
	std::vector<Point> blizzards, upBliz, downBliz, leftBliz, rightBliz;
	Point start, target;

	while (in >> line)
	{
		if (!maxX++)
		{
			start = Point(0, (int)line.find('.'));
			maxY = line.size() - 1;
		}
		else
			for (int i = 1, x = maxX - 1; i < line.size()-1; ++i)
				switch (line[i])
				{
				case '>': rightBliz.emplace_back(x, i); break;
				case '<': leftBliz.emplace_back(x, i); break;
				case '^': upBliz.emplace_back(x, i); break;
				case 'v': downBliz.emplace_back(x, i); break;
				case '.': target = Point(x, i);
				}
	}
	Boundaries<int> field(1, 1, --maxX-1, maxY-1);

	auto CrossValley = [&](Point a, Point b)
	{
		int time = 0;
		std::vector<Point> curr{ a }, next;
		while (!std::binary_search(ALLc(curr), b))
		{
			blizzards.clear();
			for (Point& p : upBliz)	{ if (!--p.x) p.x = maxX - 1; blizzards.push_back(p); }
			for (Point& p : leftBliz) {	if (!--p.y) p.y = maxY - 1;	blizzards.push_back(p);	}
			for (Point& p : downBliz) { if (++p.x == maxX) p.x = 1; blizzards.push_back(p); }
			for (Point& p : rightBliz) { if (++p.y == maxY) p.y = 1; blizzards.push_back(p); }
			std::sort(ALL(blizzards));
			blizzards.erase(std::unique(ALL(blizzards)), blizzards.end());

			for (Point p : curr)
				for (const Point& delta : moves)
					if (Point n = p + delta;
						(field.Inside(n) || n == start || n == target) && !std::binary_search(ALLc(blizzards), n))
						next.push_back(n);
			std::sort(ALL(next));
			next.erase(std::unique(ALL(next)), next.end());

			std::swap(curr, next);
			next.clear();
			++time;
		}
		return time;
	};

	part2 = part1 = CrossValley(start, target);
	part2 += CrossValley(target, start);
	part2 += CrossValley(start, target);
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}