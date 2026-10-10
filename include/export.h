#ifndef CFX_BASE_INCLUDE_EXPORT_H_
#define CFX_BASE_INCLUDE_EXPORT_H_

#if defined(_WIN32) || defined(__CYGWIN__)
#define CFX_PLATFORM_WINDOWS 1
#else
#define CFX_PLATFORM_WINDOWS 0
#endif

#if CFX_PLATFORM_WINDOWS
#ifdef CFX_BUILD_SHARED
#define CFLUXX_API __declspec(dllexport)
#else
#define CFLUXX_API __declspec(dllimport)
#endif
#define CFLUXX_LOCAL
#else
#if defined(CFX_BUILD_SHARED) && (defined(__GNUC__) || defined(__clang__))
#define CFLUXX_API __attribute__((visibility("default")))
#else
#define CFLUXX_API
#endif
#if defined(__GNUC__) || defined(__clang__)
#define CFLUXX_LOCAL __attribute__((visibility("hidden")))
#else
#define CFLUXX_CFLUXX_LOCAL
#endif
#endif

#endif  // !_CFX-BASE_INCLUDE_EXPORT_H_
