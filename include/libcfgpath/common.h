#ifndef LIBCFGPATH_COMMON_H_
#define LIBCFGPATH_COMMON_H_

// Use extern "C" when compiled in C++
#ifdef __cplusplus
  #define LIBCFGPATH_C_LINKAGE() \
    extern "C" {
  #define LIBCFGPATH_C_LINKAGE_END() \
    } // extern "C"
#else
  #define LIBCFGPATH_C_LINKAGE()
  #define LIBCFGPATH_C_LINKAGE_END()
#endif

// Platform determination
#if defined(__linux__) || defined(BSD)
  #define LIBCFGPATH_OS_LINUX
#elif defined(WIN32) || defined(_WIN32)
  #define LIBCFGPATH_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
  #define LIBCFGPATH_OS_OSX
#else
  #error "Currently your platform is unsupported by libcfgpath."
#endif

// Values dependent on the platform
#if defined(LIBCFGPATH_OS_LINUX)
  #include <sys/stat.h> // Permissions for mkdir
  #define PATH_SEP_CHAR '/'
  #define PATH_SEP_STR "/"
  #define MAX_PATH_LENGTH 512  // arbitrary value
#elif defined(LIBCFGPATH_OS_WINDOWS)
  #include <windows.h>
  #define PATH_SEP_CHAR '\\'
  #define PATH_SEP_STR "\\"
  #define MAX_PATH_LENGTH MAX_PATH // Defined in the windows API
#elif defined(LIBCFGPATH_OS_OSX)
  #include <sys/stat.h> // Permissions for mkdir
  #include <sys/syslimits.h> // PATH_MAX
  #define PATH_SEP_CHAR '/'
  #define PATH_SEP_STR "/"
  #define MAX_PATH_LENGTH PATH_MAX // From <sys/syslimits.h>
#endif

// Windows thing
#if defined(_MSC_VER) || defined(__MINGW32__)
#define inline __inline
#include <direct.h>
#define mkdir _mkdir
#endif

// Define even more things
#define POSIX_CONFIG_EXTENSION ".conf"
#define WINDOWS_CONFIG_EXTENSION ".ini"
#define LIBCFGPATH_ERROR_RETURN(outBuffer) \
  outBuffer[0] = '\0'; \
  return;
#define POSIX_MKDIR_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) // 0755

LIBCFGPATH_C_LINKAGE()

enum Folders {
    CONFIG_FOLDER,
    DATA_FOLDER,
    CACHE_FOLDER
};

enum Files {
    CONFIG_FILE
};

int mkdir_parent(const char* file, mode_t mode);

LIBCFGPATH_C_LINKAGE_END()

#endif //LIBCFGPATH_COMMON_H_
