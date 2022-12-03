import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day03.exe inputFilename" << std::endl;
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	int part1 = 0, part2 = 0;

	std::string input;
	std::vector<std::string> group;
	while (in >> input)
	{
		std::string f = input.substr(0, input.size() / 2), s = input.substr(input.size() / 2), i;
		std::sort(ALL(f));
		std::sort(ALL(s));
		std::set_intersection(ALLc(f), ALLc(s), std::back_insert_iterator(i));
		char c = i[0];
		part1 += c >= 'a' && c <= 'z' ? c - 'a' + 1 : c - 'A' + 27;

		std::sort(ALL(input));
		group.emplace_back(std::move(input));
		if (group.size() == 3)
		{
			std::string a, b;
			std::set_intersection(ALLc(group[0]), ALLc(group[1]), std::back_insert_iterator(a));
			std::set_intersection(ALLc(a), ALLc(group[2]), std::back_insert_iterator(b));
			c = b[0];
			part2 += c >= 'a' && c <= 'z' ? c - 'a' + 1 : c - 'A' + 27;
			group.clear();
		}
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, part2) << std::endl;
	return 0;
}