#pragma once
#include <string>
#include "Volume.h"

class USBDevice : Volume
{
public:
	USBDevice() : Volume() {}
	USBDevice(wchar_t driveLetter);

	virtual bool updateInformation() override;

protected:
	uint16_t deviceType = 0, deviceTypeModifier = 0;
	uint32_t version = 0, size = 0;
	bool removableMedia = false, commandQueueing = false;
	std::wstring vendorId, productId, productRevision, serialNumber, busType;
};