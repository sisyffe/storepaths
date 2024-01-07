#include "libcfgpath/cfgpath.h"

#include <wordexp.h>

#if !defined(LIBCFGPATH_OS_LINUX) && !defined(LIBCFGPATH_OS_OSX)
#  error Cannot build this file because you are not on Linux
#endif

#define LIBCFGPATH_CAN_INCLUDE_IMPLEMENTATION_HPP
#include "libcfgpath/implementations.hpp"
#include "libcfgpath/sizedstream.hpp"

namespace libcfgpath::linux {
    enum PathType { HOME, SPECIFIC };

    static inline std::string expand(const char* string) {
        // Used mainly to expand $HOME variable
        wordexp_t p;
        wordexp(string, &p, 0);
        if (p.we_wordc < 1)
            return "";

        std::string result = p.we_wordv[0];
        wordfree(&p);
        return result;
    }

    static inline bool getSpecificFolder(SizedStream& stream, const std::string& appName,
                                 const char* envVar, const char* defaultFolder) {
        PathType pathType = SPECIFIC;
        const char* var = getenv(envVar);

        // Determine which variable to use
        if (var == nullptr) {
            pathType = HOME;

            if (getenv("HOME") == nullptr) // Cannot find home directory
                return false;
        }

        std::string expanded;

        // Get the configiration directory
        switch (pathType) {
            case SPECIFIC: // XDG_CONFIG_HOME found
                if (var[0] != PATH_SEP_CHAR) // Path is not absolute so invalid
                    return false;
            stream << var;
            break;
            case HOME: // Using the home directory
                if ((expanded = expand(defaultFolder)).empty()
                    || expanded[0] != PATH_SEP_CHAR) // Expantion failed or path is not absolute
                        return false;
            stream << expanded;
            break;
        }

        // Add the app name
        stream << PATH_SEP_CHAR << appName << PATH_SEP_CHAR;
        return true;
    }

    DEFINE_GET_FOLDER_FUNCTION(
        getConfigFolder,
        getSpecificFolder(result, appName, "XDG_CONFIG_HOME", LIBCFGPATH_LINUX_DEFAULT_CONFIG_FOLDER),
        ""
    )

    DEFINE_GET_FOLDER_FUNCTION(
        getDataFolder,
        getSpecificFolder(result, appName, "XDG_DATA_HOME", LIBCFGPATH_LINUX_DEFAULT_DATA_FOLDER),
        ""
    )

    DEFINE_GET_FOLDER_FUNCTION(
        getCacheFolder,
        getSpecificFolder(result, appName, "XDG_CACHE_HOME", LIBCFGPATH_LINUX_DEFAULT_CACHE_FOLDER),
        ""
    )

    DEFINE_GET_FILE_FUNCTION(
        getConfigFile,
        getSpecificFolder(result, appName, "XDG_CONFIG_HOME", LIBCFGPATH_LINUX_DEFAULT_CONFIG_FOLDER),
        appName << CONFIG_EXTENSION
    )
} // libcfgpath
