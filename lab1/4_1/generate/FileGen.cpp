#include <fstream>

void createFile(const std::string& path, char value, size_t count) {
	std::ofstream out(path, std::ios::binary);
	out.write(std::string(count, value).c_str(), count);
}

void generateFiles() {
	createFile("empty.bin", 0, 0);
	createFile("255.bin", 'A', 255);
	createFile("256.bin", 'B', 256);
	createFile("257.bin", 'C', 257);
	createFile("255char.bin", static_cast<char>(255), 300);

	createFile("expected_pack_empty.bin", 0, 0);
	createFile("expected_pack_255.bin", 'A', 255);
	createFile("expected_pack_256.bin", 'B', 256);
	createFile("expected_pack_257.bin", 'C', 257);
	createFile("expected_pack_255char.bin", static_cast<char>(255), 300);
}