/*
 * The file is part of WitchEngine.
 * Copyright (C) 2012 The Team Entertainment
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _WIN32_WINNT 0x0501

#include <WitchCore/String.hpp>
#
#include "FileImpl.hpp"

namespace WitchEngine
{
	namespace Core
	{
		void FileImpl::close()
		{
			CloseHandle(_handle);
		}
		
		bool FileImpl::atEnd() const
		{
			LARGE_INTEGER fileSize;
			if(!GetFileSizeEx(_handle, &fileSize))
				fileSize.QuadPart = 0;
				
			return (getCursorPos() >= static_cast<uint64>(fileSize.QuadPart));
		}
		
		uint64 FileImpl::getCursorPos() const
		{
			LARGE_INTEGER zero;
			zero.QuadPart = 0;
			
			LARGE_INTEGER position;
			SetFilePointerEx(_handle, zero, &position, FILE_CURRENT);
			
			return position.QuadPart;
		}
		
		bool FileImpl::open(const String &filename, OpenMode mode)
		{
			DWORD accessFlags;
			DWORD shareModeFlags;
			DWORD openModeFlags;
			
			if(mode & ReadOnly)
			{
				accessFlags = GENERIC_READ;
				openModeFlags = OPEN_EXISTING;
			}
			else if(mode & ReadWrite)
			{
				if(mode & Append)
					accessFlags = FILE_APPEND_DATA;
				else
					accessFlags = GENERIC_READ | GENERIC_WRITE;
					
				if(mode & Truncate)
					openModeFlags = CREATE_ALWAYS;
				else
					openModeFlags = OPEN_ALWAYS;
			}
			else if(mode & WriteOnly)
			{
				if(mode & Append)
					accessFlags = FILE_APPEND_DATA;
				else
					accessFlags = GENERIC_WRITE;
					
				if(mode & Truncate)
					openModeFlags = CREATE_ALWAYS;
				else
					openModeFlags = OPEN_ALWAYS;
			}
			else
				return false;
				
			wchar_t *path;
			_handle = CreateFileW(L"", accessFlags, shareModeFlags, nullptr, openModeFlags, 0, nullptr);
			
			return _handle != INVALID_HANDLE_VALUE;
		}
		
		bool FileImpl::flush()
		{
			return FlushFileBuffers(_handle);
		}
		
		std::size_t FileImpl::read(char *buffer, std::size_t size)
		{
			DWORD read = 0;
			if(ReadFile(_handle, buffer, size, &read, nullptr))
			{
				return read;
			}
			else
				return 0;
		}
		
		std::size_t FileImpl::write(const char *buffer, std::size_t size)
		{
			DWORD written = 0;
			
			LARGE_INTEGER cursorPos;
			cursorPos.QuadPart = getCursorPos();
			
			LockFile(_handle, cursorPos.LowPart, cursorPos.HighPart, size, 0);
			WriteFile(_handle, buffer, size, &written, nullptr);
			UnlockFile(_handle, cursorPos.LowPart, cursorPos.HighPart, size, 0);
			
			return written;
		}
		
		bool FileImpl::copy(const String &sourcePath, const String &targetPath)
		{
			wchar_t* path;
			wchar_t* newPath;
			bool success = CopyFileW(path, newPath, false);
			
			if(success)
				return true;
			else
				return false;
		}
		
		bool FileImpl::remove(const String &filepath)
		{
			wchar_t *path;
			bool success = DeleteFileW(path);
			
			if(success)
				return true;
			else
				return false;
		}
		
		bool FileImpl::exists(const String &filePath)
		{
			wchar_t *path;
			HANDLE handle = CreateFileW(path, 
										0,
										FILE_SHARE_READ | FILE_SHARE_WRITE,
										nullptr,
										OPEN_EXISTING,
										0,
										nullptr);
										
			if(handle == INVALID_HANDLE_VALUE)
				return false;
				
			CloseHandle(handle);
			
			return true;
		}
		
		std::time_t FileImpl::creationTime(const String &filePath)
		{
			wchar_t *path;
			HANDLE handle = CreateFileW(path,
										0,
										FILE_SHARE_READ | FILE_SHARE_WRITE,
										nullptr, 
										OPEN_EXISTING,
										0,
										nullptr);
										
			if(handle == INVALID_HANDLE_VALUE)
				return 0;
				
			FILETIME creationTime;
			if(!GetFileTime(handle, &creationTime, nullptr, nullptr))
			{
				CloseHandle(handle);
				return 0;
			}
			
			CloseHandle(handle);
			
			return 0;
		}
		
		std::time_t FileImpl::lastAccessTime(const String &filePath)
		{
			wchar_t *path;
			HANDLE handle = CreateFileW(path,
										0,
										FILE_SHARE_READ,
										nullptr,
										OPEN_EXISTING,
										0,
										nullptr);
										
			if(handle == INVALID_HANDLE_VALUE)
				return 0;
				
			FILETIME accessTime;
			if(!GetFileTime(handle, nullptr, &accessTime, nullptr))
			{
				CloseHandle(handle);
				return 0;
			}
			
			CloseHandle(handle);
			
			return 0;
		}
		
		std::time_t FileImpl::lastWriteTime(const String &filePath)
		{
			wchar_t *path;
			HANDLE handle = CreateFileW(path, 
										0, 
										FILE_SHARE_READ, 
										nullptr, 
										OPEN_EXISTING, 
										0, 
										nullptr);
			
			if(handle == INVALID_HANDLE_VALUE)
				return 0;
				
			FILETIME writeTime;
			if(!GetFileTime(handle, nullptr, nullptr, &writeTime))
			{
				CloseHandle(handle);
				
				return 0;
			}
			
			CloseHandle(handle);
			
			return 0;
		}
		
		uint64 FileImpl::size(const String &filePath)
		{
			wchar_t *path;
			HANDLE handle = CreateFileW(path,
										0,
										FILE_SHARE_READ,
										nullptr,
										OPEN_EXISTING,
										0,
										nullptr);
										
			if(handle == INVALID_HANDLE_VALUE)
				return 0;
				
			LARGE_INTEGER fileSize;
			if(!GetFileSizeEx(handle, &fileSize))
				fileSize.QuadPart = 0;
				
			CloseHandle(handle);
			
			return fileSize.QuadPart;
		}
		
		bool FileImpl::rename(const String &sourcePath, const String &targetPath)
		{
			wchar_t *path;
			wchar_t *newPath;
			
			bool success = MoveFileExW(path, newPath, MOVEFILE_COPY_ALLOWED) != 0;
			
			if(success)
				return true;
			else
				return false;
		}
	}
}