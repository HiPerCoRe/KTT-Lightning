template <typename T, typename Operator>
__global__ void transform(const T* input1, const T* input2, T* out, const size_t numItems)
{
    Operator op;

    #pragma unroll UNROLL_FACTOR
    for (int element = 0; element < ELEMENTS_PER_THREAD; ++element)
    {
        const size_t index = ELEMENTS_PER_THREAD * blockIdx.x * blockDim.x + ELEMENTS_PER_THREAD * threadIdx.x;
        const size_t offsetIndex = index + element;

        if (offsetIndex < numItems)
        {
            out[offsetIndex] = op(input1[offsetIndex], input2[offsetIndex]);
        }
    }
};
