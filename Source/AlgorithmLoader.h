#pragma once

#include <Ktt.h>

namespace kttl
{

class AlgorithmLoader
{
public:
    explicit AlgorithmLoader(ktt::Tuner& tuner);

    void Load();

private:
    ktt::Tuner& m_Tuner;
};

} // namespace kttl
