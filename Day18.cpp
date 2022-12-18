import std.core;

using u64 = unsigned long long;
#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

template<typename T>
struct Coord3D
{
	constexpr Coord3D(T X = 0, T Y = 0, T Z = 0) : x(X), y(Y), z(Z) {}
	constexpr Coord3D operator + (const Coord3D& p) const { return Coord3D(x + p.x, y + p.y, z + p.z); }
	T x, y, z;
};

using Point3D = Coord3D<int>;
constexpr std::array<Point3D, 6> deltas = { Point3D(-1, 0, 0), Point3D(0, -1, 0), Point3D(0, 0, -1), Point3D(1, 0, 0), Point3D(0, 1, 0) , Point3D(0, 0, 1) };

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day17.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}

	std::string line;
	std::vector<std::string> inputs;
	int part1 = 0, part2 = 0, maxX = 0, maxY = 0, maxZ = 0;
	while (std::getline(in, line))
	{
		int x, y, z;
		sscanf_s(line.c_str(), "%d,%d,%d", &x, &y, &z);
		maxX = std::max(x, maxX);
		maxY = std::max(y, maxY);
		maxZ = std::max(z, maxZ);

		inputs.emplace_back(std::move(line));
	}

	std::vector<char> tmpA(maxZ+2, 0);
	std::vector<std::vector<char>> tmpB(maxY+2, tmpA);
	std::vector<std::vector<std::vector<char>>> field(maxX+2, tmpB);

	for (const std::string& s : inputs)
	{
		int x, y, z;
		sscanf_s(s.c_str(), "%d,%d,%d", &x, &y, &z);
		field[x][y][z] = 1;
	}

	for (int x = 0; x <= maxX; ++x)
		for (int y = 0; y <= maxY; ++y)
			for (int z = 0; z <= maxZ; ++z)
				if (field[x][y][z])
				{
					if (x == 0 || !field[x - 1][y][z]) ++part1;
					if (y == 0 || !field[x][y - 1][z]) ++part1;
					if (z == 0 || !field[x][y][z - 1]) ++part1;
					if (x == maxX || !field[x + 1][y][z]) ++part1;
					if (y == maxY || !field[x][y + 1][z]) ++part1;
					if (z == maxZ || !field[x][y][z + 1]) ++part1;
				}

	std::vector<Point3D> curr, next;
	field[0][0][0] = 2;
	curr.push_back(Point3D(0, 0, 0));

	auto Get = [&field](const Point3D& p) { return field[p.x][p.y][p.z]; };
	auto Set = [&field, &next](const Point3D& p) { field[p.x][p.y][p.z] = 2; next.push_back(p); };

	while (!curr.empty())
	{
		for (const Point3D& p : curr)
		{
			if (Point3D n = p + deltas[0]; p.x && !Get(n))
				Set(n);
			if (Point3D n = p + deltas[1]; p.y && !Get(n))
				Set(n);
			if (Point3D n = p + deltas[2]; p.z && !Get(n))
				Set(n);
			if (Point3D n = p + deltas[3]; p.x != maxX && !Get(n))
				Set(n);
			if (Point3D n = p + deltas[4]; p.y != maxY && !Get(n))
				Set(n); 
			if (Point3D n = p + deltas[5]; p.z != maxZ && !Get(n))
				Set(n);
		}
		std::swap(next, curr);
		next.clear();
	}

	for (int x = 0; x <= maxX; ++x)
		for (int y = 0; y <= maxY; ++y)
			for (int z = 0; z <= maxZ; ++z)
				if (field[x][y][z]==1)
				{
					if (x == 0 || field[x - 1][y][z] == 2) ++part2;
					if (y == 0 || field[x][y - 1][z] == 2) ++part2;
					if (z == 0 || field[x][y][z - 1] == 2) ++part2;
					if (x == maxX || field[x + 1][y][z] == 2) ++part2;
					if (y == maxY || field[x][y + 1][z] == 2) ++part2;
					if (z == maxZ || field[x][y][z + 1] == 2) ++part2;
				}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}