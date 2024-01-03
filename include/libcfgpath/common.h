#ifndef LIBCFGPATH_COMMON_H_
#define LIBCFGPATH_COMMON_H_

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

// Includes depending on the platform
#if defined(LIBCFGPATH_OS_LINUX)
  #include <unistd.h> // access
  #include <sys/stat.h> // mkdir
#elif defined(LIBCFGPATH_OS_WINDOWS)
  #include <windows.h> // MAX_PATH
  #include <direct.h> // idk
  #include <io.h> // _mkdir and _access
#elif defined(LIBCFGPATH_OS_OSX)
  #include <unistd.h> // access
  #include <sys/stat.h> // mkdir
  #include <sys/syslimits.h> // PATH_MAX
#endif

// Values depending on the platform
#if defined(LIBCFGPATH_OS_LINUX)
  #define PATH_SEP_CHAR '/'
  #define PATH_SEP_STR "/"
  #define MAX_PATH_LENGTH 512  // arbitrary value

  #define CONFIG_EXTENSION ".conf"
  #define MKDIR_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) // 0755
#elif defined(LIBCFGPATH_OS_WINDOWS)
  #define PATH_SEP_CHAR '\\'
  #define PATH_SEP_STR "\\"
  #define MAX_PATH_LENGTH MAX_PATH // Defined in the windows API

  #define CONFIG_EXTENSION ".ini"
  #define MKDIR_MODE 0 // No mkdir mode on windows
  #define mkdir(path, mode) _mkdir(path)
  #define mode_t unsigned short
  // #define inline __inline // disabled
#elif defined(LIBCFGPATH_OS_OSX)
  #define PATH_SEP_CHAR '/'
  #define PATH_SEP_STR "/"
  #define MAX_PATH_LENGTH PATH_MAX // From <sys/syslimits.h>

  #define CONFIG_EXTENSION ".conf"
  #define MKDIR_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) // 0755

  #define LIBCFGPATH_OSX_CONFIG_FOLDER "config"
  #define LIBCFGPATH_OSX_DATA_FOLDER "data"
  #define LIBCFGPATH_OSX_CACHE_FOLDER "cache"
#endif

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

#define LIBCFGPATH_ERROR_RETURN(outBuffer) \
  outBuffer[0] = '\0'; \
  return;

LIBCFGPATH_C_LINKAGE()

enum Folders {
    CONFIG_FOLDER,
    DATA_FOLDER,
    CACHE_FOLDER
};

enum Files {
    CONFIG_FILE
};

int canAccessFolder(const char* path);
int crossMkdir(const char* path, mode_t mode);

int mkdirParent(const char* path, mode_t mode);

LIBCFGPATH_C_LINKAGE_END()

#endif //LIBCFGPATH_COMMON_H_
