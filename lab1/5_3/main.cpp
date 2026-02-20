#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

constexpr int MAX_SIZE = 100;

struct Point
{
    int row;
    int col;
    Point(int r, int c) : row(r), col(c) {}
};

// Режимы работы программы
enum class Mode
{
    File,
    Stdin,
    Help
};

// Прототипы функций
Mode ParseArguments(int argc, char* argv[], std::string& inputFile, std::string& outputFile);
void PrintHelp();
void ProcessFileMode(const std::string& inputFilePath, const std::string& outputFilePath);
void ProcessStdinMode();
void LoadImage(std::istream& input, std::vector<std::string>& image);
void FindStartPoints(const std::vector<std::string>& image, std::stack<Point>& startPoints);
bool IsValidPoint(int row, int col, int rows, int cols);
bool IsBoundary(char ch);
void FloodFill(std::vector<std::string>& image, int startRow, int startCol);
void ProcessImage(std::vector<std::string>& image);
void SaveImage(std::ostream& output, const std::vector<std::string>& image);
void HandleError();

int main(int argc, char* argv[])
{
    try
    {
        std::string inputFilePath, outputFilePath;
        Mode mode = ParseArguments(argc, argv, inputFilePath, outputFilePath);

        switch (mode)
        {
            case Mode::Help:
                PrintHelp();
                return 0;

            case Mode::File:
                ProcessFileMode(inputFilePath, outputFilePath);
                break;

            case Mode::Stdin:
                ProcessStdinMode();
                break;
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        HandleError();
        return 1;
    }
}

Mode ParseArguments(int argc, char* argv[], std::string& inputFile, std::string& outputFile)
{
    if (argc == 2 && std::string(argv[1]) == "-h")
    {
        return Mode::Help;
    }
    else if (argc == 3)
    {
        inputFile = argv[1];
        outputFile = argv[2];
        return Mode::File;
    }
    else if (argc == 1)
    {
        return Mode::Stdin;
    }
    else
    {
        throw std::runtime_error("Invalid arguments");
    }
}

void PrintHelp()
{
    std::cout << "fill.exe - программа для заливки контуров\n"
              << "Использование:\n"
              << "  fill.exe <input file> <output file>  - обработка файлов\n"
              << "  fill.exe                             - ввод через stdin\n"
              << "  fill.exe -h                           - показать справку\n"
              << "Символ 'O' - точки начала заливки\n"
              << "Символ '#' - границы контуров\n"
              << "Символ '.' - результат заливки\n"
              << "Максимальный размер изображения: " << MAX_SIZE << "x" << MAX_SIZE << std::endl;
}

void ProcessFileMode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open())
    {
        throw std::runtime_error("Cannot open input file");
    }

    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open())
    {
        throw std::runtime_error("Cannot open output file");
    }

    std::vector<std::string> image;
    LoadImage(inputFile, image);
    ProcessImage(image);
    SaveImage(outputFile, image);
}

void ProcessStdinMode()
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

    while (std::getline(input, line) && rowCount < MAX_SIZE)
    {
        // Обрезаем строку до MAX_SIZE символов
        if (line.length() > MAX_SIZE)
        {
            line = line.substr(0, MAX_SIZE);
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
                startPoints.push(Point(static_cast<int>(i), static_cast<int>(j)));
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
    if (rows == 0) return;

    int cols = static_cast<int>(image[0].length());

    if (!IsValidPoint(startRow, startCol, rows, cols))
    {
        return;
    }

    // Если начальная точка - граница, ничего не делаем
    if (IsBoundary(image[startRow][startCol]))
    {
        return;
    }

    std::stack<Point> points;
    points.push(Point(startRow, startCol));

    while (!points.empty())
    {
        Point current = points.top();
        points.pop();

        int r = current.row;
        int c = current.col;

        if (!IsValidPoint(r, c, rows, cols))
        {
            continue;
        }

        // Если текущая точка - граница или уже обработана, пропускаем
        char currentChar = image[r][c];
        if (IsBoundary(currentChar) || currentChar == '.')
        {
            continue;
        }

        // Заливаем текущую точку
        image[r][c] = '.';

        // Проверяем всех четырех соседей явно
        // Сосед сверху
        if (IsValidPoint(r - 1, c, rows, cols) && !IsBoundary(image[r - 1][c]) && image[r - 1][c] != '.')
        {
            points.push(Point(r - 1, c));
        }

        // Сосед снизу
        if (IsValidPoint(r + 1, c, rows, cols) && !IsBoundary(image[r + 1][c]) && image[r + 1][c] != '.')
        {
            points.push(Point(r + 1, c));
        }

        // Сосед слева
        if (IsValidPoint(r, c - 1, rows, cols) && !IsBoundary(image[r][c - 1]) && image[r][c - 1] != '.')
        {
            points.push(Point(r, c - 1));
        }

        // Сосед справа
        if (IsValidPoint(r, c + 1, rows, cols) && !IsBoundary(image[r][c + 1]) && image[r][c + 1] != '.')
        {
            points.push(Point(r, c + 1));
        }
    }
}

void ProcessImage(std::vector<std::string>& image)
{
    if (image.empty()) return;

    // Находим все стартовые точки
    std::stack<Point> startPoints;
    FindStartPoints(image, startPoints);

    // Выравниваем все строки до одинаковой длины (по максимальной)
    size_t maxLength = 0;
    for (const auto& line : image)
    {
        maxLength = std::max(maxLength, line.length());
    }

    for (auto& line : image)
    {
        if (line.length() < maxLength)
        {
            line.append(maxLength - line.length(), ' ');
        }
    }

    // Выполняем заливку от каждой стартовой точки
    while (!startPoints.empty())
    {
        Point p = startPoints.top();
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

void HandleError()
{
    std::cout << "ERROR" << std::endl;
}