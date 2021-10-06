#include <iostream>
#include <string>
#include <vector>
#include <cuda.h>

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

    // Initialize data
    const size_t numberOfElements = 1024 * 1024;    
    std::vector<float> inputHost(numberOfElements);
    
    for (size_t i = 0; i < numberOfElements; ++i)
    {
        inputHost[i] = 1.0f; //static_cast<float>(i);
    }

    kttl::Initialize(deviceIndex);

    CUdeviceptr input;
    cuMemAlloc(&input, numberOfElements * sizeof(float));
    cuMemcpyHtoD(input, inputHost.data(), numberOfElements * sizeof(float));

    const float result = kttl::Reduce(input, numberOfElements, 0.0f, kttl::Plus<float>());
    kttl::Terminate();

    cuMemFree(input);
    return 0;
}
