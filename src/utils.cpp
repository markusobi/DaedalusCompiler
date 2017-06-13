#include "utils.hpp"
#include <fstream>
#include <streambuf>

namespace Utils
{
    std::string readAllText(const std::string& filename)
    {
        // from https://stackoverflow.com/a/2602060/2056153
        std::ifstream fileStream(filename);
        if (fileStream.fail())
            throw std::runtime_error("Error: couldn't open file \"" + filename + '"');
        std::string content;

        fileStream.seekg(0, std::ios::end);
        content.reserve(fileStream.tellg());
        fileStream.seekg(0, std::ios::beg);

        content.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

        return content;
    }
}