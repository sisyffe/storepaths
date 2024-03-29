#include "storepaths/common.h"

#include <string>

#include "storepaths/sizedstream.hpp"

using namespace storepaths;

#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_OS_OSX)
#  include <unistd.h> // access
#endif

STOREPATHS_C_LINKAGE

int canAccessFolder(const char* path) {
#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_OS_OSX)
    if (access(path, F_OK | R_OK) == 0)
#elif defined(STOREPATHS_OS_WINDOWS)
    if (_access(path, 02) == 0)
#endif
        return 0;
    return -1;
}

int crossMkdir(const char* path, const mode_t mode) {
#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_OS_OSX)
    return mkdir(path, mode);
#elif defined(STOREPATHS_OS_WINDOWS)
    UNUSED(mode)
    return _mkdir(path);
#endif
}

static inline void addToken(SizedStream& elements, const char* token, bool firstIter) {
#if defined(STOREPATHS_OS_LINUX) || defined(STOREPATHS_OS_OSX)
    UNUSED(firstIter)
    elements << PATH_SEP_CHAR;
#elif defined(STOREPATHS_OS_WINDOWS)
    if (!firstIter)
        elements << PATH_SEP_CHAR;
#endif
    elements << token;
}

int mkdirParent(const char* path, const mode_t mode) {
    int returnCode = 0;
    bool firstIter = true;

    // Setup strtok
    std::string pathCopy = path;
    char* context = nullptr;
    SizedStream elements{ pathCopy.size() + 1 };

    // Checks if each folder of the pat hexists
    const char* token = strtok_r(pathCopy.data(), PATH_SEP_STR, &context);
    while (token != nullptr) {
        addToken(elements, token, firstIter);
        token = strtok_r(nullptr, PATH_SEP_STR, &context);

        // Create folder if it does not exists
        if (canAccessFolder(elements.readBuffer()) != 0) {
            if ((returnCode = crossMkdir(elements.readBuffer(), mode)) != 0)
                return returnCode;
        }

        firstIter = false;
    }

    return returnCode;
}

STOREPATHS_C_LINKAGE_END
