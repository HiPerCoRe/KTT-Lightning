#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <cuda.h>
#include <rttr/registration>
#include <Ktt.h>

#include <Operators/Operator.h>

namespace kttl
{

class Reduction
{
public:
    Reduction(ktt::Tuner& tuner);

    void Initialize(const rttr::type& elementType, const Operator& op);
    rttr::variant Run(CUdeviceptr srcBuffer, const size_t elementCount, const rttr::variant& init, const Operator& op);

private:
    std::string m_DefaultSource;
    std::map<std::pair<rttr::type, std::string>, std::pair<ktt::KernelDefinitionId, ktt::KernelId>> m_TypeToKernel;
    ktt::Tuner& m_Tuner;
    ktt::ArgumentId m_SrcId;
    ktt::ArgumentId m_DstId;
    ktt::ArgumentId m_NId;
    ktt::ArgumentId m_InOffsetId;
    ktt::ArgumentId m_OutOffsetId;
    CUdeviceptr m_DstBuffer;
    size_t m_BufferSize;

    void UpdateArguments(const std::pair<rttr::type, std::string>& typePair);
    void ClearArguments(const std::pair<rttr::type, std::string>& typePair);
};

} // namespace kttl
