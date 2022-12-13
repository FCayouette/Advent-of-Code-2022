import std.core;

constexpr bool isDigit(char c) { return c >= '0' && c <= '9'; }

enum Result
{
	Unknown = 0,
	RightOrder,
	NotRight
};

std::string GetList(const std::string& s, int start)
{
	int end = ++start;
	int depth = 1;
	while (depth)
	{
		if (s[end] == '[')
			++depth;
		else if (s[end] == ']')
			--depth;
		++end;
	}
	return s.substr(start, end - start - 1);
}

Result Compare(const std::string& a, const std::string& b, int x, int y)
{	// Empty checks
	if (a.empty())
		return b.empty() ? Unknown : RightOrder;
	else if (b.empty())
		return NotRight;

	while (x < a.size() && y < b.size())
	{
		if (a[x] == ',')
			++x;
		if (b[y] == ',')
			++y;

		if (a[x] == '[')
		{
			std::string lList = GetList(a, x);
			if (b[y] == '[')
			{
				std::string rList = GetList(b, y);
				Result r = Compare(lList, rList, 0, 0);
				if (r != Unknown)
					return r;
				x += lList.size() + 2;
				y += rList.size() + 2;
			}
			else
			{
				int Y = y;
				while (isDigit(b[Y])) ++Y;
				std::string rList = b.substr(y, Y - y);
				Result r = Compare(lList, rList, 0, 0);
				if (r != Unknown)
					return r;
				x += lList.size() + 2;
				y = Y;
			}
		}
		else
		{
			int X = x;
			while (isDigit(a[X])) ++X;
			std::string lList = a.substr(x, X - x);
			
			if (b[y] == '[')
			{
				std::string rList = GetList(b, y);
				Result r = Compare(lList, rList, 0, 0);
				if (r != Unknown)
					return r;
				x += lList.size() + 1;
				y += rList.size() + 2;
			}
			else
			{
				int Y = y;
				while (isDigit(b[Y])) ++Y;
				int left = stoi(a.substr(x, X - x)), right = stoi(b.substr(y, Y - y));
				if (left < right) return RightOrder;
				else if (left > right) return NotRight;
				x = X;
				y = Y;
			}
		}
	}
	if (x >= a.size())
		return y >= b.size() ? Unknown : RightOrder;
	return NotRight;
}

struct Decoder
{
	Decoder(const std::string& s = "") : str(s) {}
	std::string str;
	bool operator <(const Decoder& d) const
	{
		return Compare(str, d.str, 0, 0) == RightOrder;
	}
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day13.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << std::endl;
		return -1;
	}

	int part1 = 0, part2 = 0, count = 1;
	std::string first, second;
	std::vector<Decoder> packets;
	while (in >> first >> second)
	{
		if (Compare(first, second, 0, 0) == RightOrder)
			part1 += count;

		packets.emplace_back(first);
		packets.emplace_back(second);
		++count;
	}
	packets.emplace_back("[[2]]");
	packets.emplace_back("[[6]]");

	std::sort(packets.begin(), packets.end());

	count = 0;
	while (true)
		if (packets[count++].str == "[[2]]")
		{
			part2 = count;
			break;
		}
	while (true)
		if (packets[++count].str == "[[6]]")
		{
			part2 *= count+1;
			break;
		}

	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, part2);
	return 0;
}