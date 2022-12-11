import std.core;

using u64 = unsigned long long;
#define ALL(x) (x).begin(),(x).end()

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
	void DoOp()
	{
		for (u64& item : items)
			switch (op)
			{
			case OP::Add: item += opVal; break;
			case OP::Mul: item *= opVal; break;
			default: item *= item;
			}
	}
};

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

	u64 part1 = 0, modulo = 1;
	std::string line;
	std::vector<Monkey> monkeys, backup;
	std::vector<u64> inspections;

	int value;
	char c;


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
		line = line.substr(line.find("by") + 3);
		u64 divisor = stoi(line);
		m.divisor = divisor;
		modulo *= divisor;
		
		std::getline(in, line);
		m.onTrue = line.back() - '0';
		
		std::getline(in, line);
		m.onFalse = line.back() - '0';

		monkeys.push_back(m);
		inspections.push_back(0);

		std::getline(in, line);
	}

	backup = monkeys;

	for (int round = 0; round < 20; ++round)
		for (int i = 0; i < monkeys.size(); ++i)
		{
			Monkey& m = monkeys[i];
			inspections[i] += m.items.size();
			m.DoOp();
			for (u64 item : m.items)
			{
				item /= 3;
				monkeys[item % m.divisor == 0 ? m.onTrue : m.onFalse].items.push_back(item);
			}
			m.items.clear();
		}
	
	std::sort(ALL(inspections), std::greater<>());
	part1 = inspections[0] * inspections[1];

	std::fill(ALL(inspections), 0);
	std::swap(monkeys, backup);
	for (int round = 0; round < 10000; ++round)
		for (int i = 0; i < monkeys.size(); ++i)
		{
			Monkey& m = monkeys[i];
			inspections[i] += m.items.size();
			m.DoOp();
			for (u64 item : m.items)
			{
				item %= modulo;
				monkeys[item % m.divisor == 0 ? m.onTrue : m.onFalse].items.push_back(item);
			}
			m.items.clear();
		}
	
	std::sort(ALL(inspections), std::greater<>());

	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, inspections[0] * inspections[1]);
	return 0;
}