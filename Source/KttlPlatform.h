/** @file KttlPlatform.h
  * Preprocessor definitions which ensure compatibility for multiple compilers and KTT Lightning version definitions.
  */
#pragma once

#include <cstdint>
#include <string>

#ifndef KTTL_API
#if defined(_MSC_VER)
    #pragma warning(disable : 4251) // Irrelevant MSVC warning as long as exported classes have no public attributes.
    
    #if defined(KTTL_LIBRARY)
        #define KTTL_API __declspec(dllexport)
    #else
        #define KTTL_API __declspec(dllimport)
    #endif // KTTL_LIBRARY
#else
    #define KTT_API
#endif // _MSC_VER
#endif // KTTL_API

/** Major version of KTT framework. First number in KTT Lightning version description.
  */
#define KTTL_VERSION_MAJOR 0

/** Minor version of KTT framework. Second number in KTT Lightning version description.
  */
#define KTTL_VERSION_MINOR 1

/** Patch version of KTT framework. Third number in KTT Lightning version description.
  */
#define KTTL_VERSION_PATCH 0

namespace kttl
{

/** @fn uint32_t GetKttlVersion()
  * Returns the current KTT Lightning version in integer format.
  * @return KTT Lightning version as integer xxxyyyzzz.
  */
KTTL_API uint32_t GetKttlVersion();

/** @fn std::string GetKttlVersionString()
  * Returns the current KTT Lightning version in string format.
  * @return KTT Lightning version as string x.y.z.
  */
KTTL_API std::string GetKttlVersionString();

} // namespace kttl
