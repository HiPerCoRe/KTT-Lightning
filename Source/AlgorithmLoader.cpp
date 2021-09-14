#include <AlgorithmLoader.h>
#include <Utility/FileSystem.h>

namespace kttl
{

AlgorithmLoader::AlgorithmLoader(ktt::Tuner& tuner) :
    m_Tuner(tuner)
{}

void AlgorithmLoader::Load()
{
    const std::string reductionCode = LoadFileToString("../Source/Kernels/Reduction.cu");
}

} // namespace kttl
