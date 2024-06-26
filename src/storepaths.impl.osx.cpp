#include "storepaths/storepaths.h"

#ifndef STOREPATHS_OS_OSX
#error Cannot build this file because you are not on macOS
#endif

#include <glob.h>   /* Utility stuff (replace the tilde) */
#include <sysdir.h> /* Apple API */

#define STOREPATHS_CAN_INCLUDE_IMPLEMENTATION_HPP
#include "storepaths/implementations.hpp"
#include "storepaths/sizedstream.hpp"

namespace storepaths::osx {
    static inline bool getSpecificFolder(SizedStream &stream,
            const sysdir_search_path_directory_t directory) {
        char buffer[MAX_PATH_LENGTH];
        const sysdir_search_path_enumeration_state state =
            sysdir_start_search_path_enumeration(directory, SYSDIR_DOMAIN_MASK_USER);

        if (!sysdir_get_next_search_path_enumeration(state, buffer))
            return false;
        // You can continue with `sysdir_get_next_search_path_enumeration` but here we
        // are only getting the first one only

        glob_t globbuf;
        if (glob(buffer, GLOB_TILDE, nullptr, &globbuf) != 0) {
            globfree(&globbuf);
            return false;
        }

        stream << globbuf.gl_pathv[0] << PATH_SEP_CHAR;

        globfree(&globbuf);
        return true;
    }

    DEFINE_GET_FOLDER_FUNCTION(
        getConfigFolder,
        getSpecificFolder(result, SYSDIR_DIRECTORY_APPLICATION_SUPPORT),
        appName << PATH_SEP_CHAR
    )

    DEFINE_GET_FOLDER_FUNCTION(
        getDataFolder,
        getSpecificFolder(result, SYSDIR_DIRECTORY_APPLICATION_SUPPORT),
        appName << PATH_SEP_CHAR
    )

    DEFINE_GET_FOLDER_FUNCTION(
        getCacheFolder,
        getSpecificFolder(result, SYSDIR_DIRECTORY_CACHES),
        appName << PATH_SEP_CHAR
    )

    DEFINE_GET_COMMON_FILE_FUNCTION(
        getCommonConfigFile,
        getSpecificFolder(result, SYSDIR_DIRECTORY_APPLICATION_SUPPORT),
        appName << PATH_SEP_CHAR,
        fileName.value_or(appName) << '.' << extension
    )

    std::pair<std::string, PathInfo> getOsxConfigFile(const std::string &appName,
            const std::optional<std::string> &fileName) {
        UNUSED(appName) // OSX implementation does not use this but others do so need
                        // the signature to be the same
        SizedStream stream{MAX_PATH_LENGTH};
        const char *homeDir = getenv("HOME");

        if (homeDir == nullptr)
            return {"", {false, 0, false, false}};

        stream << homeDir << "/Library/Preferences/" << fileName.value_or(appName)
               << STOREPATHS_OSX_CONFIG_EXTENSION;
        return {stream.toString(), {true, 0, false, true}};
    }
} // namespace storepaths::osx
