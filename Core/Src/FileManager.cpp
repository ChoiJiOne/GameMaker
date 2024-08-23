#include <Shlwapi.h>

#include "Assertion.h"
#include "FileManager.h"

using namespace GameMaker;

FileManager& FileManager::Get()
{
	static FileManager instance;
	return instance;
}

void FileManager::Startup()
{
	CHECK(!bIsStartup_);

	WINDOWS_CHECK(GetCurrentDirectoryA(MAX_STRING_SIZE, workingDirectoryA_));
	WINDOWS_CHECK(GetCurrentDirectoryW(MAX_STRING_SIZE, workingDirectoryW_));

	std::strncat(workingDirectoryA_, "\\", 1);
	std::wcsncat(workingDirectoryW_, L"\\", 1);
	
	bIsStartup_ = true;
}

void FileManager::Shutdown()
{
	CHECK(bIsStartup_);

	bIsStartup_ = false;
}

std::vector<uint8_t> FileManager::ReadFile(const std::string& path)
{
	HANDLE file = ::CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	WINDOWS_ASSERT(file != INVALID_HANDLE_VALUE, "failed to open %s", path.c_str());

	DWORD fileSize = ::GetFileSize(file, nullptr);
	std::vector<uint8_t> buffer(fileSize);

	DWORD bytesRead;
	WINDOWS_ASSERT(::ReadFile(file, buffer.data(), fileSize, &bytesRead, nullptr), "failed to read %s", path.c_str());
	WINDOWS_ASSERT(::CloseHandle(file), "failed to close %s", path.c_str());

	return buffer;
}

std::vector<uint8_t> FileManager::ReadFile(const std::wstring& path)
{
	HANDLE file = ::CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	WINDOWS_ASSERT(file != INVALID_HANDLE_VALUE, L"failed to open %s", path.c_str());

	DWORD fileSize = ::GetFileSize(file, nullptr);
	std::vector<uint8_t> buffer(fileSize);

	DWORD bytesRead;
	WINDOWS_ASSERT(::ReadFile(file, buffer.data(), fileSize, &bytesRead, nullptr), L"failed to read %s", path.c_str());
	WINDOWS_ASSERT(::CloseHandle(file), L"failed to close %s", path.c_str());

	return buffer;
}