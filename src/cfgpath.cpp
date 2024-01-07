#include "libcfgpath/cfgpath.h"

// # Define C++ common funcs
#define DEFINE_GET_FILE_FOLDER_FUNCS \
    std::pair<std::string, bool> getFolder(const Folders folderType, const std::string& appName) { \
        switch (folderType) { \
            case CONFIG_FOLDER: \
                return getConfigFolder(appName); \
            case DATA_FOLDER: \
                return getDataFolder(appName); \
            case CACHE_FOLDER: \
                return getCacheFolder(appName); \
            default: \
                return { "", false }; \
        } \
    } \
    \
    std::string getFile(const Files fileType, const std::string &appName) { \
        switch (fileType) { \
            case CONFIG_FILE: \
                return getConfigFile(appName); \
            default: \
                return ""; \
        } \
    }

namespace libcfgpath {
#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_OS_OSX)
    namespace linux {
        DEFINE_GET_FILE_FOLDER_FUNCS
    } // linux
#endif
#if defined(LIBCFGPATH_OS_WINDOWS)
    namespace windows {
        DEFINE_GET_FILE_FOLDER_FUNCS
    } // windows
#endif
#if defined(LIBCFGPATH_OS_OSX)
    namespace osx {
        DEFINE_GET_FILE_FOLDER_FUNCS
    } // osx
#endif
} // libcfgpath

#undef DEFINE_GET_FILE_FOLDER_FUNCS

// # Proxy for C
LIBCFGPATH_C_LINKAGE()

#define CPP_FOLDER_FUNC_WRAPPER(funcName, callbackFunc) \
    bool funcName(char* outBuffer, const size_t maxLength, const char* appName) { \
        auto [string, succeeded] = callbackFunc(appName); \
        if (string.size() + 1 > maxLength || string.empty()) { \
            outBuffer[0] = '\0'; \
            return false; \
        } \
        std::strncpy(outBuffer, string.c_str(), string.size() + 1); \
        return succeeded; \
    }

#define CPP_FILE_FUNC_WRAPPER(funcName, callbackFunc) \
    void funcName(char* outBuffer, const size_t maxLength, const char* appName) { \
        auto result = callbackFunc(appName); \
        if (result.size() + 1 > maxLength || result.empty()) { \
            outBuffer[0] = '\0'; \
            return; \
        } \
        std::strncpy(outBuffer, result.c_str(), result.size() + 1); \
    }
#define DEFINE_C_WRAPPERS(nameSpace, aliasConfigName, aliasDataName, aliasCacheName, aliasConfFileName) \
    CPP_FOLDER_FUNC_WRAPPER(aliasConfigName, libcfgpath::nameSpace::getConfigFolder) \
    CPP_FOLDER_FUNC_WRAPPER(aliasDataName, libcfgpath::nameSpace::getDataFolder) \
    CPP_FOLDER_FUNC_WRAPPER(aliasCacheName, libcfgpath::nameSpace::getCacheFolder) \
    CPP_FILE_FUNC_WRAPPER(aliasConfFileName, libcfgpath::nameSpace::getConfigFile)

#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_OS_OSX)
    DEFINE_C_WRAPPERS(posix, getPosixConfigFolder, getPosixDataFolder, getPosixCacheFolder, getPosixConfigFile)
#endif
#if defined(LIBCFGPATH_OS_WINDOWS)
    DEFINE_C_WRAPPERS(windows, getWindowsConfigFolder, getWindowsDataFolder, getWindowsCacheFolder, getWindowsConfigFile)
#endif
#if defined(LIBCFGPATH_OS_OSX)
    DEFINE_C_WRAPPERS(osx, getOsxConfigFolder, getOsxDataFolder, getOsxCacheFolder, getOsxConfigFile)
#endif

#undef DEFINE_C_ALIASES
#undef CPP_FOLDER_FUNC_WRAPPER
#undef CPP_FILE_FUNC_WRAPPER

LIBCFGPATH_C_LINKAGE_END()
