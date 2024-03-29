#ifndef STOREPATHS_COMMON_H_
#define STOREPATHS_COMMON_H_

#ifndef __cplusplus
#include <stdbool.h>
#endif

// # Macros and includes
// ## Platform determination
#if defined(__linux__) || defined(BSD)
#  define STOREPATHS_OS_LINUX
#elif defined(WIN32) || defined(_WIN32)
#  define STOREPATHS_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#  define STOREPATHS_OS_OSX
#else
#  error "Currently your platform is unsupported by STOREPATHS."
#endif

// ## Includes depending on the platform
#if defined(STOREPATHS_OS_LINUX)
#  include <sys/stat.h> // mkdir
#elif defined(STOREPATHS_OS_WINDOWS)
#  include <windows.h> // MAX_PATH
#  include <direct.h> // _mkdir
#  include <io.h> // _access
#elif defined(STOREPATHS_OS_OSX)
#  include <sys/stat.h> // mkdir
#  include <sys/syslimits.h> // PATH_MAX
#endif

// ## Values depending on the platform
#if defined(STOREPATHS_OS_LINUX)
#  define PATH_SEP_CHAR '/'
#  define PATH_SEP_STR "/"
#  define MAX_PATH_LENGTH 512  // arbitrary value

#  define STOREPATHS_MKDIR_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) // 0755
#elif defined(STOREPATHS_OS_WINDOWS)
#  define PATH_SEP_CHAR '\\'
#  define PATH_SEP_STR "\\"
#  define MAX_PATH_LENGTH MAX_PATH // Defined in the windows API

#  define STOREPATHS_MKDIR_MODE 0 // No mkdir mode on windows
#  define mkdir(path, mode) _mkdir(path)
#  define strtok_r strtok_s
#  define mode_t unsigned short
#elif defined(STOREPATHS_OS_OSX)
#  define PATH_SEP_CHAR '/'
#  define PATH_SEP_STR "/"
#  define MAX_PATH_LENGTH PATH_MAX // From <sys/syslimits.h>

#  define STOREPATHS_MKDIR_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) // 0755
#endif

// ### Common Values
#define STOREPATHS_DEFAULT_CONFIG_FILE_EXTENSION "json"
// Posix
#define STOREPATHS_POSIX_CONFIG_EXTENSION ".conf"
#define STOREPATHS_POSIX_DEFAULT_CONFIG_FOLDER "$HOME/.config"
#define STOREPATHS_POSIX_DEFAULT_DATA_FOLDER "$HOME/.local/share"
#define STOREPATHS_POSIX_DEFAULT_CACHE_FOLDER "$HOME/.cache"
// Windows
#define STOREPATHS_WINDOWS_CONFIG_EXTENSION ".ini"

#define STOREPATHS_WINDOWS_COM_INIT_NOINIT 0
#define STOREPATHS_WINDOWS_COM_INIT_GLOBAL 1
#define STOREPATHS_WINDOWS_COM_INIT_LAZY 2
#define STOREPATHS_WINDOWS_COM_INIT_EACH_USE 3

#ifndef STOREPATHS_WINDOWS_COM_INIT_MODE
#  define STOREPATHS_WINDOWS_COM_INIT_MODE STOREPATHS_WINDOWS_COM_INIT_GLOBAL
#endif
// OSX
#define STOREPATHS_OSX_CONFIG_EXTENSION ".plist"

// ## Use extern "C" when compiled in C++
#ifdef __cplusplus
#  define STOREPATHS_C_LINKAGE \
      extern "C" {
#  define STOREPATHS_C_LINKAGE_END \
      } // extern "C"
#  define STOREPATHS_NAMESPACE_IF_CPP \
    namespace storepaths {
#  define STOREPATHS_NAMESPACE_IF_CPP_END \
    } // storepaths
#else
#  define STOREPATHS_C_LINKAGE
#  define STOREPATHS_C_LINKAGE_END
#  define STOREPATHS_NAMESPACE_IF_CPP
#  define STOREPATHS_NAMESPACE_IF_CPP_END
#endif

// ## Other things
#ifndef UNUSED
#  define UNUSED(x) (void)x;
#endif

// # Common stuff
// ## Enums for C++ functions
#ifdef __cplusplus
namespace storepaths {
    enum Folders {
        CONFIG_FOLDER,
        DATA_FOLDER,
        CACHE_FOLDER
    };

    enum Files {
        COMMON_CONFIG_FILE,
        PLATFORM_CONFIG_FILE
    };
} // storepaths
#endif // __cplusplus

STOREPATHS_NAMESPACE_IF_CPP
typedef struct {
    bool found;
    int mkdirReturnCode;
    bool bufferTooSmall;
    bool succeeded;
} PathInfo;
STOREPATHS_NAMESPACE_IF_CPP_END

// ## System functions
STOREPATHS_C_LINKAGE
int canAccessFolder(const char* path);
int crossMkdir(const char* path, mode_t mode);
int mkdirParent(const char* path, mode_t mode);
STOREPATHS_C_LINKAGE_END

#endif //STOREPATHS_COMMON_H_
