#include "libcfgpath/cfgpath.h"

#ifndef LIBCFGPATH_OS_OSX
#  error Cannot build this file because you are not on macOS
#endif

#include <sysdir.h> /* Apple API */
#include <glob.h> /* Utility stuff (replace the tilde) */

#define LIBCFGPATH_INCLUDE_IMPLEMENTATION
#include "libcfgpath/implementations.hpp"
#include "libcfgpath/sizedstream.hpp"

namespace libcfgpath {
    static inline bool getBaseFolder(SizedStream& stream, const std::string& appName) {
        // This function does not create the folder it returns
        char buffer[MAX_PATH_LENGTH];
        const sysdir_search_path_enumeration_state state = sysdir_start_search_path_enumeration(
            SYSDIR_DIRECTORY_APPLICATION_SUPPORT,
            SYSDIR_DOMAIN_MASK_USER
        );
        if (!sysdir_get_next_search_path_enumeration(state, buffer)) {
            return false;
        }

        glob_t globbuf;
        if (glob(buffer, GLOB_TILDE, nullptr, &globbuf) != 0) {
            globfree(&globbuf);
            return false;
        }

        stream << globbuf.gl_pathv[0] << PATH_SEP_CHAR << appName << PATH_SEP_CHAR;

        globfree(&globbuf);
        return true;
    }

    DEFINE_GET_FOLDER_FUNCTION(
        getConfigFolder,
        getBaseFolder(result, appName),
        LIBCFGPATH_OSX_CONFIG_FOLDER << PATH_SEP_CHAR
    )

    DEFINE_GET_FOLDER_FUNCTION(
        getDataFolder,
        getBaseFolder(result, appName),
        LIBCFGPATH_OSX_DATA_FOLDER << PATH_SEP_CHAR
    )

    DEFINE_GET_FOLDER_FUNCTION(
        getCacheFolder,
        getBaseFolder(result, appName),
        LIBCFGPATH_OSX_CACHE_FOLDER << PATH_SEP_CHAR
    )

    DEFINE_GET_FILE_FUNCTION(
        getConfigFile,
        getBaseFolder(result, appName),
        appName << CONFIG_EXTENSION
    )
} // libcfgpath
