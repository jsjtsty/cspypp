#pragma once
#include <string>
#include "Volume.h"

class USBDevice : Volume
{
public:
	USBDevice() : Volume() {}
	USBDevice(wchar_t driveLetter);

	virtual bool updateInformation() override;

	inline uint16_t getDeviceType() const noexcept;
	inline uint16_t getDeviceTypeModifier() const noexcept;
	inline uint32_t getVersion() const noexcept;
	inline uint32_t getSize() const noexcept;
	inline bool isRemovableMedia() const noexcept;
	inline bool isCommandQueueing() const noexcept;
	inline std::wstring getVendorId() const noexcept;
	inline std::wstring getProductId() const noexcept;
	inline std::wstring getProductRevision() const noexcept;
	inline std::wstring getSerialNumber() const noexcept;
	inline std::wstring getBusType() const noexcept;

protected:
	uint16_t deviceType = 0, deviceTypeModifier = 0;
	uint32_t version = 0, size = 0;
	bool removableMedia = false, commandQueueing = false;
	std::wstring vendorId, productId, productRevision, serialNumber, busType;
};