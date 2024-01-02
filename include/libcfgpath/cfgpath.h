#ifndef LIBCFGPATH_CFGPATH_H_
#define LIBCFGPATH_CFGPATH_H_

#include <stdlib.h>

#ifdef __cplusplus
#include <string>
#endif

#include "libcfgpath/common.h"

#ifdef __cplusplus
std::string getConfigFolder(const std::string& appName);
std::string getDataFolder(const std::string& appName);
std::string getCacheFolder(const std::string& appName);
std::string getConfigFile(const std::string& appName);

std::string getFolder(Folders folderType, const std::string& appName);
std::string getFile(Files fileType, const std::string& appName);
#endif

LIBCFGPATH_CPP_CHECK()

void getConfigFolder(char* outBuffer, size_t maxLength, const char* appName);
void getDataFolder(char* outBuffer, size_t maxLength, const char* appName);
void getCacheFolder(char* outBuffer, size_t maxLength, const char* appName);
void getConfigFile(char* outBuffer, size_t maxLength, const char* appName);

void getFolder(enum Folders folderType, char* outBuffer, size_t maxLength, const char* appName);
void getFile(enum Files fileType, char* outBuffer, size_t maxLength, const char* appName);

LIBCFGPATH_CPP_CHECK_END()

#endif //LIBCFGPATH_CFGPATH_H_
