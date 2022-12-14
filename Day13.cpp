import std.core;

constexpr bool isDigit(char c) { return c >= '0' && c <= '9'; }

enum Result
{
	Unknown = 0,
	RightOrder,
	NotRight
};

std::string_view GetList(const std::string_view& s, size_t start)
{
	size_t end = ++start;
	size_t depth = 1;
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

Result Compare(const std::string_view& a, const std::string_view& b)
{	// Empty checks
	if (a.empty())
		return b.empty() ? Unknown : RightOrder;
	else if (b.empty())
		return NotRight;

	size_t x = 0, y = 0;

	while (x < a.size() && y < b.size())
	{
		if (a[x] == ',')
			++x;
		if (b[y] == ',')
			++y;

		if (a[x] == '[')
		{
			std::string_view lList = GetList(a, x);
			if (b[y] == '[')
			{
				std::string_view rList = GetList(b, y);
				if (Result r = Compare(lList, rList))
					return r;
				x += lList.size() + 2;
				y += rList.size() + 2;
			}
			else
			{
				size_t Y = y;
				while (Y < b.size() && isDigit(b[Y])) ++Y;
				if (Result r = Compare(lList, b.substr(y, Y - y)))
					return r;
				x += lList.size() + 2;
				y = Y;
			}
		}
		else
		{
			size_t X = x;
			while (X < a.size() && isDigit(a[X])) ++X;
			std::string_view lList = a.substr(x, X - x);
			
			if (b[y] == '[')
			{
				std::string_view rList = GetList(b, y);
				if (Result r = Compare(lList, rList))
					return r;
				x += lList.size() + 1;
				y += rList.size() + 2;
			}
			else
			{
				size_t Y = y, left, right;
				while (Y < b.size() && isDigit(b[Y])) ++Y;
				auto R = std::from_chars(a.data() + x, a.data() + X, left);
				R = std::from_chars(b.data() + y, b.data() + Y, right);
				if (auto t = left <=> right;
					t < 0) return RightOrder;
				else if (t > 0) return NotRight;
				x = X;
				y = Y;
			}
		}
	}
	if (x >= a.size())
		return y >= b.size() ? Unknown : RightOrder;
	return NotRight;
}

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

	int part1 = 0, part2, count = 1;
	std::string first, second;
	std::vector<std::string> packets;
	while (in >> first >> second)
	{
		if (Compare(first, second) == RightOrder)
			part1 += count;

		packets.emplace_back(first);
		packets.emplace_back(second);
		++count;
	}
	packets.emplace_back("[[2]]");
	packets.emplace_back("[[6]]");

	std::sort(packets.begin(), packets.end(), [](const std::string& a, const std::string& b) { return Compare(a, b) == RightOrder; });

	auto iter = std::find(packets.cbegin(), packets.cend(), "[[2]]");
	part2 = ++iter - packets.cbegin();
	iter = std::find(iter, packets.cend(), "[[6]]");
	part2 *= ++iter - packets.cbegin();
	
	std::cout << std::format("Part 1: {}\nPart 2: {}", part1, part2);
	return 0;
}