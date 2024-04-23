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

	std::cout << "Enter file name: ";
	std::string fileName;
	std::cin >> fileName;
	const int parts[19] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 100, 200, 300, 400, 500 };

	for (auto num_parts : parts) {
		auto begClock = clock();
		std::string fileData;
		readFile1(fileName, fileData);

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

		std::vector<std::vector<char>> toWriteVec(num_parts);
		std::vector<std::vector<bool>> dataVec(num_parts);

		for (int i = 0; i < num_parts; i++) {
			thVec[i] = std::thread([&fileSectionDataVec, i, &huffmanCodeVec, &dataVec, &toWriteVec]() {
				std::vector<bool> data;
				for (char ch : fileSectionDataVec[i]) {
					for (auto i : huffmanCodeVec[i][ch]) {
						if (i == '1') data.push_back(1);
						else data.push_back(0);
					}
				}
				dataVec[i] = data;
				for (size_t it = 0; it < data.size(); it += 8) {
					char sym = 0;
					int k = 128;
					for (int j = 0; j < 8; j++) {
						sym += data[it + j] * k;
						k /= 2;
					}
					toWriteVec[i].push_back(sym);
				}
			});
		}
		
		for (auto& th : thVec) th.join();

		for (auto& i : toWriteVec) size += i.size();

		for (int i = 0; i < num_parts; i++) {
			writeFile1(huffmanCodeVec[i], wFile, fileSectionDataVec[i], size, dataVec[i], toWriteVec[i]);
		}

		wFile.close();
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

		std::vector<int> zerosVec1(num_parts);
		std::vector<std::vector<std::pair<char, std::string>>> huffmanCodeVec1(num_parts);
		std::vector<std::vector<char>> fileSectionDataVec1(num_parts);

		for (int i = 0; i < num_parts; i++) {
			int zeros = 0;
			std::vector<std::pair<char, std::string>> huffmanCode;
			auto fileSectionData = readFile2(rFile, huffmanCode, zeros);
			huffmanCodeVec1[i] = huffmanCode;
			fileSectionDataVec1[i] = fileSectionData;
			zerosVec1[i] = zeros;
		}
		std::vector<std::string> resVec(num_parts);
		for (int i = 0; i < num_parts; i++) {
			thVec[i] = std::thread([&fileSectionDataVec1, &huffmanCodeVec1, &zerosVec1, i, &resVec]() {
				resVec[i] = decode(fileSectionDataVec1[i], huffmanCodeVec1[i], zerosVec1[i]);
				});
		}

		for (auto& th : thVec) th.join();
		
		std::ofstream w1File(newName, std::ios::binary);
		std::vector<char> temp;
		for (auto& sec : resVec) {
			w1File << sec;
		}
		std::cout << clock() - begClock << std::endl;
	}
}