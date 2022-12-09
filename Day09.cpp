import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	constexpr Coord operator - (const Coord& p) const { return Coord(x - p.x, y - p.y); }
	T x, y;
};

using Point = Coord<int>;
constexpr std::array<char, 4> coords = { 'R', 'D', 'L', 'U' };
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day09.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	std::array<Point, 10> longRope;
	std::set<Point> part1, part2;
	
	auto Move = [](const Point& front, Point& back)
	{
		Point diff = front - back;
		if (std::abs(diff.x) < 2 && std::abs(diff.y) < 2)
			diff.x = diff.y = 0;
		else
		{
			diff.x = std::max(-1, std::min(1, diff.x));
			diff.y = std::max(-1, std::min(1, diff.y));
		}
		back += diff;
	};

	char dir;
	int count;
	while (in >> dir >> count)
	{
		Point delta = directions[std::find(ALLc(coords), dir) - coords.cbegin()];
		do 
		{
			longRope[0] += delta;
			for (int i = 1; i < longRope.size(); ++i)
				Move(longRope[i-1], longRope[i]);

			part1.insert(longRope[1]);
			part2.insert(longRope.back());

		} while (--count > 0);

	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1.size(), part2.size());
	return 0;
}
