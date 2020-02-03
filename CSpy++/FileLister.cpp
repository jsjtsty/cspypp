#include "general.h"
#include <memory>
#include <Windows.h>
#include "FileLister.h"
#include "filemanager.h"
using namespace std;

namespace {
	Directory* ListDirectory(Directory* directory)
	{
		WIN32_FIND_DATA findData;
		wstring findPattern = directory->getPath() + L"/*.*";
		HANDLE hFind = FindFirstFileW(findPattern.c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE) {
			return directory;
		}
		do {
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (findData.cFileName[0] != L'.') {
					Directory* newDir = new Directory(findData, directory->getPath() + L"/" + findData.cFileName);
					directory->addDirectory(newDir);
					ListDirectory(newDir);
				}
			}
			else {
				File* newFile = new File(findData, directory->getPath() + L"/" + findData.cFileName);
				directory->addFile(newFile);
			}
		} while (FindNextFileW(hFind, &findData));
		FindClose(hFind);
		return directory;
	}
}

namespace FileLister
{
	DirectoryPtr list_directory(wstring_view path)
	{
		DirectoryPtr pDir = nullptr;

		WIN32_FIND_DATA directoryData = { 0 };
		wstring_view directoryPath;

		if (path[path.length() - 1] == L'/' || path[path.length() - 1] == L'\\') {
			directoryPath = path.substr(0, path.length() - 1);
		}
		else {
			directoryPath = path;
		}

		if (directoryPath[directoryPath.length() - 1] != L':') {
			HANDLE handle = FindFirstFileW(directoryPath.data(), &directoryData);
			if (handle == INVALID_HANDLE_VALUE) {
				return nullptr;
			}

			if (!(directoryData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				bool flag = false;
				while (FindNextFileW(handle, &directoryData)) {
					if (directoryData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					return nullptr;
				}
			}

			pDir = make_shared<Directory>(directoryData, directoryPath.data());
		}
		else {
			pDir = make_shared<Directory>();
			pDir->setPath(directoryPath);
		}

		ListDirectory(pDir.get());

		return pDir;
	}

	VolumeDirectoryPtr list_volume(wchar_t driveLetter)
	{
		wchar_t path[] = { driveLetter,L':',L'\0' };
		VolumeDirectoryPtr dir = make_shared<VolumeDirectory>(driveLetter);
		ListDirectory(dir.get());
		return dir;
	}

	USBDirectoryPtr list_usb(wchar_t driveLetter)
	{
		wchar_t path[] = { driveLetter,L':',L'\0' };
		USBDirectoryPtr dir = make_shared<USBDirectory>(driveLetter);
		ListDirectory(dir.get());
		return dir;
	}

	USBDirectoryPtr list_usb(const USBDevice& usb)
	{
		wchar_t path[] = { usb.getDriveLetter(),L':',L'\0' };
		USBDirectoryPtr dir = make_shared<USBDirectory>(usb);
		ListDirectory(dir.get());
		return dir;
	}
}