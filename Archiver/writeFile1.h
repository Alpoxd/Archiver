
void writeFile1(std::unordered_map<char, std::string>& huffmanCode, std::ofstream& wFile, std::string text, unsigned long long& size, std::vector<bool> data, std::vector<char> toWrite) {

	wFile << huffmanCode.size() << std::endl;

	auto tmp = huffmanCode.size();
	while (tmp > 0) {
		tmp /= 10;
		size++;
	}
	size++;

	for (auto& i : huffmanCode) {
		wFile << i.first << i.second << std::endl;
		size += 1 + i.second.size();
	}

	int endZeros = data.size() % 8;
	if (endZeros) {
		wFile << 8 - endZeros << std::endl;
		while (endZeros != 8) {
			data.push_back(0);
			endZeros++;
		}
	}
	else wFile << endZeros << std::endl;
	size += 2;

	wFile << data.size() / 8 << std::endl;

	for (auto i : toWrite) {
		wFile.put(i);
	}
}