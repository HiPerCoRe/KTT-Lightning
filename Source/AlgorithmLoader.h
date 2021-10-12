#pragma once

#include <memory>
#include <Ktt.h>

#include <Algorithms/Reduction.h>
#include <Algorithms/Transform.h>

namespace kttl
{

class AlgorithmLoader
{
public:
    explicit AlgorithmLoader(ktt::Tuner& tuner);
    ~AlgorithmLoader();

    void Load();

    Reduction& GetReduction();
    Transform& GetTransform();

private:
    ktt::Tuner& m_Tuner;
    std::unique_ptr<Reduction> m_Reduction;
    std::unique_ptr<Transform> m_Transform;
};

} // namespace kttl
