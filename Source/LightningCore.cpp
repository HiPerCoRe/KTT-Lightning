#include <AlgorithmLoader.h>
#include <LightningCore.h>
#include <TunerStorage.h>

namespace kttl
{

LightningCore::LightningCore(const uint32_t deviceIndex) :
    m_Storage(std::make_unique<TunerStorage>(deviceIndex)),
    m_Loader(std::make_unique<AlgorithmLoader>(m_Storage->GetTuner()))
{}

LightningCore::~LightningCore() = default;

void LightningCore::Initialize()
{
    m_Loader->Load();
}

std::vector<uint8_t> LightningCore::Reduce(void* buffer, const size_t elementCount, const size_t elementSize,
    const std::string& typeName, const void* init, const Operator& op)
{
    auto& reduction = m_Loader->GetReduction();
    return reduction.Run(reinterpret_cast<CUdeviceptr>(buffer), elementCount, elementSize, typeName, init, op);
}

std::vector<uint8_t> LightningCore::Transform(void* srcBuffer1, void* srcBuffer2, void* dstBuffer, const size_t elementCount,
    const size_t elementSize, const std::string& typeName, const Operator& op)
{
    auto& transform = m_Loader->GetTransform();
    return transform.Run(reinterpret_cast<CUdeviceptr>(srcBuffer1), reinterpret_cast<CUdeviceptr>(srcBuffer2),
        reinterpret_cast<CUdeviceptr>(dstBuffer), elementCount, elementSize, typeName, op);
}

} // namespace kttl
