#ifndef STOREPATHS_STOREPATHS_H_
#define STOREPATHS_STOREPATHS_H_

#include "storepaths/common.h"

// # Implementation for C++
#ifdef __cplusplus
#include <string>
#include <optional>

// ## Implementation for each platform
#define DECLARE_CPP_FUNCS(platformConfigFileName) \
    std::pair<std::string, PathInfo> getConfigFolder(const std::string& appName); \
    std::pair<std::string, PathInfo> getDataFolder(const std::string& appName); \
    std::pair<std::string, PathInfo> getCacheFolder(const std::string& appName); \
    std::pair<std::string, PathInfo> getJSONConfigFile(const std::string& appName, \
        const std::optional<std::string>& fileName = std::nullopt); \
    std::pair<std::string, PathInfo> platformConfigFileName(const std::string& appName, \
        const std::optional<std::string>& fileName = std::nullopt); \
    \
    std::pair<std::string, PathInfo> getFolder(Folders folderType, const std::string& appName); \
    std::pair<std::string, PathInfo> getFile(Files fileType, const std::string& appName, \
        const std::optional<std::string>& fileName = std::nullopt);

namespace storepaths {
#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_OS_OSX)
    namespace posix {
        DECLARE_CPP_FUNCS(getPosixConfigFile)
    } // linux
#endif
#if defined(STOREPATHS_OS_WINDOWS)
    namespace windows {
        DECLARE_CPP_FUNCS(getWindowsConfigFile)
    } // windows
#endif
#if defined(STOREPATHS_OS_OSX)
    namespace osx {
        DECLARE_CPP_FUNCS(getOsxConfigFile)
    } // osx
#endif
} // storepaths

#undef DECLARE_CPP_FUNCS

// ## Using namespaces for current platform
namespace storepaths {
#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_USING_POSIX_IMPL_FOR_OSX)
    using namespace posix;
#elif defined(STOREPATHS_OS_WINDOWS)
    using namespace windows;
#elif defined(STOREPATHS_OS_OSX) && !defined(STOREPATHS_USING_POSIX_IMPL_FOR_OSX)
    using namespace osx;
#endif
} // storepaths

#else // #ifdef __cplusplus

#include <stdbool.h>

// # Proxy for C of the above functions
// ## Implementation for each platform
#define DECLARE_C_FUNCS(configName, dataName, cacheName, confFileName, platformConfName) \
    PathInfo configName(char* outBuffer, size_t maxLength, const char* appName); \
    PathInfo dataName(char* outBuffer, size_t maxLength, const char* appName); \
    PathInfo cacheName(char* outBuffer, size_t maxLength, const char* appName); \
    PathInfo confFileName(char* outBuffer, size_t maxLength, const char* appName, const char* fileName); \
    PathInfo platformConfName(char* outBuffer, size_t maxLength, const char* appName, const char* fileName);

#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_OS_OSX)
    DECLARE_C_FUNCS(getPosixConfigFolder, getPosixDataFolder, getPosixCacheFolder, getPosixJSONConfigFile, getPosixConfigFile)
#endif
#if defined(STOREPATHS_OS_WINDOWS)
    DECLARE_C_FUNCS(getWindowsConfigFolder, getWindowsDataFolder, getWindowsCacheFolder, getWindowsJSONConfigFile, getWindowsConfigFile)
#endif
#if defined(STOREPATHS_OS_OSX)
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
    static inline PathInfo getJSONConfigFile(char* outBuffer, const size_t maxLength, \
            const char* appName, const char* fileName) { \
        return aliasConfFileName(outBuffer, maxLength, appName, fileName); \
    } \
    /* No alias for platform config file */

// Only one can be defined
#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_USING_POSIX_IMPL_FOR_OSX)
    DEFINE_C_ALIASES(getPosixConfigFolder, getPosixDataFolder, getPosixCacheFolder, getPosixJSONConfigFile)
#elif defined(STOREPATHS_OS_WINDOWS)
    DEFINE_C_ALIASES(getWindowsConfigFolder, getWindowsDataFolder, getWindowsCacheFolder, getWindowsJSONConfigFile)
#elif defined(STOREPATHS_OS_OSX) && !defined(STOREPATHS_USING_POSIX_IMPL_FOR_OSX)
    DEFINE_C_ALIASES(getOsxConfigFolder, getOsxDataFolder, getOsxCacheFolder, getOsxJSONConfigFile)
#endif

#undef DEFINE_C_ALIASES

#endif // #ifdef __cplusplus #else

#endif //STOREPATHS_STOREPATHS_H_
