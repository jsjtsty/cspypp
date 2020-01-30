#include "general.h"
#include <memory>
#include <Windows.h>
#include "FileLister.h"
#include "filemanager.h"
using namespace std;

namespace {
	Directory* ListDirectory(Directory* directory)
	{
		
	}
}

namespace FileLister {
	DirectoryPtr list_directory(const wstring_view path)
	{
		DirectoryPtr pDir = nullptr;

		WIN32_FIND_DATA directoryData;
		wstring_view directoryPath;
		if (path[path.length() - 1] == L'/' || path[path.length() - 1] == L'\\') {
			directoryPath = path.substr(0, path.length() - 1);
		}
		else {
			directoryPath = path;
		}

		HANDLE handle = FindFirstFileW(directoryPath.data(), &directoryData);

		if (!handle) {
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
}