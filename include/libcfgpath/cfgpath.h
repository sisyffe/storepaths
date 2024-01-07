#ifndef LIBCFGPATH_CFGPATH_H_
#define LIBCFGPATH_CFGPATH_H_

#include "libcfgpath/common.h"

// # Implementation for C++
#ifdef __cplusplus
#include <string>

// ## Implementation for each platform
#define DECLARE_CPP_FUNCS \
    std::pair<std::string, bool> getConfigFolder(const std::string& appName); \
    std::pair<std::string, bool> getDataFolder(const std::string& appName); \
    std::pair<std::string, bool> getCacheFolder(const std::string& appName); \
    std::string getConfigFile(const std::string& appName); \
    \
    std::pair<std::string, bool> getFolder(Folders folderType, const std::string& appName); \
    std::string getFile(Files fileType, const std::string& appName);

namespace libcfgpath {
#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_OS_OSX)
    namespace linux {
        DECLARE_CPP_FUNCS
    } // linux
#endif
#if defined(LIBCFGPATH_OS_WINDOWS)
    namespace windows {
        DECLARE_CPP_FUNCS
    } // windows
#endif
#if defined(LIBCFGPATH_OS_OSX)
    namespace osx {
        DECLARE_CPP_FUNCS
    } // osx
#endif
} // libcfgpath

#undef DECLARE_CPP_FUNCS

// ## Using namespaces for current platform
namespace libcfgpath {
#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_USING_LINUX_IMPL_FOR_OSX)
    using namespace linux;
#elif defined(LIBCFGPATH_OS_WINDOWS)
    using namespace windows;
#elif defined(LIBCFGPATH_OS_OSX) && !defined(LIBCFGPATH_USING_LINUX_IMPL_FOR_OSX)
    using namespace osx;
#endif
} // libcfgpath

#else // #ifdef __cplusplus

#include <stdbool.h>

// # Proxy for C of the above functions
// ## Implementation for each platform
#define DECLARE_C_FUNCS(configName, dataName, cacheName, confFileName) \
    bool configName(char* outBuffer, size_t maxLength, const char* appName); \
    bool dataName(char* outBuffer, size_t maxLength, const char* appName); \
    bool cacheName(char* outBuffer, size_t maxLength, const char* appName); \
    void confFileName(char* outBuffer, size_t maxLength, const char* appName);

#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_OS_OSX)
    DECLARE_C_FUNCS(getLinuxConfigFolder, getLinuxDataFolder, getLinuxCacheFolder, getLinuxConfigFile)
#endif
#if defined(LIBCFGPATH_OS_WINDOWS)
    DECLARE_C_FUNCS(getWindowsConfigFolder, getWindowsDataFolder, getWindowsCacheFolder, getWindowsConfigFile)
#endif
#if defined(LIBCFGPATH_OS_OSX)
    DECLARE_C_FUNCS(getOsxConfigFolder, getOsxDataFolder, getOsxCacheFolder, getOsxConfigFile)
#endif

#undef DECLARE_C_FUNCS

// ## Make simple aliases for the current platform
#define DEFINE_C_ALIASES(aliasConfigName, aliasDataName, aliasCacheName, aliasConfFileName) \
    static inline bool getConfigFolder(char* outBuffer, const size_t maxLength, const char* appName) { \
        return aliasConfigName(outBuffer, maxLength, appName); \
    } \
    static inline bool getDataFolder(char* outBuffer, const size_t maxLength, const char* appName) { \
        return aliasDataName(outBuffer, maxLength, appName); \
    } \
    static inline bool getCacheFolder(char* outBuffer, const size_t maxLength, const char* appName) { \
        return aliasCacheName(outBuffer, maxLength, appName); \
    } \
    static inline void getConfigFile(char* outBuffer, const size_t maxLength, const char* appName) { \
        aliasConfFileName(outBuffer, maxLength, appName); \
    }

#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_USING_LINUX_IMPL_FOR_OSX)
    DEFINE_C_ALIASES(getLinuxConfigFolder, getLinuxDataFolder, getLinuxCacheFolder, getLinuxConfigFile)
#elif defined(LIBCFGPATH_OS_WINDOWS)
    DEFINE_C_ALIASES(getWindowsConfigFolder, getWindowsDataFolder, getWindowsCacheFolder, getWindowsConfigFile)
#elif defined(LIBCFGPATH_OS_OSX) && !defined(LIBCFGPATH_USING_LINUX_IMPL_FOR_OSX)
    DEFINE_C_ALIASES(getOsxConfigFolder, getOsxDataFolder, getOsxCacheFolder, getOsxConfigFile)
#endif

#undef DEFINE_C_ALIASES

#endif // #ifdef __cplusplus #else

#endif //LIBCFGPATH_CFGPATH_H_
