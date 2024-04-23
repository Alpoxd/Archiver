
void writeFile1(std::unordered_map<char, std::string>& huffmanCode, std::ofstream& wFile, std::string text, std::vector<bool> data, std::vector<char> toWrite) {
	wFile << huffmanCode.size() << std::endl;
	for (auto& i : huffmanCode) {
		wFile << i.first << i.second << std::endl;
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

	wFile << data.size() / 8 << std::endl;

	for (auto i : toWrite) {
		wFile.put(i);
	}
}