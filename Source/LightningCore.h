#pragma once

#include <memory>

#include <AlgorithmLoader.h>
#include <TunerStorage.h>

namespace kttl
{

class LightningCore
{
public:
    LightningCore(const uint32_t deviceIndex);
    ~LightningCore();

    void Initialize();

private:
    std::unique_ptr<TunerStorage> m_Storage;
    std::unique_ptr<AlgorithmLoader> m_Loader;
};

} // namespace kttl
