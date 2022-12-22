import std.core;

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	T x, y;
};
using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day22.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}

	int maxX = 0, direction = 0, cubeDirection = 0, cubeSize;
	std::string line;
	std::vector<std::string> map;
	Point pos, cubePos;

	auto Move = [&map, &direction, &pos](int n)
	{
		while (n--)
		{
			Point next = pos + directions[direction];
			if (next.x < 0 || (direction == 2 && map[next.y][next.x] == ' '))
			{
				auto p = map[next.y].find_last_not_of(' ');
				next.x = p;
			}
			else if (next.y < 0 || (direction == 3 && map[next.y][next.x] == ' ')) 
			{
				for (int p = map.size() - 1; p >= 0; --p)
					if (map[p][next.x] != ' ')
					{
						next.y = p;
						break;
					}
			}
			else if (next.y == map.size() || (direction == 1 && map[next.y][next.x] == ' '))
			{
				for (int p = 0; p < map.size(); ++p)
					if (map[p][next.x] != ' ')
					{
						next.y = p;
						break;
					}
			}
			else if (next.x == map[next.y].size() || (direction == 0 && map[next.y][next.x] == ' '))
			{
				auto p = map[next.y].find_first_not_of(' ');
				next.x = p;
			}
			if (map[next.y][next.x] == '#')
				break;
			pos = next;
		}
	};
	std::vector<std::pair<Point, Point>> faces;
	std::map<std::string, std::string> mappings;

	auto CubeMove = [&map, &cubeDirection, &cubePos, &cubeSize, &faces, &mappings](int n)
	{
		auto NextPos = [&mappings, &faces, &cubeSize](int toFace, char fromSide, char toSide, int delta)
		{
			bool inverse = fromSide == toSide;
			if ((fromSide == 'R' && toSide == 'T') ||
				(fromSide == 'T' && toSide == 'R') ||
				(fromSide == 'B' && toSide == 'L') ||
				(fromSide == 'L' && toSide == 'B'))
				inverse = true;

			Point pos = faces[toFace].first;
			if (inverse)
				delta = cubeSize - 1 - delta;

			switch (toSide)
			{
			case 'T': pos.x += delta; break;
			case 'L': pos.y += delta; break;
			case 'B': pos.x += delta; pos.y += cubeSize - 1; break;
			default : pos.y += delta; pos.x += cubeSize - 1;
			}
			return pos;
		};

		while (n--)
		{
			int currFace = std::find_if(faces.cbegin(), faces.cend(), [&cubePos](const std::pair<Point, Point>& p)
				{ return (cubePos.x >= p.first.x && cubePos.x <= p.second.x &&
						cubePos.y >= p.first.y && cubePos.y <= p.second.y); }) - faces.cbegin();
			std::string s;
			int dirDiff = 0;
			s += (char)('1' + currFace);

			Point next = cubePos + directions[cubeDirection];
			if (next.x < 0 || (cubeDirection == 2 && map[next.y][next.x] == ' '))
			{
				int delta = cubePos.y - faces[currFace].first.y;
				
				const std::string& to = mappings.find(s + 'L')->second;
				int toFace = to[0] - '1';
				
				next = NextPos(toFace, 'L', to[1], delta);
				dirDiff = to[2]-'0';
			}
			else if (next.y < 0 || (cubeDirection == 3 && map[next.y][next.x] == ' '))
			{
				int delta = cubePos.x - faces[currFace].first.x;

				const std::string& to = mappings.find(s + 'T')->second;
				int toFace = to[0] - '1';

				next = NextPos(toFace, 'T', to[1], delta);
				dirDiff = to[2] - '0';
			}
			else if (next.y == map.size() || (cubeDirection == 1 && map[next.y][next.x] == ' '))
			{
				int delta = cubePos.x - faces[currFace].first.x;

				const std::string& to = mappings.find(s + 'B')->second;
				int toFace = to[0] - '1';

				next = NextPos(toFace, 'B', to[1], delta);
				dirDiff = to[2] - '0';
			}
			else if (next.x == map[next.y].size() || (cubeDirection == 0 && map[next.y][next.x] == ' '))
			{
				int delta = cubePos.y - faces[currFace].first.y;

				const std::string& to = mappings.find(s + 'R')->second;
				int toFace = to[0] - '1';

				next = NextPos(toFace, 'R', to[1], delta);
				dirDiff = to[2] - '0';
			}
			if (map[next.y][next.x] == '#')
				break;
			cubePos = next;
			cubeDirection = (cubeDirection + dirDiff) % 4;
		}
	};

	while (std::getline(in, line))
	{
		if (line.length() > 1 && !(line[0] >= '0' && line[0] <= '9'))
		{
			if (int delta = line.size() - maxX;
				delta > 0)
			{
				for (std::string& s : map)
					for (int i = 0; i < delta; ++i)
						s += ' ';
				maxX = line.size();
			}
			while (line.size() != maxX)
				line += ' ';
			if (map.empty())
			{
				auto p = line.find('.');
				cubePos = pos = Point(p, 0);
			}
			map.emplace_back(std::move(line));
		}
		else if (line.empty())
			continue;
		else
		{
			
			std::string m;
			while (std::getline(in, m))
				mappings[m.substr(0, 2)] = m.substr(2);

			cubeSize = std::max<int>(maxX, map.size()) / 4;
			for (int y = 0; y < map.size(); y += cubeSize)
				for(int x = 0; x < map[y].size(); x += cubeSize)
					if (map[y][x] != ' ')
					{
						Point a(x, y), b(x + cubeSize - 1, y + cubeSize - 1);
						faces.emplace_back(a, b);
					}


			auto p = line.find_first_of("LR");
			while (p != std::string::npos)
			{
				int move = stoi(line.substr(0, p));
				Move(move);
				CubeMove(move);

				direction = (direction + (line[p] == 'L' ? 3 : 1)) % 4;
				cubeDirection = (cubeDirection + (line[p] == 'L' ? 3 : 1)) % 4;
				
				line = line.substr(p + 1);
				p = line.find_first_of("LR");
			}
			int t = stoi(line);
			Move(t);
			CubeMove(t);
			std::cout << std::format("Part 1: {}\nPart 2: {}\n", 
				1000 * (pos.y + 1) + 4 * (pos.x + 1) + direction, 1000 * (cubePos.y + 1) + 4 * (cubePos.x + 1) + cubeDirection);
		}
	}
	
	return 0;
}
