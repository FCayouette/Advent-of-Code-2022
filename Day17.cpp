import std.core;

using u64 = unsigned long long;
#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

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

	std::array<std::vector<std::string>, 5> patterns;
	patterns[0] = { "@@@@" };
	patterns[1] = { "_@_", "@@@", "_@_" };
	patterns[2] = { "@@@", "__@", "__@" };
	patterns[3] = { "@", "@", "@", "@" };
	patterns[4] = { "@@", "@@" };


	u64 part1 = 0, part2 = 0, total, numCycles, cycleSize;
	std::string instructions;
	
	in >> instructions;

	int width = 7, top = 0, inst = 0, iter = 0, patternToDo = 100000000, patternEndTop;;
	int patternStartIter = -1, patternShape, patternInst, patternTop;

	std::vector<std::string> column;
	column.reserve(4000);
	std::vector<std::array<u64, 5>> seenPattern;
	seenPattern.resize(instructions.size());

	for (; iter < 2022; ++iter)
	{
		int index = iter % patterns.size();
		
		const std::vector<std::string>& shape = patterns[index];
		int newSize = top + shape.size() + 3;
		while (column.size() < newSize)
			column.push_back("_______");

		if (seenPattern[inst][index])
		{
			if (patternStartIter < 0)
			{
				patternStartIter = iter;
				patternInst = inst;
				patternShape = index;
				patternTop = top;
			}
		}
		else
		{
			seenPattern[inst][index] = iter;
			patternStartIter = -1;
		}
		
		for (int r = 0; r < shape.size(); ++r)
			for (int x = 0; x < shape[r].size(); ++x)
				column[top + 3 + r][x + 2] = shape[r][x];

		int x = 2, X = 2+shape[0].size(), bottom = top + 3;
		bool falling = true;
		while (falling)
		{
			int deltaX = instructions[inst] == '<' ? -1 : 1;
			if (!((deltaX < 0 && x == 0) || (deltaX > 0 && X == 7)))
			{
				for (int y = 0; y < shape.size() && deltaX; ++y)
					for (int c = 0; c<shape[0].size() && deltaX; ++c)
						if (column[bottom + y][x + c] == '@' && column[bottom + y][x + c + deltaX] == '#')
							deltaX = 0;
				if (deltaX < 0)
				{
					for (int y = 0; y < shape.size(); ++y)
						for (int c = 1; c < 7; ++c)
							if (column[bottom + y][c] == '@')
								std::swap(column[bottom + y][c], column[bottom + y][c - 1]);
				}
				else if (deltaX > 0)
					for (int y = 0; y < shape.size(); ++y)
						for (int c = 6; c >= 0; --c)
							if (column[bottom + y][c] == '@')
								std::swap(column[bottom + y][c], column[bottom + y][c + 1]);
				x += deltaX;
				X += deltaX;
			}

			if (!bottom)
				falling = false;
			else
				for (int y = 0; y < shape.size(); ++y)
					for (int c = 0; c < 7; ++c)
						if (column[y + bottom][c] == '@' && column[y + bottom - 1][c] == '#')
							falling = false;
			
			if (falling)
			{
				for (int y = 0; y < shape.size(); ++y)
					for (int c = 0; c < 7; ++c)
						if (column[y + bottom][c] == '@')
							std::swap(column[y + bottom - 1][c], column[y + bottom][c]);
				--bottom;
			}
			else
			{
				top = std::max<int>(top, bottom + shape.size());
				for (int y = 0; y < shape.size(); ++y)
					for (int c = 0; c < 7; ++c)
						if (column[y + bottom][c] == '@')
							column[y + bottom][c] = '#';
			}
			int modulo = instructions.size();
			inst = (inst + 1) % modulo;
		}

	}
	part1 = top;

	
	while (--patternToDo)
	{
		int index = iter % patterns.size();
		if (seenPattern[inst][index])
		{
			if (patternStartIter < 0)
			{
				patternStartIter = iter;
				patternInst = inst;
				patternShape = index;
				patternTop = top;
			}
			else if (patternInst == inst && patternShape == index)
			{
				total = 1000000000000ull - patternStartIter;
				numCycles = total / (iter - patternStartIter);
				cycleSize = top - patternTop;
				patternToDo = total % (iter - patternStartIter);
				patternEndTop = top;
			}
		}
		else
		{
			seenPattern[inst][index] = iter;
			patternStartIter = -1;
		}

		const std::vector<std::string>& shape = patterns[index];
		int newSize = top + shape.size() + 3;
		while (column.size() < newSize)
			column.push_back("_______");

		for (int r = 0; r < shape.size(); ++r)
			for (int x = 0; x < shape[r].size(); ++x)
				column[top + 3 + r][x + 2] = shape[r][x];

		int x = 2, X = 2 + shape[0].size(), bottom = top + 3;
		bool falling = true;
		while (falling)
		{
			int deltaX = instructions[inst] == '<' ? -1 : 1;
			if (!((deltaX < 0 && x == 0) || (deltaX > 0 && X == 7)))
			{
				for (int y = 0; y < shape.size() && deltaX; ++y)
					for (int c = 0; c < shape[0].size() && deltaX; ++c)
						if (column[bottom + y][x + c] == '@' && column[bottom + y][x + c + deltaX] == '#')
							deltaX = 0;
				if (deltaX < 0)
				{
					for (int y = 0; y < shape.size(); ++y)
						for (int c = 1; c < 7; ++c)
							if (column[bottom + y][c] == '@')
								std::swap(column[bottom + y][c], column[bottom + y][c - 1]);
				}
				else if (deltaX > 0)
					for (int y = 0; y < shape.size(); ++y)
						for (int c = 6; c >= 0; --c)
							if (column[bottom + y][c] == '@')
								std::swap(column[bottom + y][c], column[bottom + y][c + 1]);
				x += deltaX;
				X += deltaX;
			}

			if (!bottom)
				falling = false;
			else
				for (int y = 0; y < shape.size(); ++y)
					for (int c = 0; c < 7; ++c)
						if (column[y + bottom][c] == '@' && column[y + bottom - 1][c] == '#')
							falling = false;
			
			if (falling)
			{
				for (int y = 0; y < shape.size(); ++y)
					for (int c = 0; c < 7; ++c)
						if (column[y + bottom][c] == '@')
							std::swap(column[y + bottom - 1][c], column[y + bottom][c]);
				--bottom;
			}
			else
			{
				top = std::max<int>(top, bottom + shape.size());
				for (int y = 0; y < shape.size(); ++y)
					for (int c = 0; c < 7; ++c)
						if (column[y + bottom][c] == '@')
							column[y + bottom][c] = '#';
			}
			inst = (inst + 1) % instructions.size();
		}
		++iter;
	}
	part2 = numCycles * cycleSize + patternTop + (top - patternEndTop);
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}