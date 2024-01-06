#include "libcfgpath/cfgpath.h"

#ifndef LIBCFGPATH_OS_WINDOWS
#  error Cannot build this file because you are not on Windows
#endif

#include <ShlObj.h> // SHGetKnownFolderPath
#include <optional>

#define LIBCFGPATH_CAN_INCLUDE_IMPLEMENTATION_HPP
#include "libcfgpath/implementations.hpp"
#include "libcfgpath/sizedstream.hpp"

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

	static inline std::optional<std::string> toStringfromWide(const std::wstring& inString) {
		// Get the required buffer size for the multi-byte string
		BOOL usedDefaultChar = false;
		int bufferSize = WideCharToMultiByte(
			CP_ACP, // Default code page
			0, // No flags
			inString.c_str(), // String to convert
			-1, // -1 for null-terminated strings
			nullptr, // No target string
			0, // Size of the target string
			nullptr, // No special charatcer for replacement
			&usedDefaultChar // Know if the function used a default character
		);

		if (!bufferSize || usedDefaultChar)
			return std::nullopt;

		std::string multiByteStr(bufferSize - 1, '\0');

		// Perform the actual conversion
		int result = WideCharToMultiByte(
			CP_ACP, // Same codepage as above
			0, // No flags
			inString.c_str(), // Source
			-1, // The string is null-terminated
			multiByteStr.data(), // Target
			bufferSize, // String size
			nullptr, // No spcial character
			&usedDefaultChar // Know if the function used a default character
		);

		if (!result || usedDefaultChar)
			return std::nullopt;

		return multiByteStr;
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

		auto convertedString = toStringfromWide(path);

		if (!convertedString.has_value()) {
			CoTaskMemFree(path);
			return false;
		}

		stream << convertedString.value() << PATH_SEP_CHAR;
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