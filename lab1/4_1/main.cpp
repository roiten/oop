#include "generate/FileGen.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

void Compress(const std::string& inputPath,
	const std::string& outputPath)
{
	std::ifstream input(inputPath, std::ios::binary);
	if (!input)
	{
		std::cerr << "Cannot open input file: " << inputPath << std::endl;
		throw std::runtime_error("Cannot open input file");
	}
	std::ofstream output(outputPath, std::ios::binary);
	if (!output)
		throw std::runtime_error("Cannot open output file");

	char current;
	if (!input.get(current))
		return;

	unsigned char count = 1;
	char next;

	while (input.get(next))
	{
		if (next == current && count < 255)
		{
			count++;
		}
		else
		{
			output.put(static_cast<char>(count));
			output.put(current);

			current = next;
			count = 1;
		}
	}

	output.put(static_cast<char>(count));
	output.put(current);
}

void Decompress(const std::string& inputPath,
	const std::string& outputPath)
{
	std::ifstream input(inputPath, std::ios::binary);
	if (!input)
	{
		std::cerr << "Cannot open input file: " << inputPath << std::endl;
		throw std::exception("Cannot open input file");
	}
	std::ofstream output(outputPath, std::ios::binary);
	if (!output)
		throw std::exception("Cannot open output file");

	char countByte;
	char value;

	while (input.get(countByte))
	{
		if (!input.get(value))
			throw std::exception("Corrupted file");

		auto count = static_cast<unsigned char>(countByte);

		if (count == 0)
			throw std::exception("Invalid repeat count");

		for (size_t i = 0; i < count; ++i)
			output.put(value);
	}
}

int main(int argc, char* argv[])
{
	try
	{
		generateFiles();

		if (argc != 4)
			throw std::runtime_error(
				"Usage: rle pack/unpack <input> <output>");

		std::string mode = argv[1];

		if (mode == "pack")
			Compress(argv[2], argv[3]);
		else if (mode == "unpack")
			Decompress(argv[2], argv[3]);
		else
			throw std::runtime_error("Unknown mode");

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}