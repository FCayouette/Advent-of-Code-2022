import std.core;

using i64 = long long;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day25.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}

	i64 sum = 0;
	std::string line, out = "";
	while (in >> line)
	{
		i64 number = 0;
		for (i64 i = line.size()-1, power = 1; i >= 0; --i, power *= 5)
			switch (line[i])
			{
			case '-': number -= power; break;
			case '=': number -= 2 * power; break;
			default: number += power * (line[i] - '0');
			}
		sum += number;
	}

	while (sum)
	{
		char c = '0' + (sum % 5);
		bool high = c > '2';
		if (high)
			switch (c)
			{
			case '3':c = '='; break;
			case '4':c = '-'; break;
			default: c = '0';
			}
		out += c;
		sum = sum / 5 + high;
	}
	std::reverse(out.begin(), out.end());
	std::cout << std::format("SNAFU: {}\n", out);
	return 0;
}