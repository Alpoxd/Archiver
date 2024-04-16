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
	const int parts[19] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 100, 200, 300, 400, 500 };
	//const int num_parts = 16;
	for (auto num_parts : parts) {
		if (mode == 1) {
			auto begClock = clock();
			std::string fileData;
			readFile1(fileName, fileData);

			if (!fileData.empty()) {
				//std::cout << "[+] Successfully read " << fileData.size() << " bytes from the file.\nPlease, wait...\n";
			}

			const unsigned long long partSize = fileData.size() / num_parts;

			std::vector<std::unordered_map<char, std::string>> huffmanCodeVec(num_parts);
			std::vector<std::string> fileSectionDataVec(num_parts);
			std::vector<std::thread> thVec(num_parts);

			for (int i = 0; i < num_parts; i++) {
				thVec[i] = std::thread([&fileData, i, partSize, &fileSectionDataVec, &huffmanCodeVec, num_parts]() {

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

			unsigned long long size = 0;

			auto flag = false;
			std::string fileExt;
			for (auto i = fileName.size(); i > 0; i--) {
				if (fileName[i] == '.') {
					flag = true;
					break;
				}
				fileExt += fileName[i];
			}
			std::string newName = fileName;
			if (flag) {
				std::reverse(fileExt.begin(), fileExt.end());
				newName.resize(newName.size() - fileExt.size());
			}
			else fileExt = "noext";

			std::ofstream wFile(newName + ".arv", std::ios::binary);
			wFile << fileExt << std::endl;
			size += fileExt.size() + 1;

			for (int i = 0; i < num_parts; i++) {
				writeFile1(huffmanCodeVec[i], wFile, fileSectionDataVec[i], size);
			}

			wFile.close();
			//std::cout << "[+] File successfully encoded in " << fileName << ".arv\n";
			std::cout << num_parts << " " << clock() - begClock << " " << size << " ";
			begClock = clock();
			std::ifstream rFile(newName + ".arv", std::ios::binary);
			if (!rFile) {
				std::cerr << "[-] Error opening file: " << fileName << std::endl;
				std::exit(0);
			}
			rFile >> fileExt;
			if (fileExt != "noext")
				newName = newName + "_d." + fileExt;

			int zeros = 0;
			std::vector<std::vector<std::pair<char, std::string>>> huffmanCodeVec1(num_parts);
			std::vector<std::vector<char>> fileSectionDataVec1(num_parts);
			
			for (int i = 0; i < num_parts; i++) {
				std::vector<std::pair<char, std::string>> huffmanCode;
				auto fileSectionData = readFile2(rFile, huffmanCode, zeros);
				huffmanCodeVec1[i] = huffmanCode;
				fileSectionDataVec1[i] = fileSectionData;
			}
			std::vector<std::string> resVec(num_parts);
			for (int i = 0; i < num_parts; i++) {
				thVec[i] = std::thread([&fileSectionDataVec1, &huffmanCodeVec1, zeros, i, &resVec]() {
					resVec[i] = decode(fileSectionDataVec1[i], huffmanCodeVec1[i], zeros);
					});
			}
			for (auto& th : thVec) th.join();

			std::ofstream w1File(newName, std::ios::binary);
			for (auto& i : resVec) {
				w1File << i;
			}
			std::cout << clock() - begClock << std::endl;
		}

		if (mode == 2) {

			std::ifstream rFile(fileName, std::ios::binary);
			if (!rFile) {
				std::cerr << "[-] Error opening file: " << fileName << std::endl;
				std::exit(0);
			}
			fileName.resize(fileName.size() - 4);
			std::string fileExt;
			rFile >> fileExt;
			std::string newName = fileName + "_d." + fileExt;

			int zeros = 0;
			std::vector<std::vector<std::pair<char, std::string>>> huffmanCodeVec(num_parts);
			std::vector<std::vector<char>> fileSectionDataVec(num_parts);

			for (int i = 0; i < num_parts; i++) {
				std::vector<std::pair<char, std::string>> huffmanCode;
				auto fileSectionData = readFile2(rFile, huffmanCode, zeros);
				huffmanCodeVec[i] = huffmanCode;
				fileSectionDataVec[i] = fileSectionData;
			}
			if (!fileSectionDataVec.empty() and !huffmanCodeVec.empty()) {
				std::cout << "[+] File is successfully opened\nPlease, wait...\n";
			}
			std::vector<std::thread> thVec(num_parts);
			std::vector<std::string> resVec(num_parts);
			for (int i = 0; i < num_parts; i++) {
				thVec[i] = std::thread([&fileSectionDataVec, &huffmanCodeVec, zeros, i, &resVec]() {
					resVec[i] = decode(fileSectionDataVec[i], huffmanCodeVec[i], zeros);
					});
			}
			for (auto& th : thVec) th.join();

			std::ofstream wFile(newName, std::ios::binary);
			for (auto& i : resVec) {
				wFile << i;
			}
			std::cout << "[+] File successfully decoded in " << newName << "\n";
		}
	}
	
}