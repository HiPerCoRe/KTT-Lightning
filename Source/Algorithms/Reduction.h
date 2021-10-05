#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <cuda.h>
#include <Ktt.h>

#include <Operators/Operator.h>

namespace kttl
{

class Reduction
{
public:
    Reduction(ktt::Tuner& tuner);

    void Initialize(const std::string& elementType, const Operator& op);
    std::vector<uint8_t> Run(CUdeviceptr srcBuffer, const size_t elementCount, const size_t elementSize, const std::string& typeName,
        const void* init, const Operator& op);

private:
    std::string m_DefaultSource;
    std::map<std::pair<std::string, std::string>, std::pair<ktt::KernelDefinitionId, ktt::KernelId>> m_TypeToKernel;
    ktt::Tuner& m_Tuner;
    ktt::ArgumentId m_SrcId;
    ktt::ArgumentId m_DstId;
    ktt::ArgumentId m_NId;
    ktt::ArgumentId m_InOffsetId;
    ktt::ArgumentId m_OutOffsetId;
    CUdeviceptr m_DstBuffer;
    size_t m_BufferSize;

    void UpdateArguments(const std::pair<std::string, std::string>& typePair);
    void ClearArguments(const std::pair<std::string, std::string>& typePair);
};

} // namespace kttl
