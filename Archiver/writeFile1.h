
void writeFile1(std::unordered_map<char, std::string>& huffmanCode, std::ofstream& wFile, std::string text, unsigned long long& size) {
	std::vector<bool> data;
	for (char ch : text) {
		for (auto i : huffmanCode[ch]) {
			if (i == '1') data.push_back(1);
			else data.push_back(0);
		}
	}

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

	for (size_t i = 0; i < data.size(); i += 8) {
		char sym = 0;
		int k = 128;
		for (int j = 0; j < 8; j++) {
			sym += data[i + j] * k;
			k /= 2;
		}
		wFile << sym;
		size++;
	}
}