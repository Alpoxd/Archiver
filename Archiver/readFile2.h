std::vector<char> readFile2(std::ifstream& rFile, std::vector<std::pair<char, std::string>>& huffmanCode, int& zeros) {

	size_t size;
	int treeSize = 0;
	rFile >> treeSize;
	rFile.get();

	for (int i = 0; i < treeSize; i++) {
		char charBuffer = ' ';
		std::string codeBuffer;
		rFile.get(charBuffer);
		rFile >> codeBuffer;
		rFile.get();
		huffmanCode.push_back(make_pair(charBuffer, codeBuffer));
	}

	rFile >> zeros;
	rFile.get();

	rFile >> size;
	rFile.get();

	std::vector<char> fileData;
	for (auto i = 0; i < size; i++) {
		char charBuffer = ' ';
		rFile.get(charBuffer);
		fileData.push_back(charBuffer);
	}

	return fileData;
}