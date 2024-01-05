#include "libcfgpath/cfgpath.h"

#define CPP_FUNC_WRAPPER(funcName, callbackFunc) \
    void funcName(char* outBuffer, const size_t maxLength, const char* appName) { \
        std::string result = callbackFunc(appName); \
        if (result.size() + 1 > maxLength) { \
            outBuffer[0] = '\0'; \
            return; \
        } \
        std::memcpy(outBuffer, result.data(), result.size() + 1); \
    }

namespace libcfgpath {
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
} // libcfgpath

// # Proxy for C
LIBCFGPATH_C_LINKAGE()

CPP_FUNC_WRAPPER(getConfigFolder, libcfgpath::getConfigFolder)
CPP_FUNC_WRAPPER(getDataFolder, libcfgpath::getDataFolder)
CPP_FUNC_WRAPPER(getCacheFolder, libcfgpath::getCacheFolder)
CPP_FUNC_WRAPPER(getConfigFile, libcfgpath::getConfigFile)

LIBCFGPATH_C_LINKAGE_END()

#undef CPP_FUNC_WRAPPER
