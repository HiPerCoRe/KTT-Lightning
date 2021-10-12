#include <AlgorithmLoader.h>

namespace kttl
{

AlgorithmLoader::AlgorithmLoader(ktt::Tuner& tuner) :
    m_Tuner(tuner)
{}

AlgorithmLoader::~AlgorithmLoader() = default;

void AlgorithmLoader::Load()
{
    m_Reduction = std::make_unique<Reduction>(m_Tuner);
    m_Transform = std::make_unique<Transform>(m_Tuner);
}

Reduction& AlgorithmLoader::GetReduction()
{
    return *m_Reduction;
}

Transform& AlgorithmLoader::GetTransform()
{
    return *m_Transform;
}

} // namespace kttl
