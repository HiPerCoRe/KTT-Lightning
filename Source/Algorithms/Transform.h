#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <cuda.h>
#include <Ktt.h>

#include <Operators/Operator.h>

namespace kttl
{

class Transform
{
public:
    Transform(ktt::Tuner& tuner);

    void Initialize(const std::string& elementType, const Operator& op, const size_t elementCount);
    std::vector<uint8_t> Run(CUdeviceptr srcBuffer1, CUdeviceptr srcBuffer2, CUdeviceptr dstBuffer, const size_t elementCount, const size_t elementSize,
        const std::string& typeName, const Operator& op);

private:
    std::string m_DefaultSource;
    std::map<std::pair<std::string, std::string>, std::pair<ktt::KernelDefinitionId, ktt::KernelId>> m_TypeToKernel;
    ktt::Tuner& m_Tuner;
    ktt::ArgumentId m_Src1Id;
    ktt::ArgumentId m_Src2Id;
    ktt::ArgumentId m_DstId;
    ktt::ArgumentId m_NId;

    void SetArguments(const std::pair<std::string, std::string>& typePair);
    void ClearArguments(const std::pair<std::string, std::string>& typePair);
};

} // namespace kttl
