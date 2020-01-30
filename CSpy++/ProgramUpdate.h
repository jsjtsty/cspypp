#pragma once
#include "Version.h"
#include <string>
#include <json/json.h>

enum UpdateCode : int {
	CS_UPDATE_OK = 0,
	CS_UPDATE_CONNECT_FAILED = 1,
	CS_UPDATE_INVALID_CFGFILE = 2,
	CS_UPDATE_OLD_VERSION = 3,
	CS_UPDATE_FILE_ERROR = 4,
	CS_UPDATE_INVALID_FILE = 5,
	CS_DECOMPRESS_ERROR = 6,
	CS_UPDATE_PROGRAM_LOST = 7
};

struct UpdateResult {
	UpdateCode code;
	int64_t extraMessage;
};

class ProgramUpdate
{
public:
	struct UpdateInformation {
		bool available;
		Version currentVersion;
		Version stableVersion;
		UpdateCode code;
		int64_t extraMessage;
	};
	static ProgramUpdate* getInstance();
	UpdateInformation checkForUpdate() const;
	static const wchar_t* getErrorString(UpdateCode code);
	void autoUpdate() const;
	UpdateResult downloadUpdate(const Version::Package& package) const;
	UpdateResult update() const;
protected:
	bool checkVersion(Json::Value& currentVersion, UpdateInformation& info, Version& pver) const;
	ProgramUpdate();
};

typedef ProgramUpdate::UpdateInformation UpdateInformation;