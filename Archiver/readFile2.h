std::vector<char> readFile2(std::ifstream& rFile, std::vector<std::pair<char, std::string>>& huffmanCode, int& zeros) {

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

	std::vector<char> fileData;

	while (true) {
		bool flag = true;
		char charBuffer = ' ';
		rFile.get(charBuffer);
		if (charBuffer == '!') {
			char sectionEndCheck[3];
			for (int i = 0; i < 3; i++) {
				rFile.get(sectionEndCheck[i]);
			}
			if (sectionEndCheck[0] == 'e' and sectionEndCheck[1] == 'o' and sectionEndCheck[2] == 's') {
				flag = false;
				break;
			}
			fileData.push_back(charBuffer);
			for (auto i : sectionEndCheck) {
				fileData.push_back(i);
			}
			flag = false;
		}
		if (flag) fileData.push_back(charBuffer);
	}
	rFile.get();
	return fileData;
}