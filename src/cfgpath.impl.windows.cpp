#include "libcfgpath/cfgpath.h"

#ifdef LIBCFGPATH_OS_WINDOWS

#include <ShlObj.h> // SHGetKnownFolderPath

#include <sstream>
#include <codecvt> // convert wide strings

struct COMInitializer {
private:
	bool isInitialized;
public:
	COMInitializer() {
		isInitialized = SUCCEEDED(CoInitialize(NULL)) ? true : false;
	}

	~COMInitializer() {
		CoUninitialize();
	}

	inline bool initialized() const { return isInitialized; }
};

static COMInitializer initializer;

static inline std::string toString(const PWSTR& inString) {
	using convertType = std::codecvt_utf8<WCHAR>;
	static std::wstring_convert<convertType, WCHAR> converter;
	return converter.to_bytes(inString);
}

static inline bool getFolder(std::ostringstream& stream, const KNOWNFOLDERID& id, const std::string& appName) {
	// This function does not create the folder it returns

	if (!initializer.initialized())
		return false;

	PWSTR path = nullptr;
	HRESULT hr = SHGetKnownFolderPath( // API function
		id, // Caller defines it
		KF_FLAG_DEFAULT, // No flags (=0)
		NULL, // Current user
		&path // To `path` string
	);

	if (FAILED(hr)) {
		CoTaskMemFree(path);
		return false;
	}

	stream << toString(path) << PATH_SEP_CHAR;
	CoTaskMemFree(path);
	return true;
}

#define DEFINE_GET_FILE_FOLDER_FUNCTION(funcName, funcCalled, custom) \
    std::string funcName(const std::string& appName) { \
        std::ostringstream result; \
        \
        if (!funcCalled) \
            return ""; \
        \
        result << custom; \
        mkdirParent(result.str().c_str(), MKDIR_MODE); \
		\
		return result.str(); \
	}

DEFINE_GET_FILE_FOLDER_FUNCTION(
	getConfigFolder,
	getFolder(result, FOLDERID_RoamingAppData, appName), // AppData\Roaming
	appName << PATH_SEP_CHAR << LIBCFGPATH_WINDOWS_CONFIG_FOLDER << PATH_SEP_CHAR
)

DEFINE_GET_FILE_FOLDER_FUNCTION(
	getDataFolder,
	getFolder(result, FOLDERID_RoamingAppData, appName), // AppData\Roaming
	appName << PATH_SEP_CHAR << LIBCFGPATH_WINDOWS_DATA_FOLDER << PATH_SEP_CHAR
)

DEFINE_GET_FILE_FOLDER_FUNCTION(
	getCacheFolder,
	getFolder(result, FOLDERID_LocalAppData, appName), // AppData\Local
	appName << PATH_SEP_CHAR
)

DEFINE_GET_FILE_FOLDER_FUNCTION(
	getConfigFile,
	getFolder(result, FOLDERID_RoamingAppData, appName), // AppData\Roaming
	appName << CONFIG_EXTENSION
)

#endif //LIBCFGPATH_OS_WINDOWS