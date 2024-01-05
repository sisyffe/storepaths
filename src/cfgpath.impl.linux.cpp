#include "libcfgpath/cfgpath.h"

#ifndef LIBCFGPATH_OS_LINUX
#  error Cannot build this file because you are not on Linux
#endif

// Not implemented right now

namespace libcfgpath {
    std::string getConfigFolder(const std::string& appName) {
        return "";
    }

    std::string getDataFolder(const std::string& appName) {
        return "";
    }

    std::string getCacheFolder(const std::string& appName) {
        return "";
    }

    std::string getConfigFile(const std::string& appName) {
        return "";
    }
} // libcfgpath
