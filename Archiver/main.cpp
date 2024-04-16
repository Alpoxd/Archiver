#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_map>
#include <thread>
#include "Node.h"
#include "encode.h"
#include "buildHuffmanTree.h"
#include "writeFile1.h"
#include "readFile1.h"
#include "readFile2.h"
#include "decode.h"

void main(void) {
	std::cout << "Encode to .arv - 1\nDecode from .arv - 2\nChoose mode: ";
	int mode = 0;
	std::cin >> mode;
	std::cout << "Enter file name: ";
	std::string fileName;
	std::cin >> fileName;

	const int num_parts = 16;

	if (mode == 1) {
		std::string fileData;
		readFile1(fileName, fileData);

		if (!fileData.empty()) {
			std::cout << "[+] Successfully read " << fileData.size() << " bytes from the file.\nPlease, wait...\n";
		}

		const unsigned long long partSize = fileData.size() / num_parts;

		std::vector<std::unordered_map<char, std::string>> huffmanCodeVec(num_parts);
		std::vector<std::string> fileSectionDataVec(num_parts);
		std::vector<std::thread> thVec(num_parts);

		for (int i = 0; i < num_parts; i++) {
			thVec[i] = std::thread([&fileData, i, partSize, &fileSectionDataVec, &huffmanCodeVec]() {

				auto beginSection = i * partSize;
				auto endSection = (i == (num_parts - 1)) ? fileData.size() : (i + 1) * partSize;
				std::string fileSectionData;

				for (auto j = beginSection; j < endSection; j++) fileSectionData += fileData[j];
				auto huffmanCode = buildHuffmanTree(fileSectionData);
				fileSectionDataVec[i] = fileSectionData;
				huffmanCodeVec[i] = huffmanCode;
			});
		}

		for (auto& th : thVec) th.join();

		auto flag = false;
		std::string fileExt;
		for (auto i = fileName.size(); i > 0; i--) {
			if (fileName[i] == '.') {
				flag = true;
				break;
			}
			fileExt += fileName[i];
		}
		if (flag) {
			std::reverse(fileExt.begin(), fileExt.end());
			fileName.resize(fileName.size() - fileExt.size());
		}
		else fileExt = "noext";
		
		std::ofstream wFile(fileName + ".arv", std::ios::binary);
		wFile << fileExt << std::endl;

		for (int i = 0; i < num_parts; i++) {
			writeFile1(huffmanCodeVec[i], wFile, fileSectionDataVec[i]);
		}
		std::cout << "[+] File successfully encoded in " << fileName << ".arv\n";
	}

	if (mode == 2) {

		std::ifstream rFile(fileName, std::ios::binary);
		if (!rFile) {
			std::cerr << "[-] Error opening file: " << fileName << std::endl;
			std::exit(0);
		}
		
		std::string fileExt;
		rFile >> fileExt;
		std::string newName;
		if (fileExt != "noext") {
			fileName.resize(fileName.size() - 4);
			newName = fileName + "_d." + fileExt;
		}
		else {
			newName = fileName + "_d";
		}
			
		std::vector<int> zerosVec(num_parts);
		std::vector<std::vector<std::pair<char, std::string>>> huffmanCodeVec(num_parts);
		std::vector<std::vector<char>> fileSectionDataVec(num_parts);

		for (int i = 0; i < num_parts; i++) {
			int zeros = 0;
			std::vector<std::pair<char, std::string>> huffmanCode;
			auto fileSectionData = readFile2(rFile, huffmanCode, zeros);
			huffmanCodeVec[i] = huffmanCode;
			fileSectionDataVec[i] = fileSectionData;
			zerosVec[i] = zeros;
		}
		if (!fileSectionDataVec.empty() and !huffmanCodeVec.empty()) {
			std::cout << "[+] File is successfully opened\nPlease, wait...\n";
		}
		std::vector<std::thread> thVec(num_parts);
		std::vector<std::string> resVec(num_parts);
		for (int i = 0; i < num_parts; i++) {
			thVec[i] = std::thread([&fileSectionDataVec, &huffmanCodeVec, &zerosVec, i, &resVec]() {
				resVec[i] = decode(fileSectionDataVec[i], huffmanCodeVec[i], zerosVec[i]);
			});
		}
		for (auto& th : thVec) th.join();

		std::ofstream wFile(newName, std::ios::binary);
		for (auto& sec : resVec) {
			wFile << sec;
		}
		std::cout << "[+] File successfully decoded in " << newName << "\n";
	}
}