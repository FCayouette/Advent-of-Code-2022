import std.core;

using i64 = long long;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day20.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}

	i64 part1 = 0, part2 = 0, num;
	std::map<i64, int> mapping;
	std::vector<std::pair<i64, int>> numbers, originalPositions;
	while (in >> num)
	{
		if (mapping.find(num) == mapping.cend())
		{
			numbers.emplace_back(num, 0);
			mapping[num] = 1;
		}
		else
		{
			numbers.emplace_back(num, mapping[num]);
			mapping[num] = mapping[num] + 1;
		}
	}
	originalPositions = numbers;

	auto Mix = [&originalPositions, &numbers]()
	{
		for (std::pair<i64, int> p : originalPositions)
		{
			auto iter = std::find(numbers.begin(), numbers.end(), p);
			i64 n = p.first;
			if (n > 0)
			{
				n = n % (numbers.size() - 1);
				for (i64 i = 0; i < n; ++i)
				{
					if (iter + 1 == numbers.end())
					{
						std::swap(*iter, *numbers.begin());
						iter = numbers.begin();
					}
					else
					{
						std::swap(*iter, *(iter + 1));
						++iter;
					}
				}
			}
			else if (n < 0)
			{
				n = (-n) % (numbers.size() - 1);
				for (i64 i = 0; i < n; ++i)
				{
					if (iter == numbers.begin())
					{
						std::swap(*iter, *(std::prev(numbers.end())));
						iter = std::prev(numbers.end());
					}
					else
					{
						std::swap(*iter, *(iter - 1));
						--iter;
					}
				}
			}
		}
	};

	Mix();
	auto Coordinate = [&numbers]() {
		i64 delta = std::find(numbers.cbegin(), numbers.cend(), std::make_pair(0ll, 0)) - numbers.cbegin(), result = 0;
		for (int i = 1; i < 4; ++i)
			result += numbers[(delta + i * 1000) % numbers.size()].first;
		return result;
	};
	
	part1 = Coordinate();

	for (auto& p : originalPositions)
		p.first *= 811589153;

	numbers = originalPositions;

	for (int round = 0; round < 10; ++round)
		Mix();
	
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, Coordinate());

	return 0;
}