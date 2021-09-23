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
}

Reduction& AlgorithmLoader::GetReduction()
{
    return *m_Reduction;
}

} // namespace kttl
