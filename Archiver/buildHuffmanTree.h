std::unordered_map<char, std::string> buildHuffmanTree(const std::string text) {

	std::unordered_map<char, int> freq;
	for (auto i : text) {
		freq[i]++;
	}

	std::priority_queue<Node*, std::vector<Node*>, comp> pq;

	for (auto& pair : freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (pq.size() != 1)
	{
		Node* left = pq.top(); pq.pop();
		Node* right = pq.top();	pq.pop();
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	Node* root = pq.top();

	std::unordered_map<char, std::string> huffmanCode;
	encode(root, "", huffmanCode);

	return huffmanCode;
}