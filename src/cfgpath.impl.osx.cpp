#include "libcfgpath/cfgpath.h"

#ifdef LIBCFGPATH_OS_OSX

#include <sysdir.h> /* Apple API */
#include <glob.h> /* Utility stuff (replace the tilde) */
#include <sstream>

static inline bool getBaseFolder(std::ostringstream& stream, const std::string& appName) {
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

#define GET_FILE_FOLDER_FUNCTION(funcName, funcCalled, custom) \
    std::string funcName(const std::string& appName) { \
        std::ostringstream result; \
        \
        if (!funcCalled) \
            return ""; \
        \
        result << custom; \
        mkdirParent(result.str().c_str()); \
		\
		return result.str(); \
	}

//GET_FILE_FOLDER_FUNCTION(getConfigFolder, getBaseFolder(result, appName), LIBCFGPATH_OSX_CONFIG_FOLDER << PATH_SEP_CHAR)

std::string getConfigFolder(const std::string& appName) {
    std::ostringstream result;

    if (!getBaseFolder(result, appName))
        return "";

    result << LIBCFGPATH_OSX_CONFIG_FOLDER << PATH_SEP_CHAR;
    mkdirParent(result.str().c_str(), MKDIR_MODE); // Create this folder if it doesnt already exists

    return result.str();
}

std::string getDataFolder(const std::string& appName) {
    std::ostringstream result;

    if (!getBaseFolder(result, appName))
        return "";

    result << LIBCFGPATH_OSX_DATA_FOLDER << PATH_SEP_CHAR;
    mkdirParent(result.str().c_str(), MKDIR_MODE); // Create this folder if it doesnt already exists

    return result.str();
}

std::string getCacheFolder(const std::string& appName) {
    std::ostringstream result;

    if (!getBaseFolder(result, appName))
        return "";

    result << LIBCFGPATH_OSX_CACHE_FOLDER << PATH_SEP_CHAR;
    mkdirParent(result.str().c_str(), MKDIR_MODE); // Create this folder if it doesnt already exists

    return result.str();
}

std::string getConfigFile(const std::string& appName) {
    std::ostringstream result;

    if (!getBaseFolder(result, appName))
        return "";

    mkdirParent(result.str().c_str(), MKDIR_MODE); // Create this folder if it doesnt already exists

    result << appName << CONFIG_EXTENSION;

    return result.str();
}

#endif //LIBCFGPATH_OS_OSX
