#pragma once

#include <memory>
#include <Ktt.h>

#include <Algorithms/Reduction.h>

namespace kttl
{

class AlgorithmLoader
{
public:
    explicit AlgorithmLoader(ktt::Tuner& tuner);
    ~AlgorithmLoader();

    void Load();

    Reduction& GetReduction();

private:
    ktt::Tuner& m_Tuner;
    std::unique_ptr<Reduction> m_Reduction;
};

} // namespace kttl
