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

template <typename InputBuffer, typename T, typename BinaryOperator>
T Reduce(InputBuffer buffer, const size_t elementCount, T init, BinaryOperator binaryOperator);

template <typename T, typename InputBuffer1, typename InputBuffer2, typename OutputBuffer, typename BinaryOperator>
OutputBuffer Transform(InputBuffer1 input1, InputBuffer2 input2, OutputBuffer output, const size_t elementCount, BinaryOperator binaryOperator);

class LightningCore;
KTTL_API LightningCore& Core();

} // namespace kttl

#include <LightningApi.inl>
