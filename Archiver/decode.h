std::string decode(std::vector<char> fileSectionData, std::vector<std::pair<char, std::string>>& huffmanCode, int zeros) {
	std::string data;
	for (auto i : fileSectionData) {
		data += std::bitset<8>(i).to_string();
	}
	if (zeros) data.resize(data.size() - zeros);
	std::string encData;
	int beginBin = 0;
	while (beginBin != data.size()) {
		int endBin = beginBin;
		bool flag = true;
		while (flag) {
			std::string buffer;
			int n = beginBin;
			endBin++;
			for (n; n < endBin; n++) {
				buffer += data[n];
			}
			for (auto& it : huffmanCode) {
				if (buffer == it.second) {
					flag = false;
					encData += it.first;
				}
			}
			if (endBin == data.size()) flag = false;
		}
		beginBin = endBin;
	}
	return encData;
}