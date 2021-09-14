#include <Api/KttException.h>
#include <LightningApi.h>
#include <LightningCore.h>

namespace kttl
{

static std::unique_ptr<LightningCore> core = nullptr;

void Initialize(const uint32_t deviceIndex)
{
    core = std::make_unique<LightningCore>(deviceIndex);
    core->Initialize();
}

void Terminate()
{
    core.reset();
}

LightningCore& Core()
{
    if (core == nullptr)
    {
        throw ktt::KttException("KTT Lightning must be initialized through Initialize(deviceIndex) function");
    }

    return *core;
}

} // namespace kttl
