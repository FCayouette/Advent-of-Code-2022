import std.core;

using i64 = long long;
#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

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
i64 Evaluate(const std::string& who, MonkeyMap& map)
{
	Monkey& m = map.find(who)->second;
	if (!m.op)
		return m.value;
	i64 l = Evaluate(m.left, map), r = Evaluate(m.right, map);
	switch (m.op)
	{
	case Add: return l + r;
	case Sub: return l - r;
	case Mul: return l * r;
	default: return l / r;
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
			m.op = (OP)(std::find(ALLc(Convert), c) - Convert.cbegin());
			m.right = line.substr(p + 3);
		}
		monkeys[name] = m;
	}
	part1 = Evaluate("root", monkeys);

	// Not generic, but does the job for my input
	const Monkey& root = monkeys["root"];
	Monkey& me = monkeys["humn"];
	i64 right = Evaluate(root.right, monkeys);
	i64 low = 0, high = 10000000000000;

	auto Eval = [&monkeys, &me, s = root.left](i64 val)
	{
		me.value = val;
		return Evaluate(s, monkeys);
	};

	i64 lowEval = Eval(low), highEval = Eval(high);

	while (!part2)
	{
		i64 mid = (low + high) / 2, t = Eval(mid);
		if (t == right)
			part2 = mid;
		else if (t < right)
		{
			high = mid;
			highEval = t;
		}
		else
		{
			low = mid;
			lowEval = t;
		}
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);

	return 0;
}