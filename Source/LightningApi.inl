#include <algorithm>

#include <LightningApi.h>
#include <LightningCore.h>

namespace kttl
{

template <typename InputBuffer, typename T, typename BinaryOperator>
T Reduce(InputBuffer buffer, const size_t elementCount, T init, BinaryOperator binaryOperator)
{
    const size_t elementSize = sizeof(T);
    const auto& elementType = typeid(T);
    auto result = Core().Reduce(reinterpret_cast<void*>(buffer), elementCount, elementSize, elementType.name(), &init, binaryOperator);

    if (result.empty())
    {
        return static_cast<T>(0);
    }

    return *reinterpret_cast<T*>(result.data());
}

} // namespace kttl
