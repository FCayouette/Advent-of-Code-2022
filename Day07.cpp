import std.core;

struct Node
{
	std::vector<std::pair<std::string, int>> files;
	std::vector<std::string> directories;
	int size = 0;
};

using Tree = std::map<std::string, Node>;

int computeSize(const std::string& current, Tree& tree, int& part1)
{
	int total = 0;
	auto iter = tree.find(current);
	for (const auto& p : iter->second.files)
		total += p.second;
	for (const auto& s : iter->second.directories)
	{
		int t = computeSize(s, tree, part1);
		total += t;
	}
	if (total <= 100000)
		part1 += total;
	iter->second.size = total;
	return total;
}

void findClosest(const std::string& current, int target, Tree& tree, int& part2)
{
	auto iter = tree.find(current);

	if (iter->second.size < target) return;
	if (iter->second.size < part2)
		part2 = iter->second.size;

	for (const auto& s : iter->second.directories)
		findClosest(s, target, tree, part2);
	return;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day06.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	int part1 = 0, part2 = 0;
	std::string line;

	Tree tree;
	std::string current = "/";
	bool skip = false;
	std::getline(in, line); // first ignore cd /
	while (skip || std::getline(in, line))
	{
		skip = false;

		if (line[2] == 'l')
		{
			Node n;
			skip = true;
			while (true)
			{
				if (!std::getline(in, line))
				{
					skip = false;
					break;
				}
				if (line[0] == '$')
					break;
				if (line[0] == 'd')
				{
					if (current.back() != '/')
						n.directories.push_back(current + '/' + line.substr(4));
					else n.directories.push_back(current + line.substr(4));
				}

				else
				{
					auto pos = line.find(' ');
					n.files.push_back(std::make_pair(line.substr(pos + 1), stoi(line.substr(0, pos))));
				}
			}
			tree[current] = n;
		}
		else
		{
			line = line.substr(5);
			if (line[0] == '.')
			{
				auto pos = current.find_last_of('/');
				current = current.substr(0, pos);
			}
			else
			{
				if (current.empty())
					current = "/";
				if (current.back() != '/')
					current += '/';
				current += line;
			}
		}
	}

	computeSize("/", tree, part1);

	int totalSize = tree["/"].size;
	int freeSize = 70000000 - totalSize;
	int toFree = 30000000 - freeSize;
	
	part2 = 70000000;
	findClosest("/", toFree, tree, part2);
	

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}