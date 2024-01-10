#ifndef LIBCFGPATH_CFGPATH_H_
#define LIBCFGPATH_CFGPATH_H_

#include "libcfgpath/common.h"

// # Implementation for C++
#ifdef __cplusplus
#include <string>

// ## Implementation for each platform
#define DECLARE_CPP_FUNCS(platformConfigFileName, platformConfigFileArgument) \
    std::pair<std::string, PathInfo> getConfigFolder(const std::string& appName); \
    std::pair<std::string, PathInfo> getDataFolder(const std::string& appName); \
    std::pair<std::string, PathInfo> getCacheFolder(const std::string& appName); \
    std::pair<std::string, PathInfo> getJSONConfigFile(const std::string& appName); \
    std::pair<std::string, PathInfo> platformConfigFileName(platformConfigFileArgument); \
    \
    std::pair<std::string, PathInfo> getFolder(Folders folderType, const std::string& appName); \
    std::pair<std::string, PathInfo> getFile(Files fileType, const std::string& appName);

namespace libcfgpath {
#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_OS_OSX)
    namespace posix {
        DECLARE_CPP_FUNCS(getPosixConfigFile, const std::string& appName)
    } // linux
#endif
#if defined(LIBCFGPATH_OS_WINDOWS)
    namespace windows {
        DECLARE_CPP_FUNCS(getWindowsConfigFile, const std::string& appName)
    } // windows
#endif
#if defined(LIBCFGPATH_OS_OSX)
    namespace osx {
        DECLARE_CPP_FUNCS(getOsxConfigFile, const std::string& bundleIdentifier)
    } // osx
#endif
} // libcfgpath

#undef DECLARE_CPP_FUNCS

// ## Using namespaces for current platform
namespace libcfgpath {
#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_USING_POSIX_IMPL_FOR_OSX)
    using namespace posix;
#elif defined(LIBCFGPATH_OS_WINDOWS)
    using namespace windows;
#elif defined(LIBCFGPATH_OS_OSX) && !defined(LIBCFGPATH_USING_POSIX_IMPL_FOR_OSX)
    using namespace osx;
#endif
} // libcfgpath

#else // #ifdef __cplusplus

#include <stdbool.h>

// # Proxy for C of the above functions
// ## Implementation for each platform
#define DECLARE_C_FUNCS(configName, dataName, cacheName, confFileName, platformConfName) \
    PathInfo configName(char* outBuffer, size_t maxLength, const char* appName); \
    PathInfo dataName(char* outBuffer, size_t maxLength, const char* appName); \
    PathInfo cacheName(char* outBuffer, size_t maxLength, const char* appName); \
    PathInfo confFileName(char* outBuffer, size_t maxLength, const char* appName); \
    PathInfo platformConfName(char* outBuffer, size_t maxLength, const char* appName);

#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_OS_OSX)
    DECLARE_C_FUNCS(getPosixConfigFolder, getPosixDataFolder, getPosixCacheFolder, getPosixJSONConfigFile, getPosixConfigFile)
#endif
#if defined(LIBCFGPATH_OS_WINDOWS)
    DECLARE_C_FUNCS(getWindowsConfigFolder, getWindowsDataFolder, getWindowsCacheFolder, getWindowsJSONConfigFile, getWindowsConfigFile)
#endif
#if defined(LIBCFGPATH_OS_OSX)
    DECLARE_C_FUNCS(getOsxConfigFolder, getOsxDataFolder, getOsxCacheFolder, getOsxJSONConfigFile, getOsxConfigFile)
#endif

#undef DECLARE_C_FUNCS

// ## Make simple aliases for the current platform
#define DEFINE_C_ALIASES(aliasConfigName, aliasDataName, aliasCacheName, aliasConfFileName) \
    static inline PathInfo getConfigFolder(char* outBuffer, const size_t maxLength, const char* appName) { \
        return aliasConfigName(outBuffer, maxLength, appName); \
    } \
    static inline PathInfo getDataFolder(char* outBuffer, const size_t maxLength, const char* appName) { \
        return aliasDataName(outBuffer, maxLength, appName); \
    } \
    static inline PathInfo getCacheFolder(char* outBuffer, const size_t maxLength, const char* appName) { \
        return aliasCacheName(outBuffer, maxLength, appName); \
    } \
    static inline PathInfo getJSONConfigFile(char* outBuffer, const size_t maxLength, const char* appName) { \
        return aliasConfFileName(outBuffer, maxLength, appName); \
    }

// Only one can be defined
#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_USING_POSIX_IMPL_FOR_OSX)
    DEFINE_C_ALIASES(getPosixConfigFolder, getPosixDataFolder, getPosixCacheFolder, getPosixJSONConfigFile)
#elif defined(LIBCFGPATH_OS_WINDOWS)
    DEFINE_C_ALIASES(getWindowsConfigFolder, getWindowsDataFolder, getWindowsCacheFolder, getWindowsJSONConfigFile)
#elif defined(LIBCFGPATH_OS_OSX) && !defined(LIBCFGPATH_USING_POSIX_IMPL_FOR_OSX)
    DEFINE_C_ALIASES(getOsxConfigFolder, getOsxDataFolder, getOsxCacheFolder, getOsxJSONConfigFile)
#endif

#undef DEFINE_C_ALIASES

#endif // #ifdef __cplusplus #else

#endif //LIBCFGPATH_CFGPATH_H_
