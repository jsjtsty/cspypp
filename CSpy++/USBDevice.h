#pragma once
#include <string>
#include "Volume.h"

class USBDevice : public Volume
{
public:
	USBDevice() : Volume() {}
	USBDevice(wchar_t driveLetter);

	virtual bool updateInformation() override;

	uint16_t getDeviceType() const noexcept;
	uint16_t getDeviceTypeModifier() const noexcept;
	uint32_t getVersion() const noexcept;
	uint32_t getSize() const noexcept;
	bool isRemovableMedia() const noexcept;
	bool isCommandQueueing() const noexcept;
	std::wstring getVendorId() const noexcept;
	std::wstring getProductId() const noexcept;
	std::wstring getProductRevision() const noexcept;
	std::wstring getSerialNumber() const noexcept;
	std::wstring getBusType() const noexcept;

protected:
	uint16_t deviceType = 0, deviceTypeModifier = 0;
	uint32_t version = 0, size = 0;
	bool removableMedia = false, commandQueueing = false;
	wchar_t vendorId[32] = { 0 }, productId[32] = { 0 }, productRevision[32] = { 0 }, serialNumber[32] = { 0 }, busType[32] = { 0 };
};