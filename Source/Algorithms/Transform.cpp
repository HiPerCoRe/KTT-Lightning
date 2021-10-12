#include <Algorithms/Transform.h>
#include <Utility/FileSystem.h>
#include <Utility/StlHelpers.h>

namespace kttl
{

Transform::Transform(ktt::Tuner& tuner) :
    m_Tuner(tuner),
    m_Src1Id(ktt::InvalidArgumentId),
    m_Src2Id(ktt::InvalidArgumentId),
    m_DstId(ktt::InvalidArgumentId),
    m_NId(ktt::InvalidArgumentId)
{
    m_DefaultSource = LoadFileToString(KTTL_SOURCE + std::string("Kernels/Transform.cu"));
}

void Transform::Initialize(const std::string& elementType, const Operator& op, const size_t elementCount)
{
    // Todo: handle cases with changing element count
    const auto pair = std::make_pair(elementType, op.GetName());
    
    if (ContainsKey(m_TypeToKernel, pair))
    {
        return;
    }

    const auto operatorCode = op.GetCode();
    auto operatorTypeName = op.GetName();

    if (op.IsTemplated())
    {
        operatorTypeName += "< " + elementType + " >";
    }

    m_TypeToKernel[pair].first = m_Tuner.AddKernelDefinition("transform", operatorCode + m_DefaultSource, ktt::DimensionVector(elementCount), ktt::DimensionVector(), {elementType, operatorTypeName});
    m_TypeToKernel[pair].second = m_Tuner.CreateSimpleKernel("Transform", m_TypeToKernel[pair].first);
    const auto definitionId = m_TypeToKernel[pair].first;
    const auto kernelId = m_TypeToKernel[pair].second;

    m_Tuner.AddParameter(kernelId, "WORK_GROUP_SIZE", std::vector<uint64_t>{32, 64, 128, 256, 512, 1024});
    m_Tuner.AddThreadModifier(kernelId, {definitionId}, ktt::ModifierType::Local, ktt::ModifierDimension::X, "WORK_GROUP_SIZE", ktt::ModifierAction::Multiply);

    m_Tuner.AddParameter(kernelId, "ELEMENTS_PER_THREAD", std::vector<uint64_t>{1, 2, 4, 8, 16});
    m_Tuner.AddThreadModifier(kernelId, {definitionId}, ktt::ModifierType::Global, ktt::ModifierDimension::X, "ELEMENTS_PER_THREAD", ktt::ModifierAction::Divide);

    m_Tuner.AddParameter(kernelId, "UNROLL_FACTOR", std::vector<uint64_t>{1, 2, 4, 8});
}

std::vector<uint8_t> Transform::Run(CUdeviceptr srcBuffer1, CUdeviceptr srcBuffer2, CUdeviceptr dstBuffer, const size_t elementCount,
    const size_t elementSize, const std::string& typeName, const Operator& op)
{
    m_Tuner.SetGlobalSizeType(ktt::GlobalSizeType::OpenCL);
    Initialize(typeName, op, elementCount);
    const auto pair = std::make_pair(typeName, op.GetName());
    const size_t bufferSize = elementSize * elementCount;

    m_Src1Id = m_Tuner.AddArgumentVector(reinterpret_cast<ktt::ComputeBuffer>(srcBuffer1), bufferSize, elementSize,
        ktt::ArgumentAccessType::ReadOnly, ktt::ArgumentMemoryLocation::Device);
    m_Src2Id = m_Tuner.AddArgumentVector(reinterpret_cast<ktt::ComputeBuffer>(srcBuffer2), bufferSize, elementSize,
        ktt::ArgumentAccessType::ReadOnly, ktt::ArgumentMemoryLocation::Device);
    m_DstId = m_Tuner.AddArgumentVector(reinterpret_cast<ktt::ComputeBuffer>(dstBuffer), bufferSize, elementSize,
        ktt::ArgumentAccessType::ReadWrite, ktt::ArgumentMemoryLocation::Device);
    m_NId = m_Tuner.AddArgumentScalar(elementCount);
    SetArguments(pair);

    std::vector<uint8_t> result(bufferSize);
    ktt::BufferOutputDescriptor output(m_DstId, result.data(), bufferSize);
    m_Tuner.TuneIteration(m_TypeToKernel[pair].second, {output});

    ClearArguments(pair);
    return result;
}

void Transform::SetArguments(const std::pair<std::string, std::string>& typePair)
{
    const auto definition = m_TypeToKernel[typePair].first;
    m_Tuner.SetArguments(definition, {m_Src1Id, m_Src2Id, m_DstId, m_NId});
}

void Transform::ClearArguments(const std::pair<std::string, std::string>& typePair)
{
    const auto definition = m_TypeToKernel[typePair].first;
    m_Tuner.SetArguments(definition, {});

    if (m_DstId != ktt::InvalidArgumentId)
    {
        m_Tuner.RemoveArgument(m_DstId);
        m_DstId = ktt::InvalidArgumentId;
    }

    if (m_Src1Id != ktt::InvalidArgumentId)
    {
        m_Tuner.RemoveArgument(m_Src1Id);
        m_Src1Id = ktt::InvalidArgumentId;
    }

    if (m_Src2Id != ktt::InvalidArgumentId)
    {
        m_Tuner.RemoveArgument(m_Src2Id);
        m_Src2Id = ktt::InvalidArgumentId;
    }

    if (m_NId != ktt::InvalidArgumentId)
    {
        m_Tuner.RemoveArgument(m_NId);
        m_NId = ktt::InvalidArgumentId;
    }
}

} // namespace kttl
