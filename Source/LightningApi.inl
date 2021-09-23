#include <algorithm>

#include <LightningApi.h>
#include <LightningCore.h>

namespace kttl
{

template <typename InputIterator, typename T, typename BinaryOperator>
T Reduce(InputIterator first, InputIterator last, T init, BinaryOperator binaryOperator)
{
    const size_t elementCount = static_cast<size_t>(std::distance(first, last));
    const auto result = Core().Reduce(&(*first), elementCount, init, binaryOperator);
    return result.convert<T>();
}

} // namespace kttl
