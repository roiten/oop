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

bool g_isFileMode = false;

int main(const int argc, char* argv[])
{
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
		g_isFileMode = true;
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
			  << "Максимальный размер изображения: " << MAX_IMAGE_SIZE << "x" << MAX_IMAGE_SIZE << "\n"
			  << "fill.exe <input file> <output file> - обработка файлов\n"
			  << "fill.exe - ввод через stdin\n"
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
		if ((int)line.length() > MAX_IMAGE_SIZE)
		{
			line = line.substr(0, MAX_IMAGE_SIZE);
		}

		image.push_back(line);
		rowCount++;
	}
}

void FindStartPoints(const std::vector<std::string>& image, std::vector<Point>& startPoints)
{
	for (int i = 0; i < (int)image.size(); i++)
	{
		for (int j = 0; j < (int)image[i].length(); j++)
		{
			if (image[i][j] == 'O')
			{
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

int GetRowWidth(const std::vector<std::string>& image, int row)
{
	return (int)image[row].length();
}

char GetChar(const std::vector<std::string>& image, int row, int col)
{
	if (row < 0 || row >= (int)image.size())
		return '\0';
	if (col < 0 || col >= (int)image[row].length())
		return ' '; // за пределами строки — пустое место
	return image[row][col];
}

void SetChar(std::vector<std::string>& image, int row, int col, char ch)
{
	if (row < 0 || row >= (int)image.size())
		return;
	if (col >= MAX_IMAGE_SIZE)
		return;
	if (col >= (int)image[row].length())
	{
		image[row].append(col - (int)image[row].length() + 1, ' ');
	}
	image[row][col] = ch;
}

void FloodFill(std::vector<std::string>& image, int startRow, int startCol)
{
	int rows = (int)image.size();
	if (rows == 0)
		return;

	char startChar = GetChar(image, startRow, startCol);
	if (IsBoundary(startChar))
		return;

	std::vector<std::vector<bool>> visited(rows, std::vector<bool>(MAX_IMAGE_SIZE, false));

	std::stack<Point> points;
	points.emplace(startRow, startCol);
	visited[startRow][startCol] = true;

	const int dr[] = { -1, 1, 0, 0 };
	const int dc[] = { 0, 0, -1, 1 };

	while (!points.empty())
	{
		const Point current = points.top();
		points.pop();

		char ch = GetChar(image, current.row, current.col);

		if (IsBoundary(ch))
			continue;

		if (ch == ' ')
		{
			SetChar(image, current.row, current.col, '.');
		}

		for (int d = 0; d < 4; d++)
		{
			int nr = current.row + dr[d];
			int nc = current.col + dc[d];

			if (nr < 0 || nr >= rows || nc < 0 || nc >= MAX_IMAGE_SIZE)
				continue;

			if (visited[nr][nc])
				continue;

			char neighborChar = GetChar(image, nr, nc);
			if (IsBoundary(neighborChar) || neighborChar == '.')
				continue;

			visited[nr][nc] = true;
			points.emplace(nr, nc);
		}
	}
}

void ProcessImage(std::vector<std::string>& image)
{
	if (image.empty())
		return;

	if ((int)image.size() > MAX_IMAGE_SIZE)
	{
		image.resize(MAX_IMAGE_SIZE);
	}

	for (auto& line : image)
	{
		if ((int)line.length() > MAX_IMAGE_SIZE)
		{
			line = line.substr(0, MAX_IMAGE_SIZE);
		}
		else if ((int)line.length() < MAX_IMAGE_SIZE)
		{
			line.append(MAX_IMAGE_SIZE - (int)line.length(), ' ');
		}
	}

	while ((int)image.size() < MAX_IMAGE_SIZE)
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