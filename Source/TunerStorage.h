#pragma once

#include <memory>

#include <Ktt.h>

namespace kttl
{

class TunerStorage
{
public:
    explicit TunerStorage(const uint32_t deviceIndex);

    ktt::Tuner& GetTuner();

private:
    std::unique_ptr<ktt::Tuner> m_Tuner;
};

} // namespace kttl
