/** @file LightningApi.h
  * Main part of public API of KTT Lightning.
  */
#pragma once

#include <cstdint>
#include <memory>

#include <KttlPlatform.h>
#include <Operators/Operators.h>

/** @namespace kttl
  * All classes, methods and type aliases related to KTT Lightning are located inside kttl namespace.
  */
namespace kttl
{

KTTL_API void Initialize(const uint32_t deviceIndex);
KTTL_API void Terminate();

template <typename InputIterator, typename T, typename BinaryOperator>
T Reduce(InputIterator first, InputIterator last, T init, BinaryOperator binaryOperator);

class LightningCore;
KTTL_API LightningCore& Core();

} // namespace kttl

#include <LightningApi.inl>
