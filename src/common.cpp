#include "libcfgpath/common.h"

int mkdir_parent(const char* file, const mode_t mode) {
    return mkdir(file, mode);
}
