
void writeFile1(std::unordered_map<char, std::string>& huffmanCode, std::ofstream& wFile, std::string text) {
	std::vector<bool> data;
	for (char ch : text) {
		for (auto i : huffmanCode[ch]) {
			if (i == '1') data.push_back(1);
			else data.push_back(0);
		}
	}

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

	for (size_t i = 0; i < data.size(); i += 8) {
		char sym = 0;
		int k = 128;
		for (int j = 0; j < 8; j++) {
			sym += data[i + j] * k;
			k /= 2;
		}
		wFile << sym;
	}
	wFile << "!eos\n";
}