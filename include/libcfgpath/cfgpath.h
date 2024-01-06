#ifndef LIBCFGPATH_CFGPATH_H_
#define LIBCFGPATH_CFGPATH_H_

#include "libcfgpath/common.h"

#ifndef __cplusplus
#  include <stdbool.h>
#endif

// # Actual implementation
#ifdef __cplusplus
#include <string>

namespace libcfgpath {
    std::pair<std::string, bool> getConfigFolder(const std::string& appName);
    std::pair<std::string, bool> getDataFolder(const std::string& appName);
    std::pair<std::string, bool> getCacheFolder(const std::string& appName);
    std::string getConfigFile(const std::string& appName);

    std::pair<std::string, bool> getFolder(Folders folderType, const std::string& appName);
    std::string getFile(Files fileType, const std::string& appName);
} // libcfgpath
#endif

// # Proxy for C of the above functions
LIBCFGPATH_C_LINKAGE()

bool getConfigFolder(char* outBuffer, size_t maxLength, const char* appName);
bool getDataFolder(char* outBuffer, size_t maxLength, const char* appName);
bool getCacheFolder(char* outBuffer, size_t maxLength, const char* appName);
void getConfigFile(char* outBuffer, size_t maxLength, const char* appName);

LIBCFGPATH_C_LINKAGE_END()

#endif //LIBCFGPATH_CFGPATH_H_
