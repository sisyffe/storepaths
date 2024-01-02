#include "libcfgpath/cfgpath.h"

#ifndef LIBCFGPATH_OS_OSX
#warning "You are building the osx implementation in a not apple environnement"
#endif

#include <string.h>
#include <stdlib.h>
#include <sysdir.h> /* Apple API */
#include <glob.h> /* Utility stuff (replace the tilde) */

LIBCFGPATH_CPP_CHECK()

void getConfigFolder(char* outBuffer, const size_t maxLength, const char* appName) {
    const sysdir_search_path_enumeration_state state = sysdir_start_search_path_enumeration(
        SYSDIR_DIRECTORY_APPLICATION_SUPPORT,
        SYSDIR_DOMAIN_MASK_USER
    );
    if (!sysdir_get_next_search_path_enumeration(state, outBuffer)) {
        LIBCFGPATH_ERROR_RETURN(outBuffer)
    }

    // Remove the tilde
    glob_t globbuf;
    if (glob(outBuffer, GLOB_TILDE, NULL, &globbuf) != 0) {
        globfree(&globbuf);
        LIBCFGPATH_ERROR_RETURN(outBuffer)
    }

    //  path/to/config/folder         /   appname           /   \0
    if (strlen(globbuf.gl_pathv[0]) + 1 + strlen(appName) + 1 + 1 > maxLength) {
        globfree(&globbuf);
        LIBCFGPATH_ERROR_RETURN(outBuffer)
    }

    // final copy
    strcpy(outBuffer, globbuf.gl_pathv[0]);
    strcat(outBuffer, PATH_SEP_STR);
    strcat(outBuffer, appName);
    // Make the config folder if it doesn't already exist
    mkdir(outBuffer, POSIX_MKDIR_MODE);
    strcat(outBuffer, PATH_SEP_STR);

    globfree(&globbuf);
}

void getDataFolder(char* outBuffer, const size_t maxLength, const char* appName) {
    // Same implementation as config folder
    getConfigFolder(outBuffer, maxLength, appName);
}

void getCacheFolder(char* outBuffer, const size_t maxLength, const char* appName) {
    // Same implementation as config folder
    getConfigFolder(outBuffer, maxLength, appName);
}

void getConfigFile(char* outBuffer, const size_t maxLength, const char* appName) {
    // Return a file in the config folder so need the config folder
    getConfigFolder(outBuffer, maxLength, appName);

    //  /path/to/config/    appname   .conf                            \0
    if (strlen(outBuffer) + appName + strlen(POSIX_CONFIG_EXTENSION) + 1 > maxLength) {
        LIBCFGPATH_ERROR_RETURN(outBuffer);
    }

    // final copy
    strcat(outBuffer, appName);
    strcat(outBuffer, POSIX_CONFIG_EXTENSION);
}

LIBCFGPATH_CPP_CHECK_END()
