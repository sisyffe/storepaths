#include "libcfgpath/cfgpath.h"

#define CPP_FUNC_WRAPPER(funcName) \
    void funcName(char* outBuffer, const size_t maxLength, const char* appName) { \
        std::string result = funcName(appName); \
        if (result.size() + 1 > maxLength) { \
            LIBCFGPATH_ERROR_RETURN(outBuffer) \
        } \
        std::memcpy(outBuffer, result.data(), result.size() + 1); \
    }

LIBCFGPATH_IF_CPP()

CPP_FUNC_WRAPPER(getConfigFolder)
CPP_FUNC_WRAPPER(getDataFolder)
CPP_FUNC_WRAPPER(getCacheFolder)
CPP_FUNC_WRAPPER(getConfigFile)

LIBCFGPATH_IF_CPP_END()

#undef CPP_FUNC_WRAPPER
