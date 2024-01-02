#include "libcfgpath/cfgpath.h"

#define CPP_FUNC_WRAPPER(funcName) \
    void funcName(char* outBuffer, const size_t maxLength, const char* appName) { \
        std::string result = funcName(appName); \
        if (result.size() + 1 > maxLength) { \
            LIBCFGPATH_ERROR_RETURN(outBuffer) \
        } \
        std::memcpy(outBuffer, result.data(), result.size() + 1); \
    }

#define COMMON_CPP_FUNC_WRAPPER(Type, funcName) \
    void funcName(Type type, char *outBuffer, const size_t maxLength, const char *appName) { \
        std::string result = funcName(type, appName); \
        if (result.size() + 1 > maxLength) { \
            LIBCFGPATH_ERROR_RETURN(outBuffer) \
        } \
        std::memcpy(outBuffer, result.data(), result.size() + 1); \
    }

std::string getFolder(const Folders folderType, const std::string& appName) {
    switch (folderType) {
        case CONFIG_FOLDER:
            return getConfigFolder(appName);
        case DATA_FOLDER:
            return getDataFolder(appName);
        case CACHE_FOLDER:
            return getCacheFolder(appName);
        default:
            return "";
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

LIBCFGPATH_C_LINKAGE()

CPP_FUNC_WRAPPER(getConfigFolder)
CPP_FUNC_WRAPPER(getDataFolder)
CPP_FUNC_WRAPPER(getCacheFolder)
CPP_FUNC_WRAPPER(getConfigFile)

COMMON_CPP_FUNC_WRAPPER(Folders, getFolder)
COMMON_CPP_FUNC_WRAPPER(Files, getFile)

LIBCFGPATH_C_LINKAGE_END()

#undef CPP_FUNC_WRAPPER
