#include "libcfgpath/common.h"

#include <string>

LIBCFGPATH_C_LINKAGE()

int canAccessFolder(const char* path) {
#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_OS_OSX)
    if (access(path, F_OK | R_OK) == 0)
#elif defined(LIBCFGPATH_OS_WINDOWS)
    if (_access(path, 02) == 0)
#endif
        return 0;
    return -1;
}

int crossMkdir(const char* path, const mode_t mode) {
#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_OS_OSX)
    return mkdir(path, mode);
#elif defined(LIBCFGPATH_OS_WINDOWS)
    UNUSED(mode)
    return _mkdir(path);
#endif
}

static inline void addToken(char* elements, const char* token, bool firstIter) {
#if defined(LIBCFGPATH_OS_LINUX) || defined(LIBCFGPATH_OS_OSX)
    UNUSED(firstIter)
    strcat(elements, PATH_SEP_STR);
#elif defined(LIBCFGPATH_OS_WINDOWS)
    if (!firstIter)
        strcat(elements, PATH_SEP_STR);
#endif
    strcat(elements, token);
}

int mkdirParent(const char* path, const mode_t mode) {
    int returnCode = 0;
    bool firstIter = true;

    // Setup strtok
    std::string pathCopy = path;
    char* context = nullptr;
    const auto elements = new char[pathCopy.size() + 1];
    strcpy(elements, ""); // Empty string

    // Checks if each folder of the pat hexists
    const char* token = strtok_r(pathCopy.data(), PATH_SEP_STR, &context);
    while (token != nullptr) {
        addToken(elements, token, firstIter);
        token = strtok_r(nullptr, PATH_SEP_STR, &context);

        // Create folder if it does not exists
        if (canAccessFolder(elements) != 0) {
            if ((returnCode = crossMkdir(elements, mode)) != 0)
                return returnCode;
        }

        firstIter = false;
    }

    delete[] elements;
    return returnCode;
}

LIBCFGPATH_C_LINKAGE_END()
