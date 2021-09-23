#pragma once

#include <string>

namespace kttl
{

class Operator
{
public:
    virtual ~Operator() = default;

    virtual std::string GetName() const = 0;
    virtual std::string GetCode() const = 0;
    virtual bool IsTemplated() const = 0;
};

} // namespace kttl
