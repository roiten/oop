#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

static const unsigned char TYPE_REPEAT = 0x80; // старший бит

void WriteRepeat(std::ofstream& out, unsigned char value, int length)
{
    while (length > 0)
    {
        int chunk = std::min(length, 129);
        unsigned char header =
            TYPE_REPEAT | static_cast<unsigned char>(chunk - 2);
        out.put(static_cast<char>(header));
        out.put(static_cast<char>(value));
        length -= chunk;
    }
}

void WriteLiteral(std::ofstream& out,
                  const std::vector<unsigned char>& buffer)
{
    size_t pos = 0;
    while (pos < buffer.size())
    {
        size_t chunk = std::min<size_t>(128, buffer.size() - pos);
        unsigned char header =
            static_cast<unsigned char>(chunk - 1);
        out.put(static_cast<char>(header));

        for (size_t i = 0; i < chunk; ++i)
            out.put(static_cast<char>(buffer[pos + i]));

        pos += chunk;
    }
}

void Compress(const std::string& inputPath,
              const std::string& outputPath)
{
    std::ifstream in(inputPath, std::ios::binary);
    if (!in)
        throw std::runtime_error("Cannot open input file");

    std::ofstream out(outputPath, std::ios::binary);
    if (!out)
        throw std::runtime_error("Cannot open output file");

    std::vector<unsigned char> literalBuffer;

    int prev = in.get();
    if (prev == EOF)
        return;

    int count = 1;

    while (true)
    {
        int current = in.get();

        if (current == prev && count < 129)
        {
            ++count;
        }
        else
        {
            if (count >= 2)
            {
                if (!literalBuffer.empty())
                {
                    WriteLiteral(out, literalBuffer);
                    literalBuffer.clear();
                }
                WriteRepeat(out, static_cast<unsigned char>(prev), count);
            }
            else
            {
                literalBuffer.push_back(static_cast<unsigned char>(prev));
            }

            if (current == EOF)
                break;

            prev = current;
            count = 1;
        }
    }

    if (!literalBuffer.empty())
        WriteLiteral(out, literalBuffer);
}

void Decompress(const std::string& inputPath,
                const std::string& outputPath)
{
    std::ifstream in(inputPath, std::ios::binary);
    if (!in)
        throw std::runtime_error("Cannot open input file");

    std::ofstream out(outputPath, std::ios::binary);
    if (!out)
        throw std::runtime_error("Cannot open output file");

    int header;

    while ((header = in.get()) != EOF)
    {
        bool isRepeat = (header & TYPE_REPEAT) != 0;
        unsigned char lengthField =
            static_cast<unsigned char>(header & 0x7F);

        if (isRepeat)
        {
            int length = lengthField + 2;
            int value = in.get();
            if (value == EOF)
                throw std::runtime_error("Corrupted file");

            for (int i = 0; i < length; ++i)
                out.put(static_cast<char>(value));
        }
        else
        {
            int length = lengthField + 1;
            for (int i = 0; i < length; ++i)
            {
                int value = in.get();
                if (value == EOF)
                    throw std::runtime_error("Corrupted file");
                out.put(static_cast<char>(value));
            }
        }
    }
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 4)
            throw std::runtime_error(
                "Usage: rle pack|unpack <input> <output>");

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