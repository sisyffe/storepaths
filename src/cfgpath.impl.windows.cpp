#include "libcfgpath/cfgpath.h"

#ifndef LIBCFGPATH_OS_WINDOWS
#  error Cannot build this file because you are not on Windows
#endif

#include <ShlObj.h> // SHGetKnownFolderPath

#include <codecvt> // convert wide strings

namespace libcfgpath {
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

	static inline std::string toStringfromWide(const PWSTR& inString) {
		using convertType = std::codecvt_utf8<WCHAR>;
		static std::wstring_convert<convertType, WCHAR> converter;
		return converter.to_bytes(inString);
	}

	static inline bool getFolder(SizedStream& stream, const KNOWNFOLDERID& id, const std::string& appName) {
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

		stream << toStringfromWide(path) << PATH_SEP_CHAR;
		CoTaskMemFree(path);
		return true;
	}

	DEFINE_GET_FOLDER_FUNCTION(
		getConfigFolder,
		getFolder(result, FOLDERID_RoamingAppData, appName), // AppData\Roaming
		appName << PATH_SEP_CHAR << LIBCFGPATH_WINDOWS_CONFIG_FOLDER << PATH_SEP_CHAR
	)

	DEFINE_GET_FOLDER_FUNCTION(
		getDataFolder,
		getFolder(result, FOLDERID_RoamingAppData, appName), // AppData\Roaming
		appName << PATH_SEP_CHAR << LIBCFGPATH_WINDOWS_DATA_FOLDER << PATH_SEP_CHAR
	)

	DEFINE_GET_FOLDER_FUNCTION(
		getCacheFolder,
		getFolder(result, FOLDERID_LocalAppData, appName), // AppData\Local
		appName << PATH_SEP_CHAR
	)

	DEFINE_GET_FILE_FUNCTION(
		getConfigFile,
		getFolder(result, FOLDERID_RoamingAppData, appName), // AppData\Roaming
		appName << CONFIG_EXTENSION
	)
}