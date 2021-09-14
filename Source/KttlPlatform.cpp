#include <KttlPlatform.h>

namespace ktt
{

static_assert(KTTL_VERSION_MAJOR < 1'000, "Invalid major version");
static_assert(KTTL_VERSION_MINOR < 1'000, "Invalid minor version");
static_assert(KTTL_VERSION_PATCH < 1'000, "Invalid patch version");

uint32_t GetKttlVersion()
{
    const uint32_t majorPart = KTTL_VERSION_MAJOR * 1'000'000;
    const uint32_t minorPart = KTTL_VERSION_MINOR * 1'000;
    const uint32_t patchPart = KTTL_VERSION_PATCH;

    return majorPart + minorPart + patchPart;
}

std::string GetKttlVersionString()
{
    return std::to_string(KTTL_VERSION_MAJOR) + "." + std::to_string(KTTL_VERSION_MINOR) + "." + std::to_string(KTTL_VERSION_PATCH);
}

} // namespace ktt
