import std.core;

using i64 = long long;

enum OP
{
	None, Add, Sub, Mul, Div
};

struct Monkey
{
	i64 value;
	int left, right;
	OP op = None;
};
using MonkeyMap = std::vector<Monkey>;
i64 Evaluate(int who, const MonkeyMap& map)
{
	const Monkey& m = map[who];
	if (!m.op)
		return m.value;
	i64 l = Evaluate(m.left, map), r = Evaluate(m.right, map);
	switch (m.op)
	{
	case Add: return l + r;
	case Sub: return l - r;
	case Mul: return l * r;
	default:  return l / r;
	}
}

bool HasMe(int who, const MonkeyMap& map)
{
	if (who == 1) return true;
	const Monkey& m = map[who];
	return m.op ? HasMe(m.left, map) || HasMe(m.right, map) : false;
}

i64 Solve(int at, i64 equalTo, const MonkeyMap& map)
{
	if (at == 1) return equalTo;
	const Monkey& m = map[at];
	bool left = HasMe(m.left, map);
	i64 otherSide = Evaluate(left ? m.right : m.left, map);
	int toSolve = left ? m.left : m.right;
	switch (m.op)
	{
	case Add: return Solve(toSolve, equalTo - otherSide, map);
	case Sub: return Solve(toSolve, left ? equalTo + otherSide : otherSide - equalTo, map);
	case Mul: return Solve(toSolve, equalTo / otherSide, map);
	default:  return Solve(toSolve, left ? equalTo * otherSide : otherSide / equalTo, map);
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day21.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}

	i64 part1 = 0, part2 = 0;
	std::string line;
	constexpr std::array<char, 5> Ops = { 0,'+','-','*','/' };
	std::vector<std::string> seenMonkeys = { "root", "humn" };
	MonkeyMap monkeys(2);

	auto GetIndex = [&seenMonkeys, &monkeys](const std::string& s)
	{
		if (auto iter = std::find(seenMonkeys.cbegin(), seenMonkeys.cend(), s);
			iter == seenMonkeys.cend())
		{
			int result = seenMonkeys.size();
			seenMonkeys.push_back(s);
			monkeys.emplace_back();
			return result;
		}
		else return (int)(iter - seenMonkeys.cbegin());
	};

	while (std::getline(in, line))
	{
		auto p = line.find(':');
		int mIndex = GetIndex(line.substr(0, p));
		line = line.substr(p + 2);
		
		if ((line[0] >= '0' && line[0] <= '9') || line[0] == '-')
			monkeys[mIndex].value = stoi(line);
		else
		{
			p = line.find(' ');
			monkeys[mIndex].left = GetIndex(line.substr(0, p));
			monkeys[mIndex].op = (OP)(std::find(Ops.cbegin(), Ops.cend(), line[p + 1]) - Ops.cbegin());
			monkeys[mIndex].right = GetIndex(line.substr(p + 3));
		}
	}
	part1 = Evaluate(0, monkeys);

	monkeys[0].op = Sub;
	part2 = Solve(0, 0, monkeys);

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}