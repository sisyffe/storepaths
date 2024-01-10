#ifndef LIBGRADES_IMPLEMENTATIONS_HPP_
#define LIBGRADES_IMPLEMENTATIONS_HPP_

#ifndef LIBCFGPATH_CAN_INCLUDE_IMPLEMENTATION_HPP
#  error Cannot include implementations.hpp. It is only meant for internal usage.
#endif

#define DEFINE_GET_FOLDER_FUNCTION(funcName, funcCalled, custom) \
    std::pair<std::string, PathInfo> funcName(const std::string& appName) { \
        SizedStream result{ MAX_PATH_LENGTH }; \
        \
        if (!funcCalled) /* the function failed to retrieve the path */ \
            return { "", { false, 0, false, false } } ; \
        \
        result << custom; \
        int mkdirCode = mkdirParent(result.readBuffer(), LIBCFGPATH_MKDIR_MODE); \
        \
        return { result.toString(), { true, mkdirCode, false, mkdirCode == 0} } ; \
    }

#define DEFINE_GET_FILE_FUNCTION(funcName, funcCalled, customForFolder, cutsomForFile) \
    std::pair<std::string, PathInfo> funcName(const std::string& appName, const std::optional<std::string>& fileName) { \
        SizedStream result{ MAX_PATH_LENGTH }; \
        \
        if (!funcCalled) /* the function failed to retrieve the path */ \
            return { "", { false, 0, false, false } } ; \
        \
        result << customForFolder; \
        int mkdirCode = mkdirParent(result.readBuffer(), LIBCFGPATH_MKDIR_MODE); \
        result << cutsomForFile; \
        \
        return { result.toString(), { true, mkdirCode, false, mkdirCode == 0} } ; \
    }

#endif //LIBGRADES_IMPLEMENTATIONS_HPP_
