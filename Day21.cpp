import std.core;

using i64 = long long;

enum OP
{
	None, Add, Sub, Mul, Div
};

struct Monkey
{
	i64 value;
	std::string left, right;
	OP op = None;
};
using MonkeyMap = std::map<std::string, Monkey>;
i64 Evaluate(const std::string& who, const MonkeyMap& map)
{
	const Monkey& m = map.find(who)->second;
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

bool HasMe(const std::string& who, const MonkeyMap& map)
{
	if (who == "humn") return true;
	const Monkey& m = map.find(who)->second;
	return m.op ? HasMe(m.left, map) || HasMe(m.right, map) : false;
}

i64 Solve(const std::string& at, i64 equalTo, const MonkeyMap& map)
{
	if (at == "humn") 
		return equalTo;
	const Monkey& m = map.find(at)->second;
	bool left = HasMe(m.left, map);
	i64 otherSide = Evaluate(left ? m.right : m.left, map);
	const std::string& toSolve = left ? m.left : m.right;
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
	constexpr std::array<char, 5> Convert= { ' ','+','-','*','/' };

	MonkeyMap monkeys;
	while (std::getline(in, line))
	{
		auto p = line.find(':');
		std::string name = line.substr(0, p);
		line = line.substr(p + 2);
		
		Monkey m;
		if ((line[0] >= '0' && line[0] <= '9') || line[0] == '-')
			m.value = stoi(line);
		else
		{
			p = line.find(' ');
			m.left = line.substr(0, p);
			char c = line[p + 1];
			m.op = (OP)(std::find(Convert.cbegin(), Convert.cend(), c) - Convert.cbegin());
			m.right = line.substr(p + 3);
		}
		monkeys[name] = m;
	}
	part1 = Evaluate("root", monkeys);

	monkeys["root"].op = Sub;
	part2 = Solve("root", 0, monkeys);

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}