#include <Algorithms/Reduction.h>
#include <Utility/FileSystem.h>
#include <Utility/StlHelpers.h>

namespace kttl
{

Reduction::Reduction(ktt::Tuner& tuner) :
    m_Tuner(tuner),
    m_SrcId(ktt::InvalidArgumentId),
    m_DstId(ktt::InvalidArgumentId),
    m_NId(ktt::InvalidArgumentId),
    m_InOffsetId(ktt::InvalidArgumentId),
    m_OutOffsetId(ktt::InvalidArgumentId),
    m_DstBuffer(0),
    m_BufferSize(0)
{
    m_DefaultSource = LoadFileToString(KTTL_SOURCE + std::string("Kernels/Reduction.cu"));
}

void Reduction::Initialize(const std::string& elementType, const Operator& op)
{
    const auto pair = std::make_pair(elementType, op.GetName());

    if (ContainsKey(m_TypeToKernel, pair))
    {
        return;
    }

    const auto operatorName = op.GetName();
    const auto operatorCode = op.GetCode();
    auto operatorTypeName = operatorName;

    if (op.IsTemplated())
    {
        operatorTypeName += "< " + elementType + " >";
    }

    m_TypeToKernel[pair].first = m_Tuner.AddKernelDefinition("reduce", operatorCode + m_DefaultSource, ktt::DimensionVector(), ktt::DimensionVector(), {elementType, operatorTypeName});
    m_TypeToKernel[pair].second = m_Tuner.CreateSimpleKernel("Reduction", m_TypeToKernel[pair].first);
    const auto definitionId = m_TypeToKernel[pair].first;
    const auto kernelId = m_TypeToKernel[pair].second;

    m_InOffsetId = m_Tuner.AddArgumentScalar(0);
    m_OutOffsetId = m_Tuner.AddArgumentScalar(0);

    const ktt::DeviceInfo deviceInfo = m_Tuner.GetCurrentDeviceInfo();
    const size_t cus = deviceInfo.GetMaxComputeUnits();

    m_Tuner.AddParameter(kernelId, "WORK_GROUP_SIZE_X", std::vector<uint64_t>{32, 64, 96, 128, 160, 196, 224, 256, 288, 320, 352, 384, 416, 448, 480, 512, 576, 640, 704, 768, 832, 896, 960, 1024});
    m_Tuner.AddThreadModifier(kernelId, {definitionId}, ktt::ModifierType::Local, ktt::ModifierDimension::X, "WORK_GROUP_SIZE_X", ktt::ModifierAction::Multiply);
    m_Tuner.AddParameter(kernelId, "UNBOUNDED_WG", std::vector<uint64_t>{0, 1});
    m_Tuner.AddParameter(kernelId, "WG_NUM", std::vector<uint64_t>{0, cus, cus * 2, cus * 3, cus * 4, cus * 5, cus * 6, cus * 7, cus * 8, cus * 10, cus * 12, cus * 14, cus * 16, cus * 20, cus * 24, cus * 28, cus * 32, cus * 40, cus * 48, cus * 56, cus * 64});

    m_Tuner.AddParameter(kernelId, "VECTOR_SIZE", std::vector<uint64_t>{1/*, 2, 4*/});

    m_Tuner.AddThreadModifier(kernelId, {definitionId}, ktt::ModifierType::Global, ktt::ModifierDimension::X, "VECTOR_SIZE", ktt::ModifierAction::Divide);
    m_Tuner.AddParameter(kernelId, "USE_ATOMICS", std::vector<uint64_t>{0/*, 1*/});

    auto persistConstraint = [](const std::vector<uint64_t>& v) {return (v[0] && v[1] == 0) || (!v[0] && v[1] > 0); };
    m_Tuner.AddConstraint(kernelId, {"UNBOUNDED_WG", "WG_NUM"}, persistConstraint);
    auto persistentAtomic = [](const std::vector<uint64_t>& v) {return (v[0] == 1) || (v[0] == 0 && v[1] == 1); };
    m_Tuner.AddConstraint(kernelId, {"UNBOUNDED_WG", "USE_ATOMICS"}, persistentAtomic);
    auto unboundedWG = [](const std::vector<uint64_t>& v) {return (!v[0] || v[1] >= 32); };
    m_Tuner.AddConstraint(kernelId, {"UNBOUNDED_WG", "WORK_GROUP_SIZE_X"}, unboundedWG);

    m_Tuner.SetLauncher(kernelId, [this, definitionId](ktt::ComputeInterface& interface)
    {
        const ktt::DimensionVector& globalSize = interface.GetCurrentGlobalSize(definitionId);
        const ktt::DimensionVector& localSize = interface.GetCurrentLocalSize(definitionId);
        const std::vector<ktt::ParameterPair>& pairs = interface.GetCurrentConfiguration().GetPairs();
        ktt::DimensionVector myGlobalSize = globalSize;

        // change global size for constant numbers of work-groups
        if (ktt::ParameterPair::GetParameterValue<uint64_t>(pairs, "UNBOUNDED_WG") == 0)
        {
            myGlobalSize = ktt::DimensionVector(ktt::ParameterPair::GetParameterValue<uint64_t>(pairs, "WG_NUM")
                * localSize.GetSizeX());
        }

        interface.RunKernel(definitionId, myGlobalSize, localSize);

        // execute kernel log n times, when atomics are not used 
        if (ktt::ParameterPair::GetParameterValue<uint64_t>(pairs, "USE_ATOMICS") == 0)
        {
            uint32_t n = static_cast<uint32_t>(globalSize.GetSizeX() / localSize.GetSizeX());
            uint32_t inOffset = 0;
            uint32_t outOffset = n;
            uint32_t vectorSize = static_cast<uint32_t>(ktt::ParameterPair::GetParameterValue<uint64_t>(pairs, "VECTOR_SIZE"));
            uint32_t wgSize = static_cast<uint32_t>(localSize.GetSizeX());
            size_t iterations = 0; // make sure the end result is in the correct buffer

            while (n > 1 || iterations % 2 == 1)
            {
                interface.SwapArguments(definitionId, m_SrcId, m_DstId);
                myGlobalSize.SetSizeX((n + vectorSize - 1) / vectorSize);
                myGlobalSize.SetSizeX(((myGlobalSize.GetSizeX() - 1) / wgSize + 1) * wgSize);

                if (myGlobalSize == localSize)
                {
                    outOffset = 0; // only one WG will be executed
                }

                interface.UpdateScalarArgument(m_NId, &n);
                interface.UpdateScalarArgument(m_OutOffsetId, &outOffset);
                interface.UpdateScalarArgument(m_InOffsetId, &inOffset);

                interface.RunKernel(definitionId, myGlobalSize, localSize);
                n = (n + wgSize * vectorSize - 1) / (wgSize * vectorSize);
                inOffset = outOffset / vectorSize; // input is vectorized, output is scalar
                outOffset += n;
                ++iterations;
            }
        }
    });
}

std::vector<uint8_t> Reduction::Run(CUdeviceptr srcBuffer, const size_t elementCount, const size_t elementSize, const std::string& typeName,
    [[maybe_unused]] const void* init, const Operator& op)
{
    Initialize(typeName, op);
    const auto pair = std::make_pair(typeName, op.GetName());
    ClearArguments(pair);

    const size_t bufferSize = elementSize * elementCount;
    m_SrcId = m_Tuner.AddArgumentVector(reinterpret_cast<ktt::ComputeBuffer>(srcBuffer), bufferSize, elementSize,
        ktt::ArgumentAccessType::ReadWrite, ktt::ArgumentMemoryLocation::Device);

    /*dst.resize(elementCount, static_cast<T>(0));
    m_DstId = m_Tuner.AddArgumentVector<T>(static_cast<ktt::ComputeBuffer>(m_DstBuffer), bufferSize, ktt::ArgumentAccessType::ReadWrite, ktt::ArgumentMemoryLocation::Device);

    m_NId = m_Tuner.AddArgumentScalar(elementCount);
    UpdateArguments(pair);*/

    std::vector<uint8_t> result;
    ktt::BufferOutputDescriptor output(m_DstId, result.data(), elementSize);
    m_Tuner.TuneIteration(m_TypeToKernel[pair].second, {output});
    return result;
}

void Reduction::UpdateArguments(const std::pair<std::string, std::string>& typePair)
{
    const auto definition = m_TypeToKernel[typePair].first;
    m_Tuner.SetArguments(definition, {m_SrcId, m_DstId, m_NId, m_InOffsetId, m_OutOffsetId});
}

void Reduction::ClearArguments(const std::pair<std::string, std::string>& typePair)
{
    const auto definition = m_TypeToKernel[typePair].first;
    m_Tuner.SetArguments(definition, {});
}

} // namespace kttl
