#ifndef LIBCFGPATH_CFGPATH_H_
#define LIBCFGPATH_CFGPATH_H_

#include <stdlib.h>

#include "libcfgpath/common.h"

LIBCFGPATH_CPP_CHECK()

void getConfigFolder(char* outBuffer, size_t maxLength, const char* appName);
void getDataFolder(char* outBuffer, size_t maxLength, const char* appName);
void getCacheFolder(char* outBuffer, size_t maxLength, const char* appName);
void getConfigFile(char* outBuffer, size_t maxLength, const char* appName);

LIBCFGPATH_CPP_CHECK_END()

#endif //LIBCFGPATH_CFGPATH_H_
