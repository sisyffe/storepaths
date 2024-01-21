#include "storepaths/storepaths.h"

#include <wordexp.h>

#if !defined(STOREPATHS_OS_LINUX) && !defined(STOREPATHS_OS_OSX)
#  error Cannot build this file because you are not on Linux
#endif

#define STOREPATHS_CAN_INCLUDE_IMPLEMENTATION_HPP
#include "storepaths/implementations.hpp"
#include "storepaths/sizedstream.hpp"

namespace storepaths::posix {
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

    static inline bool getSpecificFolder(SizedStream& stream, const char* envVar, const char* defaultFolder) {
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
        stream << PATH_SEP_CHAR;
        return true;
    }

    DEFINE_GET_FOLDER_FUNCTION(
        getConfigFolder,
        getSpecificFolder(result, "XDG_CONFIG_HOME", STOREPATHS_POSIX_DEFAULT_CONFIG_FOLDER),
        appName << PATH_SEP_CHAR
    )

    DEFINE_GET_FOLDER_FUNCTION(
        getDataFolder,
        getSpecificFolder(result, "XDG_DATA_HOME", STOREPATHS_POSIX_DEFAULT_DATA_FOLDER),
        appName << PATH_SEP_CHAR
    )

    DEFINE_GET_FOLDER_FUNCTION(
        getCacheFolder,
        getSpecificFolder(result, "XDG_CACHE_HOME", STOREPATHS_POSIX_DEFAULT_CACHE_FOLDER),
        appName << PATH_SEP_CHAR
    )

    DEFINE_GET_FILE_FUNCTION(
        getJSONConfigFile,
        getSpecificFolder(result, "XDG_CONFIG_HOME", STOREPATHS_POSIX_DEFAULT_CONFIG_FOLDER),
        appName << PATH_SEP_CHAR,
        fileName.value_or(appName) << STOREPATHS_JSON_EXTENSION
    )

    DEFINE_GET_FILE_FUNCTION(
        getPosixConfigFile,
        getSpecificFolder(result, "XDG_CONFIG_HOME", STOREPATHS_POSIX_DEFAULT_CONFIG_FOLDER),
        appName << PATH_SEP_CHAR,
        fileName.value_or(appName) << STOREPATHS_POSIX_CONFIG_EXTENSION
    )
} // storepaths::posix
