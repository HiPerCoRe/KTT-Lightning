#include <iterator>

#include <LightningApi.h>
#include <ThrustAdapters.h>

namespace thrust
{

template <typename InputIterator, typename T, typename BinaryFunction>
T reduce(InputIterator first, InputIterator last, T init, BinaryFunction binary_op)
{
    const auto numItems = std::distance(first, last);
    return kttl::Reduce(first, numItems, init, binary_op);
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryFunction>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryFunction op)
{
    const auto numItems = std::distance(first1, last1);
    return kttl::Transform(first1, first2, result, numItems, op);
}

} // namespace thrust
