void readFile1(const std::string& fileName, std::string& buffer) {
	std::ifstream file(fileName, std::ios::binary);

	if (!file) {
		std::cerr << "Error opening file: " << fileName << std::endl;
		std::exit(0);
	}

	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	buffer.resize(fileSize);

	file.read((char*)buffer.data(), fileSize);

	if (!file) {
		std::cerr << "Error reading file: " << fileName << std::endl;
		std::exit(1);
	}
	file.close();
}