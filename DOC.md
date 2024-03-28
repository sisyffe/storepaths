# Documentation
## Standard functions

Here are the simple functions for you to use.

### C++

Every function this library defines is in the `storepaths` namespace. These are the main functions:
```c++
std::pair<std::string, PathInfo> storepaths::getConfigFolder(const std::string& appName);
std::pair<std::string, PathInfo> storepaths::getDataFolder(const std::string& appName);
std::pair<std::string, PathInfo> storepaths::getCacheFolder(const std::string& appName);
std::pair<std::string, PathInfo> storepaths::getJSONConfigFile(const std::string& appName, const std::optional<std::string>& fileName = std::nullopt);
std::pair<std::string, PathInfo> storepaths::GET_PLATFORM_CONFIG_FILE(const std::string& appName, const std::optional<std::string>& fileName = std::nullopt);
```
The `GET_PLATFORM_CONFIG_FILE` function get these names depending on the platform:

| Platform | Function name          |  
|----------|------------------------|  
| macOS    | `getOsxConfigFile`     |  
| Windows  | `getWindowsConfigFile` |  
| Linux    | `getPosixConfigFile`   |

This is because depending on the platform, file conventions are different.
On macOS for example, the configuration file should use this format: `com.company.product.plist`.
On windows, it should be a `.ini` file.
You cannot write the same thing into a `.plist` file and into a `.ini` file.
So with different function names, there is no ambiguity.
Use the `getJSONConfigFile` instead if you want a cross-platform alternative.
The functions in the `storepaths` namespace are simply _aliases_ to the platform specific functions.

This is similar with the platform specific configuration file (on most platforms, see platform specific implementations)

#### Other way

Additionally, those functions can also be used:
```c++
std::pair<std::string, PathInfo> getFolder(storepaths::Folders folderType, const std::string& appName);
std::pair<std::string, PathInfo> getFile(storepaths::Files fileType, const std::string& appName, const std::optional<std::string>& fileName = std::nullopt);
```
`Folders` and `Files` are enums:
```c++
enum Folders {
    CONFIG_FOLDER,
    DATA_FOLDER,
    CACHE_FOLDER
};
enum Files {
    JSON_CONFIG_FILE
};
```
and are not available in C just like the previous two functions.
The behaviour of those function is as you would expect.

### C
Those are the equivalent functions in C:
```c++
PathInfo getConfigFolder(char* outBuffer, size_t maxLength, const char* appName);
PathInfo getDataFolder(char* outBuffer, size_t maxLength, const char* appName);
PathInfo getCacheFolder(char* outBuffer, size_t maxLength, const char* appName);
PathInfo getJSONConfigFile(char* outBuffer, size_t maxLength, const char* appName, const char* fileName);
PathInfo GET_PLATFORM_CONFIG_FILE(char* outBuffer, size_t maxLength, const char* appName, const char* fileName);
```
As in C++, the `GET_PLATFORM_CONFIG_FILE` function get its name depending on the platform.
The `maxLength` argument is the size of the buffer you provided with the `outBuffer` parameter. If the size of the pat exceeds `maxLength`, `outBuffer` is set to an empty string and the `bufferTooSmall` flag in the return value is set to `1`.

### Return value
The return value is this struct both in C++ and C:
```c++
/* In C, bool type is from stdbool.h */
typedef struct {
    bool found;
    int mkdirReturnCode;
    bool bufferTooSmall;
    bool succeeded;
} PathInfo;
```
- `found`: tells if the system's API call succeeded (the path has been determined). If it is true, it means the result string is valid for usage, else the result string is empty.
- `mkdirReturnCode`: tells if the folder could be created (0 = success, other = failure). This is independent on the fact that the path could be determined.
- `bufferTooSmall`: tells if the provided buffer for the result is too small. Always false in C++.
- `succeeded`: combination of all previous. `true` means everything went fine, `false` mean there was an error.

### Specifications

The _folder functions_ (`getConfigFolder`, `getDataFolder`, ...) create the folder they return, so you don't have to.
The _file functions_ create the folder they are in, but the file in question is not created.

With _file functions_, the `appName` is the folder name within the file is placed (often the configuration folder).
If you specify `fileName`, the configuration file will be named after this variable. If not `appName` will be used. E.g. (C++):
- `storepaths::getJSONConfigFile("Snake Game")` => `.../Snake Game/Snake Game.json`
- `storepaths::getJSONConfigFile("Snake Game", "configuration")` => `.../Snake Game/configuration.json`

Every function you can call from this library returns an absolute path.
**Different functions may return the same path, so do not use the same file name for a _data file_ and a _config file_ for example.**

## Advanced features
### Functioning of the functions

Every path you get is compliant to every system specifications:
- macOS: [File System Basics](https://developer.apple.com/library/archive/documentation/FileManagement/Conceptual/FileSystemProgrammingGuide/FileSystemOverview/FileSystemOverview.html#//apple_ref/doc/uid/TP40010672-CH2-SW1)
- Windows: [SHGetKnownFolderPath function](https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath)
- Linux: [XDG Base Directory Specification](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html)

### C++

In C++, the actual functions you call live in a specific namespace depending on the platform:

| Platform name | Namespace             |
|---------------|-----------------------|
| macOS         | `storepaths::osx`     |
| Windows       | `storepaths::windows` |
| Linux         | `storepaths::posix`   |

The namespace for linux is called `posix` because a similarly called symbol already exists.
So you can force the use a specific implementation, e.g.:
```c++
// Compiles only on linux (and macOS, because it is compatible), result is the same everywhere
auto [posixConfigPath, posixConfigPathInfo] = storepaths::posix::getConfigFolder(APPNAME);
// Compiles on all platforms but the result can differ
auto [configPath, configPathInfo] = storepaths::getConfigFolder(APPNAME); // Aliased to storepaths::posix::getConfigFolder on linux for example
```
On Windows for example, the first fails at compile time.
The functions in the `storepaths` namespace are aliases to the platform specific functions. Literally:
```c++
namespace storepaths {
    namespace posix { /* ... */ }
    namespace windows { /* ... */ }
    namespace osx { /* ... */ }
    
    using namespace <current platform>;
}
```

### C

The C functions are actually C++ functions to retrieve the paths. They are simply exposed to C (`extern "C"`).
The C functions have one more feature !!! I can fail more often !? Relax, It simply fails if the buffer you passed is too small (for obvious safety reasons).

Just like C++, platform specific functions are also defined. They follow this pattern: `get<platform><folderName>` instead of `get<folder name>`,
e.g: `getPosixCacheFolder` and `getWindowsCacheFolder` for `getCacheFolder`.
Just like with C++, you can use a platform specific function to force the implementation. Your code may not compile on every platform.

### Utility features

Three functions are part of the library, but they can also be used by you for your own code:
```c++
/* Know if a folder exists (use `access` (linux, macOS) or `_access` (Windows) functions) */
int canAccessFolder(const char* path);
/* Make a directory (use `mkdir` (linux, macOS) or `_mkdir` (Windows) functions). On windows `mode` is ignored.  */
int crossMkdir(const char* path, mode_t mode);
/* Equivalent to `mkdir -p` */
int mkdirParent(const char* path, mode_t mode);
```
They are `extern "C"`-ed in C++.

### Posix implementation on macOS

On macOS, you can also use the posix implementation. It means the folders you retrieve are the same as in linux.
You can access them through the `storepaths::posix` namespace in C++ or through the functions that starts with `getPosix...` in C.
Consider this script ran on macOS:
```c++
auto [configFolder, configFolderInfo] = storepaths::getConfigFolder("Snake Game");
auto [osxconfigFolder, osxconfigFolderInfo] = storepaths::osx::getConfigFolder("Snake Game");
auto [posixconfigFolder, posixconfigFolderInfo] = storepaths::posix::getConfigFolder("Snake Game");
```
- `configFolder` should be in something like `/Users/bob/Library/Application Support/Snake Game/`.
- `osxconfigFolder` should be just like `configFolder`, as expected, because it is aliased.
- `posixconfigFolder` should be in something like `/Users/bob/.config/Snake Game/`. On linux it would be roughly the same thing.
  Great ! So you can use the posix implementation in macOS... Even better... make it the default !!!...!...?
  If add this line before you include the library:
```c++
#define STOREPATHS_USING_POSIX_IMPL_FOR_OSX
```
now `storepaths::getConfigFolder` and every other similar function now defaults to `storepaths::posix::getConfigFolder` instead of `storepaths::osx::getConfigFolder`.
This line has no effects on other platforms.

To summarize:

#### Function aliases

| C++ Function name                    | Platform   | Aliased to                                    | With posix implementation for macOS                    |
|--------------------------------------|------------|-----------------------------------------------|--------------------------------------------------------|
| `storepaths::getConfigFolder`        | Linux      | `storepaths::posix::getConfigFolder`          | Does nothing                                           |
| `storepaths::getConfigFolder`        | Windows    | `storepaths::windows::getConfigFolder`        | Does nothing                                           |
| `storepaths::getConfigFolder`        | macOS      | `storepaths::osx::getConfigFolder`            | `storepaths::posix::getConfigFolder`                   |
| `storepaths::getDataFolder`          | Linux      | `storepaths::posix::getDataFolder`            | Does nothing                                           |
| ...                                  | ...        | ...                                           | ...                                                    |
| `storepaths::getJSONConfigFile`      | macOS      | `storepaths::osx::getJSONConfigFile`          | `storepaths::posix::getJSONConfigFile`                 |
| `storepaths::getPosixConfigFile`     | Linux      | `storepaths::posix::getPosixConfigFile`       | Does nothing                                           |
| `storepaths::getWindowsConfigFile`   | Windows    | `storepaths::windows::getWindowsConfigFile`   | Does nothing                                           |
| `storepaths::getOsxConfigFile`       | macOS      | `storepaths::osx::getOsxConfigFile`           | Still aliased to `storepaths::osx::getOsxConfigFile`   |

| C Function name        | Platform | Aliased to               | With posix implementation for macOS |
|------------------------|----------|--------------------------|-------------------------------------|
| `getConfigFolder`      | Linux    | `getPosixConfigFolder`   | Does nothing                        |
| `getConfigFolder`      | Windows  | `getWindowsConfigFolder` | Does nothing                        |
| `getConfigFolder`      | macOS    | `getOsxConfigFolder`     | `getPosixConfigFolder`              |
| `getDataFolder`        | Linux    | `getPosixDataFolder`     | Does nothing                        |
| ...                    | ...      | ...                      | ...                                 |
| `getJSONConfigFile`    | macOS    | `getOsxJSONConfigFile`   | `getPosixJSONConfigFile`            |
| `getPosixConfigFile`   | Linux    | Is not an alias          | Does nothing                        |
| `getWindowsConfigFile` | Windows  | Is not an alias          | Does nothing                        |
| `getOsxConfigFile`     | macOS    | Is not an alias          | Does not change                     |

## All functions and expected behaviour
### Folder functions

These functions return folders.
The returned folder always end with the path separator char ('/' on posix systems) and is always an absolute path.
The folder returned is created with all parent folders.
If the creation fails, the path is still returned but the `mkdirReturnCode` flag in `PathInfo` is set to the return code of the mkdir function (may change on different platforms).
Currently, you can only get the folders corresponding to the current user. When you are logged as root, the function may fail.

There is 3 folder functions for now:

| C++ name                      | C name            | Posix expected result                | Windows expected result                    | Osx expected result                                  |
|-------------------------------|-------------------|--------------------------------------|--------------------------------------------|------------------------------------------------------|
| `storepaths::getConfigFolder` | `getConfigFolder` | `/home/bob/.config/<app name>/`      | `C:\Users\bob\AppData\Roaming\<app name>\` | `/Users/bob/Library/Application Support/<app name>/` |
| `storepaths::getDataFolder`   | `getDataFolder`   | `/home/bob/.local/share/<app name>/` | `C:\Users\bob\AppData\Roaming\<app name>\` | `/Users/bob/Library/Application Support/<app name>/` |
| `storepaths::getCacheFolder`  | `getCacheFolder`  | `/home/bob/.cache/<app name>/`       | `C:\Users\bob\AppData\Local\<app name>\`   | `/Users/bob/Library/Caches/<app name>/`              |

### File functions

These functions return files. The file is not created but the folder it is in is created with the same logic as folder functions.
- The JSON config file gives you a common and quick way to store information: it is a sure way to store information across platforms
- The other function changes implementation based on the platform. It implements recommenced way to use config files, according to the documentation of your system.
