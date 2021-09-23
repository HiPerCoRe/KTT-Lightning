#include <iostream>
#include <string>
#include <vector>

#include <KttLightning.h>

#if defined(_MSC_VER)
    const std::string kernelPrefix = "";
#else
    const std::string kernelPrefix = "../";
#endif

int main(int argc, char** argv)
{
    uint32_t deviceIndex = 0;

    if (argc >= 2)
    {
        deviceIndex = std::stoul(std::string(argv[1]));
    }

    const size_t numberOfElements = 1024 * 1024;
    std::vector<float> input(numberOfElements);

    // Initialize data
    for (size_t i = 0; i < numberOfElements; ++i)
    {
        input[i] = static_cast<float>(i);
    }

    kttl::Initialize(deviceIndex);

    const float result = kttl::Reduce(input.begin(), input.end(), 0.0f, kttl::Plus<float>());
    kttl::Terminate();
    return 0;
}
