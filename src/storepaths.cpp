#include "storepaths/storepaths.h"

#include <cstring>

// # Define C++ common funcs
#define DEFINE_GET_FILE_FOLDER_FUNCS(platformConfigFileName) \
    std::pair<std::string, PathInfo> getFolder(const Folders folderType, const std::string& appName) { \
        switch (folderType) { \
        case CONFIG_FOLDER: \
            return getConfigFolder(appName); \
        case DATA_FOLDER: \
            return getDataFolder(appName); \
        case CACHE_FOLDER: \
            return getCacheFolder(appName); \
        default: \
            return { "", { false, 0, true, false } }; \
        } \
    } \
    \
    std::pair<std::string, PathInfo> getFile(const Files fileType, const std::string &appName, \
            const std::optional<std::string>& fileName) { \
        switch (fileType) { \
        case JSON_CONFIG_FILE: \
            return getJSONConfigFile(appName, fileName); \
        case PLATFORM_CONFIG_FILE: \
            return platformConfigFileName(appName, fileName); \
        default: \
            return { "", { false, 0, true, false } }; \
        } \
    }

namespace storepaths {
#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_OS_OSX)
    namespace posix {
        DEFINE_GET_FILE_FOLDER_FUNCS(getPosixConfigFile)
    } // linux
#endif
#if defined(STOREPATHS_OS_WINDOWS)
    namespace windows {
        DEFINE_GET_FILE_FOLDER_FUNCS(getWindowsConfigFile)
    } // windows
#endif
#if defined(STOREPATHS_OS_OSX)
    namespace osx {
        DEFINE_GET_FILE_FOLDER_FUNCS(getOsxConfigFile)
    } // osx
#endif
} // storepaths

#undef DEFINE_GET_FILE_FOLDER_FUNCS

// # Proxy for C
STOREPATHS_C_LINKAGE()

#define CPP_FOLDER_FUNC_WRAPPER(funcName, callbackFunc) \
    PathInfo funcName(char* outBuffer, const size_t maxLength, const char* appName) { \
        auto [string, info] = callbackFunc(appName); \
        if (!info.found) { \
            outBuffer[0] = '\0'; \
            return { info.found, info.mkdirReturnCode, false, false }; \
        } else if (string.size() + 1 > maxLength) { \
            outBuffer[0] = '\0'; \
            return { info.found, info.mkdirReturnCode, true, false }; \
        } \
        std::strncpy(outBuffer, string.c_str(), string.size() + 1); \
        return info; \
    }

// The differrence is that file functions get one more parameter: fileName
#define CPP_FILE_FUNC_WRAPPER(funcName, callbackFunc) \
    PathInfo funcName(char* outBuffer, const size_t maxLength, const char* appName, const char* fileName) { \
        auto [string, info] = callbackFunc(appName, fileName ? std::optional{ fileName } : std::nullopt); \
        if (!info.found) { \
            outBuffer[0] = '\0'; \
            return { info.found, info.mkdirReturnCode, false, false }; \
        } else if (string.size() + 1 > maxLength) { \
            outBuffer[0] = '\0'; \
            return { info.found, info.mkdirReturnCode, true, false }; \
        } \
        std::strncpy(outBuffer, string.c_str(), string.size() + 1); \
        return info; \
    }

#define DEFINE_C_WRAPPERS(nameSpace, aliasConfigName, aliasDataName, aliasCacheName, aliasJSONConfFileName, aliasConfFileName) \
    CPP_FOLDER_FUNC_WRAPPER(aliasConfigName, storepaths::nameSpace::getConfigFolder) \
    CPP_FOLDER_FUNC_WRAPPER(aliasDataName, storepaths::nameSpace::getDataFolder) \
    CPP_FOLDER_FUNC_WRAPPER(aliasCacheName, storepaths::nameSpace::getCacheFolder) \
    CPP_FILE_FUNC_WRAPPER(aliasJSONConfFileName, storepaths::nameSpace::getJSONConfigFile) \
    CPP_FILE_FUNC_WRAPPER(aliasConfFileName, storepaths::nameSpace::aliasConfFileName)

#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_OS_OSX)
    DEFINE_C_WRAPPERS(posix, getPosixConfigFolder, getPosixDataFolder, getPosixCacheFolder, getPosixJSONConfigFile, getPosixConfigFile)
#endif
#if defined(STOREPATHS_OS_WINDOWS)
    DEFINE_C_WRAPPERS(windows, getWindowsConfigFolder, getWindowsDataFolder, getWindowsCacheFolder, getWindowsJSONConfigFile, getWindowsConfigFile)
#endif
#if defined(STOREPATHS_OS_OSX)
    DEFINE_C_WRAPPERS(osx, getOsxConfigFolder, getOsxDataFolder, getOsxCacheFolder, getOsxJSONConfigFile, getOsxConfigFile)
#endif

#undef DEFINE_C_ALIASES
#undef CPP_FOLDER_FUNC_WRAPPER
#undef CPP_FILE_FUNC_WRAPPER

STOREPATHS_C_LINKAGE_END()
