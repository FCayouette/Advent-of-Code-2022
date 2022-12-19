import std.core;

struct Recipe
{
	int oreRobotCost, clayRobotCost, obsRobotOreCost, obsRobotClayCost, geodeRobotOreCost, geodeRobotObsCost, maxOre;
};

struct Info
{
	constexpr Info(int o = 0, int c = 0, int ob = 0, int g = 0) : ore(o), clay(c), obsidian(ob), geode(g) {}
	constexpr Info operator+(const Info& r) const { return Info(ore + r.ore, clay + r.clay, obsidian + r.obsidian, geode + r.geode); }
	constexpr const Info& operator +=(const Info& r) { ore += r.ore; clay += r.clay; obsidian += r.obsidian; geode += r.geode; return *this; }
	int ore, clay, obsidian, geode;
};

std::string GetPossibleRobots(const Info& info, const Recipe& r)
{
	std::string robots = "";
	if (info.obsidian)
		robots += 'G';
	if (info.clay && info.obsidian < r.geodeRobotObsCost)
		robots += 'B';
	if (info.clay < r.obsRobotClayCost)
		robots += 'C';
	if (info.ore < r.maxOre)
		robots += 'O';
	return robots;
}

int Find(const Recipe& recipe, Info stocks, Info production, Info robotConstruction, char robotAim, int time)
{
	stocks += production;
	if (!time)
		return stocks.geode;

	production += robotConstruction;
	Info newRobots;
	std::string poss;
	switch (robotAim)
	{
	case 'O':
		if (stocks.ore >= recipe.oreRobotCost)
		{
			newRobots.ore = 1;
			stocks.ore -= recipe.oreRobotCost;
			poss = GetPossibleRobots(production + newRobots, recipe);
		}
		break;

	case 'C':
		if (stocks.ore >= recipe.clayRobotCost)
		{
			newRobots.clay = 1;
			stocks.ore -= recipe.clayRobotCost;
			poss = GetPossibleRobots(production + newRobots, recipe);
		}
		break;
	case 'B':
		if (stocks.ore >= recipe.obsRobotOreCost && stocks.clay >= recipe.obsRobotClayCost)
		{
			newRobots.obsidian = 1;
			stocks.ore -= recipe.obsRobotOreCost;
			stocks.clay -= recipe.obsRobotClayCost;
			poss = GetPossibleRobots(production + newRobots, recipe);
		}
		break;
	case 'G':
		if (stocks.ore >= recipe.geodeRobotOreCost && stocks.obsidian >= recipe.geodeRobotObsCost)
		{
			newRobots.geode = 1;
			stocks.ore -= recipe.geodeRobotOreCost;
			stocks.obsidian -= recipe.geodeRobotObsCost;
			poss = GetPossibleRobots(production + newRobots, recipe);
		}
	}
	if (poss.empty())
		return Find(recipe, stocks, production, newRobots, robotAim, time - 1);
	int best = 0;
	for (char c : poss)
		best = std::max(best, Find(recipe, stocks, production, newRobots, c, time - 1));
	return best;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day18.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}

	int part1 = 0, part2 = 1, count = 0;
	std::string line;
	std::vector<std::string> blueprints;
	while (std::getline(in,line))
	{
		Recipe r;
		int bpNum;
		sscanf_s(line.c_str(), "Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.",
			&bpNum, &r.oreRobotCost, &r.clayRobotCost, &r.obsRobotOreCost, &r.obsRobotClayCost, &r.geodeRobotOreCost, &r.geodeRobotObsCost);

		r.maxOre = std::max(std::max(r.oreRobotCost, r.clayRobotCost), std::max(r.obsRobotOreCost, r.geodeRobotOreCost));

		Info stocks, robotProd, production(1, 0, 0, 0);
		int c = Find(r, stocks, production, robotProd, 'C', 23);
		int o = Find(r, stocks, production, robotProd, 'O', 23);
		part1 += bpNum * std::max(c, o);
		if (count++ < 3)
		{
			c = Find(r, stocks, production, robotProd, 'C', 31);
			o = Find(r, stocks, production, robotProd, 'O', 31);
			part2 *= std::max(c, o);
		}
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}