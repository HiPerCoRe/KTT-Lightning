#pragma once

namespace thrust
{

template <typename InputIterator, typename T, typename BinaryFunction>
T reduce(InputIterator first, InputIterator last, T init, BinaryFunction binary_op);

template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryFunction>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryFunction op);

} // namespace thrust

#include <ThrustAdapters.inl>
