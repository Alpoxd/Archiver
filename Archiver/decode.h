std::string decode(std::vector<char> fileSectionData, std::vector<std::pair<char, std::string>>& huffmanCode, int zeros) {
    std::string data;
    for (auto i : fileSectionData) {
        data += std::bitset<8>(i).to_string();
    }
    if (zeros) data.resize(data.size() - zeros);
    std::string encData;
    std::string buffer;
    for (char c : data) {
        buffer += c;
        for (auto& code : huffmanCode) {
            if (buffer == code.second) {
                encData += code.first;
                buffer = "";
                break;
            }
        }
    }
    return encData;
}