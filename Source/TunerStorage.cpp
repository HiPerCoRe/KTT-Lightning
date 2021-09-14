#include <TunerStorage.h>

namespace kttl
{

TunerStorage::TunerStorage(const uint32_t deviceIndex) :
    m_Tuner(std::make_unique<ktt::Tuner>(0, deviceIndex, ktt::ComputeApi::CUDA))
{}

ktt::Tuner& TunerStorage::GetTuner()
{
    return *m_Tuner;
}

} // namespace kttl
