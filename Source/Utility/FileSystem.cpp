#include <fstream>
#include <sstream>

#include <Api/KttException.h>
#include <Utility/FileSystem.h>

namespace kttl
{

std::string LoadFileToString(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw ktt::KttException("Unable to open file: " + filePath);
    }

    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

} // namespace kttl
