/** @file ComputationResult.h
  * Result data from a single kernel definition run.
  */
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <Api/Configuration/DimensionVector.h>
#include <Api/Output/KernelCompilationData.h>
#include <Api/Output/KernelProfilingData.h>
#include <KttPlatform.h>
#include <KttTypes.h>

namespace ktt
{

/** @class ComputationResult
  * Class which holds result data from a single kernel definition run such as duration, overhead and profiling data.
  */
class KTT_API ComputationResult
{
public:
    /** @fn ComputationResult()
      * Constructor which creates empty invalid computation result.
      */
    ComputationResult();

    /** @fn explicit ComputationResult(const std::string& kernelFunction)
      * Constructor which creates computation result for the specified kernel function.
      * @param kernelFunction Name of kernel function tied to the result.
      */
    explicit ComputationResult(const std::string& kernelFunction);

    /** @fn ComputationResult(const ComputationResult& other)
      * Copy constructor.
      * @param other Result from which the data will be copied.
      */
    ComputationResult(const ComputationResult& other);

    /** @fn void SetDurationData(const Nanoseconds duration, const Nanoseconds overhead)
      * Fills duration data for the result.
      * @param duration Raw kernel duration, usually reported by the underlying compute API.
      * @param overhead Overhead related to kernel launch such as kernel function compilation.
      */
    void SetDurationData(const Nanoseconds duration, const Nanoseconds overhead);

    /** @fn void SetSizeData(const DimensionVector& globalSize, const DimensionVector& localSize)
      * Fills thread size data for the result.
      * @param globalSize Global thread size with which the kernel was launched.
      * @param localSize Local thread size with which the kernel was launched.
      */
    void SetSizeData(const DimensionVector& globalSize, const DimensionVector& localSize);

    /** @fn void SetCompilationData(std::unique_ptr<KernelCompilationData> data)
      * Fills compilation data for the result.
      * @param data Compilation data generated by the compute API. See KernelCompilationData for more information.
      */
    void SetCompilationData(std::unique_ptr<KernelCompilationData> data);

    /** @fn void SetProfilingData(std::unique_ptr<KernelProfilingData> data)
      * Fills profiling data for the result.
      * @param data Profiling data generated by the compute API. See KernelProfilingData for more information.
      */
    void SetProfilingData(std::unique_ptr<KernelProfilingData> data);

    /** @fn const std::string& GetKernelFunction() const
      * Returns name of a kernel function tied to the result.
      * @return Name of a kernel function tied to the result.
      */
    const std::string& GetKernelFunction() const;

    /** @fn const DimensionVector& GetGlobalSize() const
      * Returns global thread size with which the kernel was launched.
      * @return Global thread size with which the kernel was launched.
      */
    const DimensionVector& GetGlobalSize() const;

    /** @fn const DimensionVector& GetLocalSize() const
      * Returns local thread size with which the kernel was launched.
      * @return Local thread size with which the kernel was launched.
      */
    const DimensionVector& GetLocalSize() const;

    /** @fn Nanoseconds GetDuration() const
      * Returns raw kernel duration, usually reported by the underlying compute API.
      * @return Kernel duration.
      */
    Nanoseconds GetDuration() const;

    /** @fn Nanoseconds GetOverhead() const
      * Returns overhead related to kernel launch such as kernel function compilation.
      * @return Kernel overhead.
      */
    Nanoseconds GetOverhead() const;

    /** @fn bool HasCompilationData() const
      * Checks whether result contains valid compilation data.
      * @return True if compilation data is valid. False otherwise.
      */
    bool HasCompilationData() const;

    /** @fn const KernelCompilationData& GetCompilationData() const
      * Retrieves kernel compilation data. Should only be called after prior check for valid data.
      * @return Compilation data generated by the compute API. See KernelCompilationData for more information.
      */
    const KernelCompilationData& GetCompilationData() const;

    /** @fn bool HasProfilingData() const
      * Checks whether result contains valid profiling data.
      * @return True if profiling data is valid. False otherwise.
      */
    bool HasProfilingData() const;

    /** @fn const KernelProfilingData& GetProfilingData() const
      * Retrieves kernel profiling data. Should only be called after prior check for valid data.
      * @return Profiling data generated by the compute API. See KernelProfilingData for more information.
      */
    const KernelProfilingData& GetProfilingData() const;

    /** @fn bool HasRemainingProfilingRuns() const
      * Checks whether more kernel runs under the corresponding configuration need to be performed before profiling data contains
      * valid information.
      * @return True if more kernel runs need to be performed under the same configuration. False otherwise.
      */
    bool HasRemainingProfilingRuns() const;

    /** @fn ComputationResult& operator=(const ComputationResult& other)
      * Copy assignment operator.
      * @param other Result from which the data will be copied.
      */
    ComputationResult& operator=(const ComputationResult& other);

private:
    std::string m_KernelFunction;
    DimensionVector m_GlobalSize;
    DimensionVector m_LocalSize;
    Nanoseconds m_Duration;
    Nanoseconds m_Overhead;
    std::unique_ptr<KernelCompilationData> m_CompilationData;
    std::unique_ptr<KernelProfilingData> m_ProfilingData;
};

} // namespace ktt