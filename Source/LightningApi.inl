#include <algorithm>

#include <LightningApi.h>

namespace kttl
{

template <typename InputIterator, typename T, typename BinaryOperator>
T Reduce(InputIterator first, InputIterator last, T init, BinaryOperator binaryOperator)
{
    const size_t elementCount = static_cast<size_t>(std::distance(first, last));
    return ReduceN(first, elementCount, init, binaryOperator);
}

} // namespace kttl
