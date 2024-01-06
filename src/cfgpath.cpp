#include "libcfgpath/cfgpath.h"

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

namespace libcfgpath {
    std::pair<std::string, bool> getFolder(const Folders folderType, const std::string& appName) {
        switch (folderType) {
            case CONFIG_FOLDER:
                return getConfigFolder(appName);
            case DATA_FOLDER:
                return getDataFolder(appName);
            case CACHE_FOLDER:
                return getCacheFolder(appName);
            default:
                return { "", false };
        }
    }

    std::string getFile(const Files fileType, const std::string &appName) {
        switch (fileType) {
            case CONFIG_FILE:
                return getConfigFile(appName);
            default:
                return "";
        }
    }
} // libcfgpath

// # Proxy for C
LIBCFGPATH_C_LINKAGE()

CPP_FOLDER_FUNC_WRAPPER(getConfigFolder, libcfgpath::getConfigFolder)
CPP_FOLDER_FUNC_WRAPPER(getDataFolder, libcfgpath::getDataFolder)
CPP_FOLDER_FUNC_WRAPPER(getCacheFolder, libcfgpath::getCacheFolder)
CPP_FILE_FUNC_WRAPPER(getConfigFile, libcfgpath::getConfigFile)

LIBCFGPATH_C_LINKAGE_END()

#undef CPP_FUNC_WRAPPER
