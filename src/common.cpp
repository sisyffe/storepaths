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
    return _mkdir(path);
#endif
}

int mkdirParent(const char* path, const mode_t mode) {
    int returnCode = 0;

    // Setup strtok
    std::string pathCopy = path;
    char* context;
    const auto elements = new char[pathCopy.size() + 1];
    strcpy(elements, ""); // Empty string

    const char* token = strtok_r(pathCopy.data(), PATH_SEP_STR, &context);
    while (token != nullptr) {
        strcat(elements, PATH_SEP_STR);
        strcat(elements, token);
        token = strtok_r(nullptr, PATH_SEP_STR, &context);

        if (canAccessFolder(elements) != 0) {
            if ((returnCode = crossMkdir(elements, mode)) != 0) {
                return returnCode;
            }
        }
    }

    delete[] elements;
    return returnCode;
}

LIBCFGPATH_C_LINKAGE_END()
