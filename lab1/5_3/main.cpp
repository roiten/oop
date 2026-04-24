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
void FindStartPoints(const std::vector<std::string>& image, std::vector<Point>& startPoints);
bool IsValidPoint(int row, int col, int rows, int cols);
bool IsBoundary(char ch);
void FloodFill(std::vector<std::string>& image, int startRow, int startCol);
void ProcessImage(std::vector<std::string>& image);
void SaveImage(std::ostream& output, const std::vector<std::string>& image);

int main(const int argc, char* argv[])
{
	std::locale::global(std::locale("ru_RU.utf8"));

	std::string inputFilePath, outputFilePath;
	Mode mode;

	try
	{
		mode = ParseArguments(argc, argv, inputFilePath, outputFilePath);
	}
	catch (...)
	{
		std::cout << "ERROR" << std::endl;
		return 1;
	}

	if (mode == Mode::Help)
	{
		PrintHelp();
		return 0;
	}

	if (mode == Mode::File)
	{
		try
		{
			ProcessFiles(inputFilePath, outputFilePath);
		}
		catch (...)
		{
			std::cout << "ERROR" << std::endl;
			return 1;
		}
		return 0;
	}

	try
	{
		ProcessConsole();
	}
	catch (...)
	{
		std::cout << "ERROR" << std::endl;
		return 0;
	}
	return 0;
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

	throw std::runtime_error("ERROR");
}

void PrintHelp()
{
	std::cout << "fill.exe - программа для заливки контуров\n"
			  << "Использование:\n"
			  << "fill.exe -h - инструкция по использованию\n"
			  << "fill.exe <input file> <output file>\n"
			  << "Максимальный размер изображения: " << MAX_IMAGE_SIZE << "x" << MAX_IMAGE_SIZE << "\n"
			  << "fill.exe - ввод через stdin\n"
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
		throw std::runtime_error("ERROR");
	}

	std::ofstream outputFile(outputFilePath);
	if (!outputFile.is_open())
	{
		throw std::runtime_error("ERROR");
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
	SaveImage(std::cout, image);
}

void LoadImage(std::istream& input, std::vector<std::string>& image)
{
	std::string line;
	int rowCount = 0;

	while (std::getline(input, line) && rowCount < MAX_IMAGE_SIZE)
	{
		if (static_cast<int>(line.length()) > MAX_IMAGE_SIZE)
		{
			line = line.substr(0, MAX_IMAGE_SIZE);
		}

		image.push_back(line);
		rowCount++;
	}
}

void FindStartPoints(const std::vector<std::string>& image, std::vector<Point>& startPoints)
{
	for (int i = 0; i < static_cast<int>(image.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(image[i].size()); j++)
		{
			if (image[i][j] == 'O')
			{
				//заиспользовать find
				startPoints.emplace_back(i, j);
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

int GetRowWidth(const std::vector<std::string>& image, const int row)
{
	return static_cast<int>(image[row].length());
}

char GetChar(const std::vector<std::string>& image, int row, int col)
{
	if (row < 0 || row >= static_cast<int>(image.size()))
		return '\0';
	if (col < 0 || col >= static_cast<int>(image[row].length()))
		return ' ';
	return image[row][col];
}

void SetChar(std::vector<std::string>& image, int row, int col, char ch)
{
	if (row < 0 || row >= static_cast<int>(image.size()))
		return;
	if (col >= MAX_IMAGE_SIZE)
		return;
	if (col >= static_cast<int>(image[row].length()))
	{
		image[row].append(col - static_cast<int>(image[row].length()) + 1, ' ');
	}
	image[row][col] = ch;
}

void FloodFill(std::vector<std::string>& image, int startRow, int startCol)
{
	int rows = static_cast<int>(image.size());
	if (rows == 0)
		return;

	char startChar = GetChar(image, startRow, startCol);
	if (IsBoundary(startChar))
		return;

	//подумать как подразбить
	//добавляем свои типы

	std::vector<std::vector<bool>> visited(rows, std::vector<bool>(MAX_IMAGE_SIZE, false));

	std::stack<Point> points;
	points.emplace(startRow, startCol);
	visited[startRow][startCol] = true;

	const int rowStep[] = { -1, 1, 0, 0 };
	const int columnStep[] = { 0, 0, -1, 1 };

	while (!points.empty())
	{
		const Point current = points.top();
		points.pop();

		const char ch = GetChar(image, current.row, current.col);

		if (IsBoundary(ch))
			continue;

		if (ch == ' ')
		{
			SetChar(image, current.row, current.col, '.');
		}

		for (int availableStep = 0; availableStep < 4; availableStep++)
		{
			int nextPointRowPos = current.row + rowStep[availableStep];
			int nextPointColumnPos = current.col + columnStep[availableStep];

			if (nextPointRowPos < 0 || nextPointRowPos >= rows || nextPointColumnPos < 0 || nextPointColumnPos >= MAX_IMAGE_SIZE)
				continue;

			if (visited[nextPointRowPos][nextPointColumnPos])
				continue;

			char neighborChar = GetChar(image, nextPointRowPos, nextPointColumnPos);
			if (IsBoundary(neighborChar) || neighborChar == '.')
				continue;

			visited[nextPointRowPos][nextPointColumnPos] = true;
			points.emplace(nextPointRowPos, nextPointColumnPos);
		}
	}
}

void ProcessImage(std::vector<std::string>& image)
//разбить на подфункции
{
	if (image.empty())
		return;

	if (static_cast<int>(image.size()) > MAX_IMAGE_SIZE)
	{
		image.resize(MAX_IMAGE_SIZE);
	}

	for (auto& line : image)
	{
		if (static_cast<int>(line.length()) > MAX_IMAGE_SIZE)
		{
			line = line.substr(0, MAX_IMAGE_SIZE);
		}
		else if (static_cast<int>(line.length()) < MAX_IMAGE_SIZE)
		{
			line.append(MAX_IMAGE_SIZE - static_cast<int>(line.length()), ' ');
		}
	}

	while (static_cast<int>(image.size()) < MAX_IMAGE_SIZE)
	{
		image.emplace_back(MAX_IMAGE_SIZE, ' ');
	}

	std::vector<Point> startPoints;
	FindStartPoints(image, startPoints);

	for (const auto& p : startPoints)
	{
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