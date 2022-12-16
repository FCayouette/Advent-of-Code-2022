import std.core;

using u64 = unsigned long long;
#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

int DistBetween(const std::string& to, std::vector<std::string>& path, const std::map<std::string, std::vector<std::string>>& connections)
{
	if (path.back() == to)
		return path.size() - 1;

	auto iter = connections.find(path.back());
	int best = 1000000;
	for (const std::string& s : iter->second)
	{
		if (std::find(ALLc(path), s) == path.cend())
		{
			path.push_back(s);
			int t = DistBetween(to, path, connections);
			best = std::min(t, best);
			path.pop_back();
		}
	}
	return best;	
}

u64 Check1(const std::map<std::string, int>& flowRates, std::map<std::string, bool>& states, const std::vector<std::string>& valves,
	const std::string& at, const std::map<std::string, int>& dists, int currentFlowRate, int timeLeft, int toOpen)
{
	if (timeLeft < 1)
		return 0;

	u64 best = 0;
	if (at == "AA")
	{
		for (const std::string& to : valves)
		{
			u64 t = Check1(flowRates, states, valves, to, dists, 0, timeLeft - dists.find(at + to)->second, toOpen);
			best = std::max(t, best);
		}
		return best;
	}
	if (!toOpen)
		return currentFlowRate * timeLeft;
	
	
	if (!states[at])
	{
		states[at] = true;
		best += Check1(flowRates, states, valves, at, dists, currentFlowRate + flowRates.find(at)->second, timeLeft - 1, toOpen - 1) + currentFlowRate;
		states[at] = false;
		return best;
	}
	for (const std::string& v : valves)
		if (!states[v])
		{
			if (int d = dists.find(at + v)->second;
				d >= timeLeft)
				best = std::max<u64>(best, currentFlowRate * timeLeft);
			else
			{
				u64 t = d * currentFlowRate + Check1(flowRates, states, valves, v, dists, currentFlowRate, timeLeft - d, toOpen);
				best = std::max(best, t);				
			}
		}

	return best;
}

u64 Check2(const std::map<std::string, int>& flowRates, std::map<std::string, bool>& states, const std::vector<std::string>& valves, const std::map<std::string, int>& dists,
	const std::string& meTo, const std::string& eleTo, int currentFlowRate, int timeLeft, int toOpen, int meLeft, int eleLeft)
{
	if (!toOpen)
		return currentFlowRate * timeLeft;
	if (int minTime = std::min(meLeft, eleLeft);
		minTime > 0)
	{
		if (minTime >= timeLeft)
			return currentFlowRate * timeLeft;
		return currentFlowRate*minTime + Check2(flowRates, states, valves, dists, meTo, eleTo, currentFlowRate, timeLeft - minTime, toOpen, meLeft - minTime, eleLeft - minTime);
	}
	if (meTo == "AA")
	{
		// Select for both
		u64 best = 0;
		for (const std::string& m : valves)
		{
			auto iter1 = dists.find(meTo + m);
			for (const std::string& e : valves)
				if (m != e)
				{
					auto iter2 = dists.find(eleTo + e);
					int minDist = std::min(iter1->second, iter2->second);
					u64 t = Check2(flowRates, states, valves, dists, m, e, 0, timeLeft - minDist, toOpen, iter1->second - minDist, iter2->second - minDist);
					best = std::max(best, t);
				}
		}
		return best;
	}
	if (!meLeft && !eleLeft)
	{
		if (meTo == eleTo)
			std::cout << "!";
		states[meTo] = true;
		states[eleTo] = true;

		auto f1 = flowRates.find(meTo);
		auto f2 = flowRates.find(eleTo);
		currentFlowRate += f1->second + f2->second;

		int best = 0;
		toOpen -= 2;
		if (!toOpen)
			best = currentFlowRate * timeLeft;
		else if (toOpen == 1)
		{
			std::string s;
			for (const auto& i : valves)
				if (!states[i])
				{
					s = i;
					break;
				}
			auto dm = dists.find(meTo + s), em = dists.find(eleTo + s);
			int dd = dm->second, ed = em->second;
			if (dd <= ed)
				best = Check2(flowRates, states, valves, dists, s, s, currentFlowRate, timeLeft, toOpen, dd, 1000);
			else
				best = Check2(flowRates, states, valves, dists, s, s, currentFlowRate, timeLeft, toOpen, 1000, ed);
		}
		else
		{
			for (const std::string& m : valves)
				if (!states[m])
				{
					int dd = dists.find(meTo + m)->second;
					for (const std::string& e : valves)
						if (!states[e] && e != m)
						{
							int ed = dists.find(eleTo + e)->second;
							int t = Check2(flowRates, states, valves, dists, m, e, currentFlowRate, timeLeft, toOpen, dd, ed);
							best = std::max(best, t);
						}
				}
		}

		states[meTo] = false;
		states[eleTo] = false;
		return best;
	}
	--toOpen;
	if (!meLeft)
	{
		states[meTo] = true;
		currentFlowRate += flowRates.find(meTo)->second;
		int best = 0;

		if (!toOpen)
			best = timeLeft * currentFlowRate;
		else if (toOpen == 1)
		{
			int dd = dists.find(meTo + eleTo)->second;
			if (dd != eleLeft)
				best = Check2(flowRates, states, valves, dists, eleTo, eleTo, currentFlowRate, timeLeft, toOpen, dd, eleLeft);
			else
				best = Check2(flowRates, states, valves, dists, eleTo, eleTo, currentFlowRate, timeLeft, toOpen, 100, eleLeft);
		}
		else
		{
			for (const std::string& s : valves)
				if (!states.find(s)->second && s != eleTo)
				{
					int dd = dists.find(meTo + s)->second;
					int t = Check2(flowRates, states, valves, dists, s, eleTo, currentFlowRate, timeLeft, toOpen, dd, eleLeft);
					best = std::max(best, t);
				}
		}
		states[meTo] = false;
		return best;
	}
	else
	{
		int best = 0;
		states[eleTo] = true;
		currentFlowRate += flowRates.find(eleTo)->second;
		if (!toOpen)
			best = timeLeft * currentFlowRate;
		else if (toOpen == 1)
		{
			int ed = dists.find(eleTo + meTo)->second;
			if (ed != meLeft)
				best = Check2(flowRates, states, valves, dists, meTo, meTo, currentFlowRate, timeLeft, toOpen, meLeft, ed);
			else
				best = Check2(flowRates, states, valves, dists, meTo, meTo, currentFlowRate, timeLeft, toOpen, meLeft, 100);
		}
		else
		{
			for (const std::string& s : valves)
				if (!states.find(s)->second && s != meTo)
				{
					int ed = dists.find(eleTo + s)->second;
					int t = Check2(flowRates, states, valves, dists, meTo, s, currentFlowRate, timeLeft, toOpen, meLeft, ed);
					best = std::max(best, t);
				}

		}
		states[eleTo] = false;
		return best;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day16.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1], std::ios::in);
	if (!in)
	{
		std::cout << "Could not open inputFilename " << argv[1] << '\n';
		return -1;
	}

	std::string line;
	u64 part1 = 0, part2 = 0;

	std::map<std::string, int> flowRates;
	std::map<std::string, bool> state;
	std::map<std::string, std::vector<std::string>> connections;
	std::vector<std::string> valves;
	int toOpen = 0;

	while (std::getline(in, line))
	{
		auto p = line.find(';');
		std::string a = line.substr(0, p);
		line = line.substr(line[p+24] == ' ' ? p + 25 : p + 24);
		
		std::string start = a.substr(6, 2);
		int flow = stoi(a.substr(23));

		flowRates[start] = flow;
		if (flow > 0)
		{
			++toOpen;
			valves.push_back(start);
		}
			state[start] = false;
		p = line.find(',');
		std::vector<std::string> goingTo;
		while (p != std::string::npos)
		{
			std::string c = line.substr(0, p);
			goingTo.emplace_back(std::move(c));
			line = line.substr(p + 2);
			p = line.find(',');
		}
		goingTo.emplace_back(line);
		connections[start] = goingTo;		
	}

	// Create a distance map between valves
	std::map<std::string, int> distBetween;
	
	std::vector<std::string> path;
	path.push_back("AA");
	for (const std::string& s : valves)
		distBetween["AA"+s] = DistBetween(s, path, connections);
	path.clear();
	for (int i = 0; i < valves.size(); ++i)
	{
		path.push_back(valves[i]);
		for (int j = 0; j < valves.size(); ++j)
			if (i != j)
			{
				distBetween[valves[i] + valves[j]] = DistBetween(valves[j], path, connections);
			}
		path.pop_back();
	}
	

	part1 = Check1(flowRates, state, valves, "AA", distBetween, 0, 30, valves.size());
	for (auto& p : distBetween)
		++(p.second);

	part2 = Check2(flowRates, state, valves, distBetween, "AA", "AA", 0, 26, valves.size(), 0, 0);
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}