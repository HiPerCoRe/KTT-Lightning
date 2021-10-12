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

    CUdeviceptr input1;
    cuMemAlloc(&input1, numberOfElements * sizeof(float));
    cuMemcpyHtoD(input1, inputHost.data(), numberOfElements * sizeof(float));

    CUdeviceptr input2;
    cuMemAlloc(&input2, numberOfElements * sizeof(float));
    cuMemcpyHtoD(input2, inputHost.data(), numberOfElements * sizeof(float));

    kttl::Transform<float>(input1, input2, input2, numberOfElements, kttl::Plus<float>());

    std::vector<float> outputHost(numberOfElements);
    cuMemcpyDtoH(outputHost.data(), input2, numberOfElements * sizeof(float));

    kttl::Terminate();

    cuMemFree(input1);
    cuMemFree(input2);
    return 0;
}
