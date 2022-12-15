#include <iostream>
#include <fstream>
#include <format>
#include <vector>
#include <string>
#include <set>
#include <map>

template<typename T>
struct Coord
{
    constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
    constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
    T x, y;
};

template<typename T>
T ManhattanDistance(const Coord<T>& c1, const Coord<T>& c2)
{
    return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
}

using Point = Coord<int>;
using u64 = unsigned long long;

int main(int argc, char* argv[])
{
    if (argc < 2)
        return -1;
    std::ifstream in(argv[1], std::ios::in);
    if (!in)
        return -1;

    u64 part2 = 0;
    
    std::string line;
    std::set<int> impossibles, beaconsOnTarget;
    std::map<Point, int> sensors;

    int targetLine = 2000000;

    while (std::getline(in, line))
    {
        Point b, s;
        sscanf_s(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &s.x, &s.y, &b.x, &b.y);
        int dist = ManhattanDistance(b, s);
        sensors[s] = dist;

        if (int dx = dist - std::abs(s.y - targetLine);
            dx >= 0)
            for (int i = 0; i <= dx; ++i)
            {
                impossibles.insert(s.x + i);
                impossibles.insert(s.x - i);
            }
        if (b.y == targetLine)
            beaconsOnTarget.insert(b.x);
    }
    for (int a : beaconsOnTarget)
        if (auto iter = impossibles.find(a);
            iter != impossibles.cend())
            impossibles.erase(iter);

    int MAX = 4000000;
    for (int y = 0; y < MAX && !part2; ++y)
        for (int x = 0; x < MAX && !part2; ++x)
        {
            bool found = true;
            Point p{ x, y };
            for (const auto& pair : sensors)
                if (int dist = ManhattanDistance(pair.first, p);
                    dist <= pair.second)
                {
                    x = pair.first.x + pair.second - std::abs(y - pair.first.y);
                    found = false;
                    break;
                }
            if (found)
                part2 = (u64)p.x * 4000000ull + p.y;
        }

    std::cout << std::format("Part 1: {}\nPart 2: {}\n", impossibles.size(), part2);
    return 0;
}
