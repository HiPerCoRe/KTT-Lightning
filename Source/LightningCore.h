#pragma once

#include <cstdint>
#include <memory>
#include <rttr/registration>

#include <Operators/Operator.h>
#include <KttlPlatform.h>

namespace kttl
{

class AlgorithmLoader;
class TunerStorage;

class KTTL_API LightningCore
{
public:
    LightningCore(const uint32_t deviceIndex);
    ~LightningCore();

    void Initialize();

    rttr::variant Reduce(void* buffer, const size_t elementCount, const rttr::variant& init, const Operator& op);

private:
    std::unique_ptr<TunerStorage> m_Storage;
    std::unique_ptr<AlgorithmLoader> m_Loader;
};

} // namespace kttl
