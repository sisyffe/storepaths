#ifndef LIBCFGPATH_COMMON_H_
#define LIBCFGPATH_COMMON_H_

#ifdef __cplusplus
#define LIBCFGPATH_CPP_CHECK() \
    extern "C" {
#define LIBCFGPATH_CPP_CHECK_END() \
    } // extern "C"
#else
#define LIBCFGPATH_CPP_CHECK()
#define LIBCFGPATH_CPP_CHECK_END()
#endif

LIBCFGPATH_CPP_CHECK()

enum Folders {
    CONFIG_FOLDER,
    DATA_FOLDER,
    CACHE_FOLDER
};

enum Files {
    CONFIG_FILE
};

LIBCFGPATH_CPP_CHECK_END()

#endif //LIBCFGPATH_COMMON_H_
