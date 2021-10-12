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

template <typename T, typename InputBuffer1, typename InputBuffer2, typename OutputBuffer, typename BinaryOperator>
OutputBuffer Transform(InputBuffer1 input1, InputBuffer2 input2, OutputBuffer output, const size_t elementCount, BinaryOperator binaryOperator)
{
    const size_t elementSize = sizeof(T);
    const auto& elementType = typeid(T);
    auto result = Core().Transform(reinterpret_cast<void*>(input1), reinterpret_cast<void*>(input2), reinterpret_cast<void*>(output),
        elementCount, elementSize, elementType.name(), binaryOperator);

    return 0;
    //return result;
}

} // namespace kttl
