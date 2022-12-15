#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <numeric>
#include <format>
#include <future>

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr bool operator ==(const Coord& p) { return x == p.x && y == p.y; }
	T x, y;
};

template<typename T>
T ManhattanDistance(const Coord<T>& c1, const Coord<T>& c2)
{
	return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
}

using Point = Coord<int>;
using u64 = unsigned long long;
constexpr int WORKERS = 10, GRID_SIZE = 4000000;

u64 Search(const std::vector<Point>& sensors, const std::vector<int>& ranges, int d)
{
	u64 result = 0ull;
	for (int y = d; y < GRID_SIZE && !result; y += WORKERS)
		for (int x = 0; x < GRID_SIZE && !result; ++x)
		{
			bool found = true;
			Point p{ x, y };
			for (int i = 0; i < sensors.size(); ++i)
				if (ManhattanDistance(sensors[i], p) <= ranges[i])
				{
					x = sensors[i].x + ranges[i] - std::abs(y - sensors[i].y);
					found = false;
					break;
				}
			if (found)
				result = (u64)p.x * GRID_SIZE + p.y;
		}
	return result;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
		return -1;
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
		return -1;
	
	u64 part1 = 0, part2 = 0;

	std::string line;
	std::vector<int> beaconsOnTarget, ranges;
	std::vector<Point> lineRanges, sensors;	

	while (std::getline(in, line))
	{
		Point b, s;
		sscanf_s(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &s.x, &s.y, &b.x, &b.y);
		int dist = ManhattanDistance(b, s);
		sensors.push_back(s);
		ranges.push_back(dist);
		
		if (b.y == GRID_SIZE/2)
			beaconsOnTarget.push_back(b.x);

		if (int dx = dist - std::abs(s.y - GRID_SIZE/2);
			dx >= 0)
			lineRanges.emplace_back(s.x - dx, s.x + dx);
	}
	
	int previousSize;
	do 
	{
		previousSize = lineRanges.size();
		std::sort(lineRanges.begin(), lineRanges.end());

		for (int i = 0; i < lineRanges.size()-1; ++i)
			for (int j = i + 1; j < lineRanges.size(); ++j)
				if (lineRanges[i].y >= lineRanges[j].x)
				{
					lineRanges[i].x = lineRanges[j].x = std::min(lineRanges[i].x, lineRanges[j].x);
					lineRanges[i].y = lineRanges[j].y = std::max(lineRanges[i].y, lineRanges[j].y);
				}
		lineRanges.erase(std::unique(lineRanges.begin(), lineRanges.end()), lineRanges.end());
	} while (lineRanges.size() != previousSize && lineRanges.size() > 1);
	
	part1 = std::accumulate(lineRanges.cbegin(), lineRanges.cend(), part1, [](u64 t, const Point& p) { return t + p.y - p.x + 1; });
	std::sort(beaconsOnTarget.begin(), beaconsOnTarget.end());
	beaconsOnTarget.erase(std::unique(beaconsOnTarget.begin(), beaconsOnTarget.end()), beaconsOnTarget.end());
	part1 -= beaconsOnTarget.size();

	std::array<std::future<u64>, WORKERS> workers;
	for (int i = 0; i < WORKERS; ++i)
		workers[i] = std::async(Search, sensors, ranges, i);

	std::for_each(workers.begin(), workers.end(), [&part2](std::future<u64>& w) { part2 = std::max(w.get(), part2); });

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}