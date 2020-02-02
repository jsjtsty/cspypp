#pragma once
#include <cstdint>
#include "resource.h"

class LicenseReader
{
public:
	enum class LicenseType : uint16_t {
		UNKONWN = 0,
		CSPYPP = IDL_CSPYPP,
		CURL = IDL_CURL,
		JSONCPP = IDL_JSONCPP,
		LIBSSH2 = IDL_LIBSSH2,
		LOG4CPLUS = IDL_LOG4CPLUS,
		OPENSSL = IDL_OPENSSL,
		ZLIB = IDL_ZLIB
	};

	LicenseReader(LicenseType licenseType);

	void* lockResource();
	void* getResource();
	void releaseResoure();

protected:
	void* hResource = nullptr;
	void* resourceMemory = nullptr;
};

