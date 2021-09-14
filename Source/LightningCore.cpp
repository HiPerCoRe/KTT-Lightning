#include <LightningCore.h>

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

} // namespace kttl
