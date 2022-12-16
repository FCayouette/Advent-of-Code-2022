import std.core;

int DistBetween(const std::string& to, std::vector<std::string>& path, const std::map<std::string, std::vector<std::string>>& connections)
{
	if (path.back() == to)
		return path.size();

	auto iter = connections.find(path.back());
	int best = 1000000;
	for (const std::string& s : iter->second)
		if (std::find(path.cbegin(), path.cend(), s) == path.cend())
		{
			path.push_back(s);
			best = std::min(best, DistBetween(to, path, connections));
			path.pop_back();
		}
	return best;
}

int Opt1(const std::vector<int>& flowRates, std::vector<char>& states, const std::vector<std::vector<int>>& dists, int at, int currentFlowRate, int timeLeft)
{
	states[at] = 1;
	currentFlowRate += flowRates[at];
	int best = timeLeft * currentFlowRate;
	for (int i = 0; i < states.size(); ++i)
		if (!states[i])
			if (int d = dists[at][i]; 
				d < timeLeft)
				best = std::max(best, d * currentFlowRate + Opt1(flowRates, states, dists, i, currentFlowRate, timeLeft - d));

	states[at] = 0;
	return best;
}

int Opt2(const std::vector<int>& flowRates, std::vector<char>& states, const std::vector<std::vector<int>>& dists, int meTo, int eleTo, int currentFlowRate, int timeLeft, int toOpen, int meLeft, int eleLeft)
{
	if (int minTime = std::min(meLeft, eleLeft);
		minTime > 0)
	{
		if (minTime >= timeLeft)
			return currentFlowRate * timeLeft;
		return currentFlowRate * minTime + Opt2(flowRates, states, dists, meTo, eleTo, currentFlowRate, timeLeft - minTime, toOpen, meLeft - minTime, eleLeft - minTime);
	}
	int best = 0;
	if (!meLeft && !eleLeft)
	{
		states[meTo] = states[eleTo] = 1;
		currentFlowRate += flowRates[meTo] + flowRates[eleTo];

		toOpen -= 2;
		if (!toOpen)
			best = timeLeft * currentFlowRate;
		else if (toOpen == 1)
		{
			int to = 0;
			while (states[to]) ++to;
			int dd = dists[meTo][to], ed = dists[eleTo][to];
			if (dd <= ed)
				best = Opt2(flowRates, states, dists, to, to, currentFlowRate, timeLeft, toOpen, dd, 100);
			else
				best = Opt2(flowRates, states, dists, to, to, currentFlowRate, timeLeft, toOpen, 100, ed);
		}
		else for (int i = 0; i < states.size(); ++i)
			if (!states[i])
			{
				int dd = dists[meTo][i];
				for (int j = 0; j < states.size(); ++j)
					if (!states[j] && i != j)
					{
						int ed = dists[eleTo][j], t = Opt2(flowRates, states, dists, i, j, currentFlowRate, timeLeft, toOpen, dd, ed);
						best = std::max(t, best);
					}
			}

		states[meTo] = states[eleTo] = 0;
		return best;
	}
	--toOpen;
	if (!meLeft)
	{
		states[meTo] = 1;
		currentFlowRate += flowRates[meTo];
		if (!toOpen)
			best = timeLeft * currentFlowRate;
		else if (toOpen == 1)
		{
			int dd = dists[meTo][eleTo];
			if (!dd == eleLeft)
				best = Opt2(flowRates, states, dists, eleTo, eleTo, currentFlowRate, timeLeft, 1, dd, eleLeft);
			else best = Opt2(flowRates, states, dists, eleTo, eleTo, currentFlowRate, timeLeft, 1, 100, eleLeft);
		}
		else for (int i = 0; i < states.size(); ++i)
			if (!states[i] && i != eleTo)
			{
				int dd = dists[meTo][i], t = Opt2(flowRates, states, dists, i, eleTo, currentFlowRate, timeLeft, toOpen, dd, eleLeft);
				best = std::max(best, t);
			}
		states[meTo] = 0;
		return best;
	}
	states[eleTo] = 1;
	currentFlowRate += flowRates[eleTo];
	if (!toOpen)
		best = timeLeft * currentFlowRate;
	else if (toOpen == 1)
	{
		int ed = dists[eleTo][meTo];
		if (!ed == meLeft)
			best = Opt2(flowRates, states, dists, meTo, meTo, currentFlowRate, timeLeft, 1, meLeft, ed);
		else best = Opt2(flowRates, states, dists, meTo, meTo, currentFlowRate, timeLeft, 1, meLeft, 100);
	}
	else for (int i = 0; i < states.size(); ++i)
		if (!states[i] && i != meTo)
		{
			int ed = dists[eleTo][i], t = Opt2(flowRates, states, dists, meTo, i, currentFlowRate, timeLeft, toOpen, meLeft, ed);
			best = std::max(best, t);
		}

	states[eleTo] = 0;
	return best;
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
	int part1 = 0, part2 = 0;

	std::map<std::string, std::vector<std::string>> connections;
	std::vector<std::string> valves;
	std::vector<int> rates, distancesFromStart;
	std::vector<char> states;
	std::vector<std::vector<int>> distances;

	while (std::getline(in, line))
	{
		auto p = line.find(';');
		std::string a = line.substr(0, p);
		line = line.substr(line[p+24] == ' ' ? p + 25 : p + 24);
		
		std::string start = a.substr(6, 2);
		int flow = stoi(a.substr(23));

		if (flow > 0)
		{
			valves.push_back(start);
			states.push_back(0);
			rates.push_back(flow);
		}
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

	std::vector<std::string> path;
	path.push_back("AA");
	for (const std::string& s : valves)
		distancesFromStart.push_back(DistBetween(s, path, connections));
	path.clear();

	distances.resize(valves.size());
	for (auto& v : distances)
		v.resize(valves.size());

	for (int i = 0; i < valves.size(); ++i)
	{
		path.push_back(valves[i]);
		for (int j = i+1; j < valves.size(); ++j) 
			distances[j][i] = distances[i][j] = DistBetween(valves[j], path, connections);			
		path.clear();
	}
	
	for (int i = 0; i < states.size() - 1; ++i)
	{
		int d = distancesFromStart[i];
		part1 = std::max(part1, Opt1(rates, states, distances, i, 0, 30 - d));
		for (int j = i + 1; j < valves.size(); ++j)
		{
			int e = distancesFromStart[j], minDist = std::min(d, e);
			part2 = std::max(part2, Opt2(rates, states, distances, i, j, 0, 26 - minDist, valves.size(), d - minDist, e - minDist));
		}
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	return 0;
}