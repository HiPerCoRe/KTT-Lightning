template <typename T, typename Operator>
__global__ void transform(const T* input1, const T* input2, T* out, const size_t numItems)
{
    Operator op;

    #pragma unroll UNROLL_FACTOR
    for (int element = 0; element < ELEMENTS_PER_THREAD; ++element)
    {
        #ifdef MEMORY_MAPPING_PATTERN == 0
        const size_t index = ELEMENTS_PER_THREAD * (blockIdx.x * blockDim.x + threadIdx.x);
        const size_t offsetIndex = index + element;
        #elif MEMORY_MAPPING_PATTERN == 1
        const size_t index = blockIdx.x * blockDim.x + threadIdx.x;
        const size_t offsetIndex = index + ELEMENTS_PER_THREAD * blockDim.x * element;
        #endif

        if (offsetIndex < numItems)
        {
            out[offsetIndex] = op(input1[offsetIndex], input2[offsetIndex]);
        }
    }
};
