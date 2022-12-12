import std.core;

using u64 = unsigned long long;

enum class OP
{
	Add,
	Mul,
	Square
};

struct Monkey
{
	std::vector<u64> items;
	OP op;
	u64 opVal, divisor;
	int onTrue, onFalse;

	template <typename T>
	void DoOp(const T& postOp)
	{
		for (u64& item : items)
		{
			switch (op)
			{
			case OP::Add: item += opVal; break;
			case OP::Mul: item *= opVal; break;
			default: item *= item;
			}
			item = postOp(item);
		}
	}
};

template <typename T>
u64 Simulate(std::vector<Monkey>& monkeys, int rounds, const T& postOp)
{
	std::vector<u64> inspections(monkeys.size(), 0);
	for (int r = 0; r < rounds; ++r)
		for (int i = 0; i < monkeys.size(); ++i)
		{
			Monkey& m = monkeys[i];
			inspections[i] += m.items.size();
			m.DoOp(postOp);
			for (u64 item : m.items)
				monkeys[item % m.divisor == 0 ? m.onTrue : m.onFalse].items.push_back(item);
			m.items.clear();
		}
	std::sort(inspections.begin(), inspections.end(), std::greater<>());
	return inspections[0] * inspections[1];
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day11.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	u64 part1 = 0, part2 = 0, modulo = 1;
	std::string line;
	std::vector<Monkey> monkeys, backup;
	std::vector<u64> inspections;

	while (std::getline(in, line))
	{
		std::getline(in, line);
		auto pos = line.find(':');
		line = line.substr(pos + 2);
		Monkey m;
		while ((pos = line.find(',')) != std::string::npos)
		{
			m.items.push_back(stoi(line.substr(0, pos)));
			line = line.substr(pos + 2);
		}
		m.items.push_back(stoi(line));

		std::getline(in, line);
		line = line.substr(line.find('=') + 6);
		if (line[2] == 'o')
			m.op = OP::Square;
		else
		{
			m.op = (line[0] == '+' ? OP::Add : OP::Mul);
			m.opVal = stoi(line.substr(2));
		}
		
		std::getline(in, line);
		modulo *= m.divisor = stoi(line.substr(line.find("by") + 3));
		
		std::getline(in, line);
		m.onTrue = stoi(line.substr(line.find("monkey") + 7));
		
		std::getline(in, line);
		m.onFalse = stoi(line.substr(line.find("monkey") + 7));

		monkeys.push_back(m);
		inspections.push_back(0);

		std::getline(in, line);
	}

	backup = monkeys;
	part1 = Simulate(monkeys, 20, [](u64 i) { return i / 3; });
	part2 = Simulate(backup, 10000, [modulo](u64 i) { return i % modulo; });

	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, part2);
	return 0;
}