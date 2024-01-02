#include "libcfgpath/cfgpath.h"

#ifndef LIBCFGPATH_OS_OSX
#warning "You are building the osx implementation in a not apple environnement"
#endif

#include <sysdir.h> /* Apple API */
#include <glob.h> /* Utility stuff (replace the tilde) */
#include <sstream>

std::string getConfigFolder(const std::string& appName) {
    char buffer[MAX_PATH_LENGTH];
    const sysdir_search_path_enumeration_state state = sysdir_start_search_path_enumeration(
        SYSDIR_DIRECTORY_APPLICATION_SUPPORT,
        SYSDIR_DOMAIN_MASK_USER
    );
    if (!sysdir_get_next_search_path_enumeration(state, buffer)) {
        return "";
    }

    glob_t globbuf;
    if (glob(buffer, GLOB_TILDE, NULL, &globbuf) != 0) {
        globfree(&globbuf);
        return "";
    }

    std::ostringstream result;

    // final copy
    result << globbuf.gl_pathv[0];
    result << PATH_SEP_CHAR;
    result << appName;
    // Make the config folder if it doesn't already exist
    mkdir(result.str().c_str(), POSIX_MKDIR_MODE);
    result << PATH_SEP_CHAR;

    return result.str();
}

std::string getDataFolder(const std::string& appName) {
    // Same implementation as getConfigFolder
    return getConfigFolder(appName);
}

std::string getCacheFolder(const std::string& appName) {
    // Same implementation as getConfigFolder
    return getConfigFolder(appName);
}

std::string getConfigFile(const std::string& appName) {
    std::ostringstream result;
    result << getConfigFolder(appName); // Return a file in the config folder so need the config folder
    result << appName;
    result << POSIX_CONFIG_EXTENSION;

    return result.str();
}
