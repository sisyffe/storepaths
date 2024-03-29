# The `storepaths` library

## Introduction

This **cross-platform** **cross-language** library allows you to retrieve common application folders location.
In these folders you can store application-created data.
For example, you can retrieve the configuration folder, the "data" folder, or the cache folder for the current user.
To use this library, **there is no complicated platform-checking operations** to do, you just need to include a single header file:
```c++
#include <storepaths/storepaths.h>
```

## Compatibility

Available platforms:

| Platform | Status    |  
|----------|-----------|  
| macOS    | ✅ Runs    |  
| Windows  | ❓ Unknown |  
| Linux    | ❓ Unknown |

The library can be used both in C and C++ with the same header file.

## Examples
### C++

```c++
#include <iostream>
#include <storepaths/storepaths.h>

#define APPNAME "Snake Game"

int main() {
    auto [dataFolder, dataFolderInfo] = storepaths::getDataFolder(APPNAME);
    if (!dataFolderInfo.succeeded) {
        // Error handling
    }

    std::cout << "Data folder: " << dataFolder << std::endl;
    return 0;
}
```

### C

```c++
#include <stdio.h>
#include <storepaths/storepaths.h>

#define APPNAME "Snake Game"

int main(void) {
    char dataFolder[MAX_PATH_LENGTH]; // MAX_PATH_LENGTH is from the library
    PathInfo dataFolderInfo = getDataFolder(dataFolder, sizeof(dataFolder), APPNAME);
    if (!dataFolderInfo.succeeded) {
        // Error handling
    }

    printf("Data folder: %s", dataFolder);
    return 0;
}
```

The output may be:
- on macOS (OSX): `/Users/bob/Library/Application Support/Snake Game/config/`,
- on Windows: `C:\Users\bob\AppData\Roaming\Snake Game\config\`,
- on linux systems: `/home/bob/.config/Snake Game/`.

The folder is created on the disk if it doesn't exist when the function is called.
Every function returns an absolute path.

## How to use the library
### Build it yourself

The build process is fairly simple: it is a standard CMake project.
1. Clone the project or download it
```sh
git clone https://github.com/sisyffe/storepaths.gitcd storepaths
```  
2. Check that you have CMake installed. If the following command fails [install cmake](https://cmake.org/download/):
```sh
cmake --version
```
3. Configure the project
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```
4. Build the project. If you get any errors there, good luck ! (more seriously [post an issue](https://docs.github.com/en/issues/tracking-your-work-with-issues/creating-an-issue))
```sh
cmake --build build
```
5. Extract the files
- The static library is at `build/libstorepaths.a`
- The include directory is at `include/storepaths`.

### Other ways
- Add it to your git repository a **submodule** and add it as a subdirectory of your project
  Your CMakeLists.txt file:
```cmake
# ...
add_subdirectory(storepaths)
target_link_libraries(your-project PRIVATE storepaths)
# ...
```
- You may want to install it as a package. Currently, it is **not available in any package manager** (e.g. [Homebrew](https://brew.sh/), [APT](https://en.wikipedia.org/wiki/APT_(software))...). Adding and install feature through CMake is planned.
