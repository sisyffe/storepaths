#ifndef LIBGRADES_IMPLEMENTATIONS_HPP_
#define LIBGRADES_IMPLEMENTATIONS_HPP_

#ifndef LIBCFGPATH_CAN_INCLUDE_IMPLEMENTATION_HPP
#  error Cannot include implementations.hpp. It is only meant for internal usage.
#endif

#define DEFINE_GET_FOLDER_FUNCTION(funcName, funcCalled, custom) \
    std::pair<std::string, bool> funcName(const std::string& appName) { \
        SizedStream result{ MAX_PATH_LENGTH }; \
        \
        if (!funcCalled) \
            return { "", false } ; \
        \
        result << custom; \
        if (mkdirParent(result.readBuffer(), MKDIR_MODE) != 0) \
            return { result.toString(), false }; \
        \
        return { result.toString(), true } ; \
    }

// The difference is that there is no mkdir for this
#define DEFINE_GET_FILE_FUNCTION(funcName, funcCalled, custom) \
    std::string funcName(const std::string& appName) { \
        SizedStream result{ MAX_PATH_LENGTH }; \
        \
        if (!funcCalled) \
            return "" ; \
        \
        result << custom; \
        \
        return result.toString() ; \
    }

#endif //LIBGRADES_IMPLEMENTATIONS_HPP_
