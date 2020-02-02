#include "USBDevice.h"
#include "StringExt.h"
#include <Windows.h>
#include <SetupAPI.h>
using namespace std;

#define NUMBER_OF_BUS_TYPE_STRINGS (sizeof(BusTypeStrings) / sizeof(BusTypeStrings[0]))

USBDevice::USBDevice(wchar_t driveLetter) : Volume()
{
	this->driveLetter = driveLetter;
	bool res = updateInformation();
}

bool USBDevice::updateInformation()
{
	if (!Volume::updateInformation())
		return false;
	if (driveLetter == L'A' || driveLetter == L'B' || driveLetter == L'a' || driveLetter == L'b')
		return false;
	if (diskType != DRIVE_REMOVABLE)
		return false;

	wchar_t* filePath = new wchar_t[7];
	bool result;
	DWORD readed;
	STORAGE_DESCRIPTOR_HEADER *DeviceDescriptorHeader;
	STORAGE_DEVICE_DESCRIPTOR *DeviceDescriptor;
	DWORD devDescLength;
	STORAGE_PROPERTY_QUERY query;
	const wchar_t* BusTypeStrings[] = { L"Unknown",L"SCSI",L"ATAPI",L"ATA",L"1394",L"SSA",L"Fibre",L"USB",L"RAID",L"iSCSI",
			L"SAS",L"SATA",L"SD",L"MMC",L"Virtual",L"FileBackedVirtual",L"Spaces",L"NVMe",L"SCM",L"UFS",L"Max" };

	swprintf_s(filePath, 7, L"\\\\?\\%c:", driveLetter);
	HANDLE hDevice = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	
	query.PropertyId = StorageDeviceProperty;
	query.QueryType = PropertyStandardQuery;
	DeviceDescriptorHeader = (STORAGE_DESCRIPTOR_HEADER *)malloc(sizeof(STORAGE_DESCRIPTOR_HEADER));
	result = DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), DeviceDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER), &readed, NULL);
	devDescLength = DeviceDescriptorHeader->Size;
	DeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR *)malloc(devDescLength);
	result = DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), DeviceDescriptor, devDescLength, &readed, NULL);

	LPCSTR vendorId = "", productId = "", productRevision = "", serialNumber = "";
	LPCWSTR busType;

	if ((ULONG)DeviceDescriptor->BusType < NUMBER_OF_BUS_TYPE_STRINGS) {
		busType = BusTypeStrings[DeviceDescriptor->BusType];
	}
	else {
		busType = BusTypeStrings[NUMBER_OF_BUS_TYPE_STRINGS - 1];
	}

	if ((DeviceDescriptor->ProductIdOffset != 0) &&
		(DeviceDescriptor->ProductIdOffset != -1)) {
		productId = (LPCSTR)(DeviceDescriptor);
		productId += (ULONG_PTR)DeviceDescriptor->ProductIdOffset;
	}
	if ((DeviceDescriptor->VendorIdOffset != 0) &&
		(DeviceDescriptor->VendorIdOffset != -1)) {
		vendorId = (LPCSTR)(DeviceDescriptor);
		vendorId += (ULONG_PTR)DeviceDescriptor->VendorIdOffset;
	}
	if ((DeviceDescriptor->ProductRevisionOffset != 0) &&
		(DeviceDescriptor->ProductRevisionOffset != -1)) {
		productRevision = (LPCSTR)(DeviceDescriptor);
		productRevision += (ULONG_PTR)DeviceDescriptor->ProductRevisionOffset;
	}
	if ((DeviceDescriptor->SerialNumberOffset != 0) &&
		(DeviceDescriptor->SerialNumberOffset != -1)) {
		serialNumber = (LPCSTR)(DeviceDescriptor);
		serialNumber += (ULONG_PTR)DeviceDescriptor->SerialNumberOffset;
	}

	version = DeviceDescriptor->Version;
	size = DeviceDescriptor->Size;
	deviceType = DeviceDescriptor->DeviceType;
	deviceTypeModifier = DeviceDescriptor->DeviceTypeModifier;
	removableMedia = DeviceDescriptor->RemovableMedia;
	commandQueueing = DeviceDescriptor->CommandQueueing;

	wcscpy_s(this->vendorId,to_wstring(trim(vendorId)).c_str());
	wcscpy_s(this->productId, to_wstring(trim(productId)).c_str());
	wcscpy_s(this->productRevision, to_wstring(trim(productRevision)).c_str());
	wcscpy_s(this->serialNumber, to_wstring(trim(serialNumber)).c_str());
	wcscpy_s(this->busType, busType);
	
	free(DeviceDescriptorHeader);
	free(DeviceDescriptor);
	CloseHandle(hDevice);
	delete[] filePath;

	return true;
}

uint16_t USBDevice::getDeviceType() const noexcept
{
	return deviceType;
}

uint16_t USBDevice::getDeviceTypeModifier() const noexcept
{
	return deviceTypeModifier;
}

uint32_t USBDevice::getVersion() const noexcept
{
	return version;
}

uint32_t USBDevice::getSize() const noexcept
{
	return size;
}

bool USBDevice::isRemovableMedia() const noexcept
{
	return removableMedia;
}

bool USBDevice::isCommandQueueing() const noexcept
{
	return commandQueueing;
}

wstring USBDevice::getVendorId() const noexcept
{
	return vendorId;
}

wstring USBDevice::getProductId() const noexcept
{
	return productId;
}

wstring USBDevice::getProductRevision() const noexcept
{
	return productRevision;
}

wstring USBDevice::getSerialNumber() const noexcept
{
	return serialNumber;
}

wstring USBDevice::getBusType() const noexcept
{
	return busType;
}
