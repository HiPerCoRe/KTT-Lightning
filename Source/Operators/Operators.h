#pragma once

#include <string>

#include <Operators/Operator.h>

namespace kttl
{

#define KTTL_OPERATOR(name, function) \
class name : public Operator \
{ \
public:\
function \
\
std::string GetName() const override \
{ \
    return #name; \
} \
std::string GetCode() const override \
{ \
    return "struct " ## #name ## "\n{\n __device__ " ## #function ## "\n};"; \
} \
bool IsTemplated() const override \
{ \
    return false; \
} \
};

#define KTTL_OPERATOR_TEMPLATED(name, typeName, function) \
template <typename typeName> \
class name : public Operator \
{ \
public: \
function \
\
std::string GetName() const override \
{ \
    return #name; \
} \
std::string GetCode() const override \
{ \
    return "template <typename " ## #typeName ## ">\nstruct " ## #name ## "\n{\n __device__ " ## #function ## "\n};"; \
} \
bool IsTemplated() const override \
{ \
    return true; \
} \
};

KTTL_OPERATOR_TEMPLATED(Plus, T, T operator()(const T& lhs, const T& rhs)
{
    return lhs + rhs;
})

} // namespace kttl

#include <Operators/Operators.inl>
