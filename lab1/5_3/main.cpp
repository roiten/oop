#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

constexpr int MAX_IMAGE_SIZE = 100;

struct Point
{
	int row;
	int col;
	Point(int r, int c)
		: row(r)
		, col(c)
	{
	}
};

enum class Mode
{
	File,
	Stdin,
	Help
};

Mode ParseArguments(int argc, char* argv[], std::string& inputFile, std::string& outputFile);
void PrintHelp();
void ProcessFiles(const std::string& inputFilePath, const std::string& outputFilePath);
void ProcessConsole();
void LoadImage(std::istream& input, std::vector<std::string>& image);
void FindStartPoints(const std::vector<std::string>& image, std::stack<Point>& startPoints);
bool IsValidPoint(int row, int col, int rows, int cols);
bool IsBoundary(char ch);
void FloodFill(std::vector<std::string>& image, int startRow, int startCol);
void ProcessImage(std::vector<std::string>& image);
void SaveImage(std::ostream& output, const std::vector<std::string>& image);

int main(const int argc, char* argv[])
{
	try
	{
		std::string inputFilePath, outputFilePath;
		switch (ParseArguments(argc, argv, inputFilePath, outputFilePath))
		{
		case Mode::Help:
			PrintHelp();
			return 0;

		case Mode::File:
			ProcessFiles(inputFilePath, outputFilePath);
			break;

		case Mode::Stdin:
			ProcessConsole();
			break;
		}

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
}

Mode ParseArguments(int argc, char* argv[], std::string& inputFile, std::string& outputFile)
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		return Mode::Help;
	}
	if (argc == 3)
	{
		inputFile = argv[1];
		outputFile = argv[2];
		return Mode::File;
	}
	if (argc == 1)
	{
		return Mode::Stdin;
	}

	throw std::exception("ERROR. Invalid arguments");
}

void PrintHelp()
{
	std::cout << "fill.exe - программа для заливки контуров\n"
			  << "Использование:\n"
			  << "Максимальный размер изображения: " << MAX_IMAGE_SIZE << "x" << MAX_IMAGE_SIZE << "\n"
			  << "fill.exe <input file> <output file> - обработка файлов\n"
			  << "fill.exe - ввод через консоль\n"
			  << "fill.exe -h - показать справку\n"
			  << "Символ 'O' (англ.) - точки начала заливки\n"
			  << "Символ '#' - границы контуров\n"
			  << "Символ '.' - результат заливки\n"
			  << std::endl;
}

void ProcessFiles(const std::string& inputFilePath, const std::string& outputFilePath)
{
	std::ifstream inputFile(inputFilePath);
	if (!inputFile.is_open())
	{
		throw std::exception("ERROR. Cannot open input file");
	}

	std::ofstream outputFile(outputFilePath);
	if (!outputFile.is_open())
	{
		throw std::exception("ERROR. Cannot open output file");
	}

	std::vector<std::string> image;
	LoadImage(inputFile, image);
	ProcessImage(image);
	SaveImage(outputFile, image);
}

void ProcessConsole()
{
	std::vector<std::string> image;
	LoadImage(std::cin, image);
	ProcessImage(image);
	std::cout << "Result:" << std::endl;
	SaveImage(std::cout, image);
}

void LoadImage(std::istream& input, std::vector<std::string>& image)
{
	std::string line;
	int rowCount = 0;

	while (std::getline(input, line) && rowCount < MAX_IMAGE_SIZE)
	{
		if (line.length() > MAX_IMAGE_SIZE)
		{
			line = line.substr(0, MAX_IMAGE_SIZE);
		}

		image.push_back(line);
		rowCount++;
	}
}

void FindStartPoints(const std::vector<std::string>& image, std::stack<Point>& startPoints)
{
	for (size_t i = 0; i < image.size(); i++)
	{
		for (size_t j = 0; j < image[i].length(); j++)
		{
			if (image[i][j] == 'O')
			{
				startPoints.emplace(static_cast<int>(i), static_cast<int>(j));
			}
		}
	}
}

bool IsValidPoint(int row, int col, int rows, int cols)
{
	return row >= 0 && row < rows && col >= 0 && col < cols;
}

bool IsBoundary(char ch)
{
	return ch == '#';
}

void FloodFill(std::vector<std::string>& image, int startRow, int startCol)
{
	int rows = static_cast<int>(image.size());
	if (rows == 0)
		return;

	int cols = static_cast<int>(image[0].length());

	if (!IsValidPoint(startRow, startCol, rows, cols))
	{
		return;
	}

	if (IsBoundary(image[startRow][startCol]))
	{
		return;
	}

	std::stack<Point> points;
	points.emplace(startRow, startCol);

	while (!points.empty())
	{
		const Point current = points.top();
		points.pop();

		if (!IsValidPoint(current.row, current.col, rows, cols))
		{
			continue;
		}

		char currentChar = image[current.row][current.col];

		if (IsBoundary(currentChar))
		{
			continue;
		}

		if (currentChar == '.')
		{
			continue;
		}

		if (currentChar == ' ')
		{
			image[current.row][current.col] = '.';
		}

		if (IsValidPoint(current.row - 1, current.col, rows, cols))
		{
			char neighborChar = image[current.row - 1][current.col];
			if (!IsBoundary(neighborChar) && neighborChar != '.' && neighborChar != 'O')
			{
				points.emplace(current.row - 1, current.col);
			}
		}

		if (IsValidPoint(current.row + 1, current.col, rows, cols))
		{
			char neighborChar = image[current.row + 1][current.col];
			if (!IsBoundary(neighborChar) && neighborChar != '.' && neighborChar != 'O')
			{
				points.emplace(current.row + 1, current.col);
			}
		}

		if (IsValidPoint(current.row, current.col - 1, rows, cols))
		{
			char neighborChar = image[current.row][current.col - 1];
			if (!IsBoundary(neighborChar) && neighborChar != '.' && neighborChar != 'O')
			{
				points.emplace(current.row, current.col - 1);
			}
		}

		if (IsValidPoint(current.row, current.col + 1, rows, cols))
		{
			char neighborChar = image[current.row][current.col + 1];
			if (!IsBoundary(neighborChar) && neighborChar != '.' && neighborChar != 'O')
			{
				points.emplace(current.row, current.col + 1);
			}
		}
	}
}

void ProcessImage(std::vector<std::string>& image)
{
	if (image.empty())
		return;

	if (image.size() > MAX_IMAGE_SIZE)
	{
		image.resize(MAX_IMAGE_SIZE);
	}

	for (auto& line : image)
	{
		if (line.length() > MAX_IMAGE_SIZE)
		{
			line = line.substr(0, MAX_IMAGE_SIZE);
		}
		else if (line.length() < MAX_IMAGE_SIZE)
		{
			line.append(MAX_IMAGE_SIZE - line.length(), ' ');
		}
	}

	std::stack<Point> startPoints;
	FindStartPoints(image, startPoints);

	while (!startPoints.empty())
	{
		const Point p = startPoints.top();
		startPoints.pop();

		FloodFill(image, p.row, p.col);
	}
}

void SaveImage(std::ostream& output, const std::vector<std::string>& image)
{
	for (const auto& line : image)
	{
		output << line << std::endl;
	}
}
