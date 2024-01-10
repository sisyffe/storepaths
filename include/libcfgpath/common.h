#ifndef LIBCFGPATH_COMMON_H_
#define LIBCFGPATH_COMMON_H_

#ifndef __cplusplus
#include <stdbool.h>
#endif

// # Macros and includes
// ## Platform determination
#if defined(__linux__) || defined(BSD)
#  define LIBCFGPATH_OS_LINUX
#elif defined(WIN32) || defined(_WIN32)
#  define LIBCFGPATH_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#  define LIBCFGPATH_OS_OSX
#else
#  error "Currently your platform is unsupported by libcfgpath."
#endif

// ## Includes depending on the platform
#if defined(LIBCFGPATH_OS_LINUX)
#  include <sys/stat.h> // mkdir
#elif defined(LIBCFGPATH_OS_WINDOWS)
#  include <windows.h> // MAX_PATH
#  include <direct.h> // _mkdir
#  include <io.h> // _access
#elif defined(LIBCFGPATH_OS_OSX)
#  include <sys/stat.h> // mkdir
#  include <sys/syslimits.h> // PATH_MAX
#endif

// ## Values depending on the platform
#if defined(LIBCFGPATH_OS_LINUX)
#  define PATH_SEP_CHAR '/'
#  define PATH_SEP_STR "/"
#  define MAX_PATH_LENGTH 512  // arbitrary value

#  define LIBCFGPATH_MKDIR_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) // 0755
#elif defined(LIBCFGPATH_OS_WINDOWS)
#  define PATH_SEP_CHAR '\\'
#  define PATH_SEP_STR "\\"
#  define MAX_PATH_LENGTH MAX_PATH // Defined in the windows API

#  define LIBCFGPATH_MKDIR_MODE 0 // No mkdir mode on windows
#  define mkdir(path, mode) _mkdir(path)
#  define strtok_r strtok_s
#  define mode_t unsigned short
#elif defined(LIBCFGPATH_OS_OSX)
#  define PATH_SEP_CHAR '/'
#  define PATH_SEP_STR "/"
#  define MAX_PATH_LENGTH PATH_MAX // From <sys/syslimits.h>

#  define LIBCFGPATH_MKDIR_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) // 0755
#endif

// ### Common Values
#define LIBCFGPATH_JSON_EXTENSION ".json"
// Posix
#define LIBCFGPATH_POSIX_CONFIG_EXTENSION ".conf"
#define LIBCFGPATH_POSIX_DEFAULT_CONFIG_FOLDER "$HOME/.config"
#define LIBCFGPATH_POSIX_DEFAULT_DATA_FOLDER "$HOME/.local/share"
#define LIBCFGPATH_POSIX_DEFAULT_CACHE_FOLDER "$HOME/.cache"
// Windows
#define LIBCFGPATH_WINDOWS_CONFIG_EXTENSION ".ini"
#define LIBCFGPATH_WINDOWS_CONFIG_FOLDER "config"
#define LIBCFGPATH_WINDOWS_DATA_FOLDER "data"

#define LIBCFGPATH_WINDOWS_COM_INIT_NOINIT 0
#define LIBCFGPATH_WINDOWS_COM_INIT_GLOBAL 1
#define LIBCFGPATH_WINDOWS_COM_INIT_LAZY 2
#define LIBCFGPATH_WINDOWS_COM_INIT_EACH_USE 3

#ifndef LIBCFGPATH_WINDOWS_COM_INIT_MODE
#  define LIBCFGPATH_WINDOWS_COM_INIT_MODE LIBCFGPATH_WINDOWS_COM_INIT_GLOBAL
#endif
// OSX
#define LIBCFGPATH_OSX_CONFIG_EXTENSION ".plist"
#define LIBCFGPATH_OSX_CONFIG_FOLDER "config"
#define LIBCFGPATH_OSX_DATA_FOLDER "data"

// ## Use extern "C" when compiled in C++
#ifdef __cplusplus
#  define LIBCFGPATH_C_LINKAGE() \
      extern "C" {
#  define LIBCFGPATH_C_LINKAGE_END() \
      } // extern "C"
#  define LIBCGPATH_STRUCT_DEFINITION(structName) \
      struct structName {
#  define LIBCGPATH_STRUCT_DEFINITION_END(structName) \
      };
#else
#  define LIBCFGPATH_C_LINKAGE()
#  define LIBCFGPATH_C_LINKAGE_END()
#  define LIBCGPATH_STRUCT_DEFINITION(structName) \
      typedef struct {
#  define LIBCGPATH_STRUCT_DEFINITION_END(structName) \
      } structName;
#endif

// ## Other things
#ifndef UNUSED
#  define UNUSED(x) (void)x;
#endif

// # Common stuff
// ## Enums for C++ functions
#ifdef __cplusplus
namespace libcfgpath {
    enum Folders {
        CONFIG_FOLDER,
        DATA_FOLDER,
        CACHE_FOLDER
    };

    enum Files {
        JSON_CONFIG_FILE,
        PLATFORM_CONFIG_FILE
    };
} // libcfgpath
#endif // __cplusplus

LIBCGPATH_STRUCT_DEFINITION(PathInfo)
    bool found;
    int mkdirReturnCode;
    bool bufferTooSmall;
    bool succeeded;
LIBCGPATH_STRUCT_DEFINITION_END(PathInfo)

// ## System functions
LIBCFGPATH_C_LINKAGE()
int canAccessFolder(const char* path);
int crossMkdir(const char* path, mode_t mode);
int mkdirParent(const char* path, mode_t mode);
LIBCFGPATH_C_LINKAGE_END()

#endif //LIBCFGPATH_COMMON_H_
