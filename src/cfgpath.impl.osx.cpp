#include "libcfgpath/cfgpath.h"

#ifndef LIBCFGPATH_OS_OSX
#warning "You are building the osx implementation in a not apple environnement"
#endif

#include <sysdir.h> /* Apple API */
#include <glob.h> /* Utility stuff (replace the tilde) */
#include <sstream>

static inline std::string getBaseFolder(const std::string& appName) {
    char buffer[MAX_PATH_LENGTH];
    const sysdir_search_path_enumeration_state state = sysdir_start_search_path_enumeration(
        SYSDIR_DIRECTORY_APPLICATION_SUPPORT,
        SYSDIR_DOMAIN_MASK_USER
    );
    if (!sysdir_get_next_search_path_enumeration(state, buffer)) {
        return "";
    }

    glob_t globbuf;
    if (glob(buffer, GLOB_TILDE, nullptr, &globbuf) != 0) {
        globfree(&globbuf);
        return "";
    }

    std::ostringstream result;

    // final copy
    result << globbuf.gl_pathv[0];
    result << PATH_SEP_CHAR;
    result << appName;
    result << PATH_SEP_CHAR;

    return result.str();
}

std::string getConfigFolder(const std::string& appName) {
    std::ostringstream result;

    // final copy
    result << getBaseFolder(appName);
    result << LIBCFGPATH_OSX_CONFIG_FOLDER;
    // Make the config folder if it doesn't already exist
    mkdirParent(result.str().c_str(), MKDIR_MODE);
    result << PATH_SEP_CHAR;

    return result.str();
}

std::string getDataFolder(const std::string& appName) {
    std::ostringstream result;

    // final copy
    result << getBaseFolder(appName);
    result << LIBCFGPATH_OSX_DATA_FOLDER;
    // Make the config folder if it doesn't already exist
    mkdirParent(result.str().c_str(), MKDIR_MODE);
    result << PATH_SEP_CHAR;

    return result.str();
}

std::string getCacheFolder(const std::string& appName) {
    std::ostringstream result;

    // final copy
    result << getBaseFolder(appName);
    result << LIBCFGPATH_OSX_CACHE_FOLDER;
    // Make the config folder if it doesn't already exist
    mkdirParent(result.str().c_str(), MKDIR_MODE);
    result << PATH_SEP_CHAR;

    return result.str();
}

std::string getConfigFile(const std::string& appName) {
    std::ostringstream result;
    result << getBaseFolder(appName);
    result << appName;
    result << CONFIG_EXTENSION;

    return result.str();
}
