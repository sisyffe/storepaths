#include "storepaths/storepaths.h"

#ifndef STOREPATHS_OS_WINDOWS
#  error Cannot build this file because you are not on Windows
#endif

#include <ShlObj.h> // SHGetKnownFolderPath
#include <optional>

#define STOREPATHS_CAN_INCLUDE_IMPLEMENTATION_HPP
#include "storepaths/implementations.hpp"
#include "storepaths/sizedstream.hpp"

#define STOREPATHS_WINDOWS_RETURN(returnVal) \
	CoTaskMemFree(path); \
	return returnVal;

namespace storepaths::windows {
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

		[[nodiscard]] inline bool initialized() const { return isInitialized; }
	};

	static COMInitializer comInitializer;

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
		const int result = WideCharToMultiByte(
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

	static inline bool getSpecificFolder(SizedStream& stream, const KNOWNFOLDERID& id) {
		if (!comInitializer.initialized())
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
		getSpecificFolder(result, FOLDERID_RoamingAppData), // AppData\Roaming
		appName << PATH_SEP_CHAR
	)

	DEFINE_GET_FOLDER_FUNCTION(
		getDataFolder,
		getSpecificFolder(result, FOLDERID_RoamingAppData), // AppData\Roaming
		appName << PATH_SEP_CHAR
	)

	DEFINE_GET_FOLDER_FUNCTION(
		getCacheFolder,
		getSpecificFolder(result, FOLDERID_LocalAppData), // AppData\Local
		appName << PATH_SEP_CHAR
	)

	DEFINE_GET_COMMON_FILE_FUNCTION(
		getCommonConfigFile,
		getSpecificFolder(result, FOLDERID_RoamingAppData), // AppData\Roaming
		appName << PATH_SEP_CHAR,
		fileName.value_or(appName) << '.' << extension
	)

	DEFINE_GET_FILE_FUNCTION(
		getWindowsConfigFile,
		getSpecificFolder(result, FOLDERID_RoamingAppData), // AppData\Roaming
		appName << PATH_SEP_CHAR,
		fileName.value_or(appName) << STOREPATHS_WINDOWS_CONFIG_EXTENSION
	)
} // storepaths::windows