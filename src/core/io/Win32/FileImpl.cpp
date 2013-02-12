/*
 * The file is part of WitchEngine.
 * Copyright (C) 2012 The Team Entertainment
 * Contact: http://www.theteamentertainment.com/license
 *
 * Commercial License Usage
 * Licensees holding valid WitchEngine licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and The Team Entertainment. For licensing terms and
 * conditions see http://www.theteamentertainment.com/licensing. For further information
 * use the contact from at http://www.theteamentertainment.com/contact-us
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published be the Free Software
 * Foundation and appearing in the file COPYING included in the
 * packaging of the file. Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file COPYING.LESSER includd in the
 * packaging of this file. Please review the following information to
 * ensure the GNU Lesser General Public Lecense version 3.0 requirements
 * will be met: http://www.gnu.org/copyleft/lesser.html
 *
*/

#include "../File.hpp"
#
#include "FileImpl.hpp"
#include "../../kernel/Win32/Time.hpp"
#
#include <cstring>

namespace WitchEngine
{
	namespace Core
	{
		void FileImpl::close()
		{
			CloseHandle(_handle);
		}
		
		bool FileImpl::endOfFile() const
		{
			LARGE_INTEGER fileSize;
			if(!GetFileSizeEx(_handle, &fileSize))
				fileSize.QuadPart = 0;
				
			return cursorPos() >= static_cast<uint64>(fileSize.QuadPart);
		}
		
		void FileImpl::flush()
		{
			if(!FlushFileBuffers(_handle))
			{
				// TODO: Throw an exception.
			}
		}
		
		uint64 FileImpl::cursorPos() const
		{
			LARGE_INTEGER zero;
			zero.QuadPart = 0;
			
			LARGE_INTEGER position;
			SetFilePointerEx(_handle, zero, &position, FILE_CURRENT);
			
			return position.QuadPart;
		}
		
		bool FileImpl::open(const String &filePath, File::OpenMode mode)
		{
			DWORD access;
			DWORD openMode;
			
			if(mode & File::ReadOnly)
			{
				access = GENERIC_READ;
				openMode = OPEN_EXISTING;
			}
			else if(mode & File::ReadWrite)
			{
				if(mode & File::Append)
					access = FILE_APPEND_DATA;
				else
					access = GENERIC_READ | GENERIC_WRITE;
				
				if(mode & File::Truncate)
					openMode = CREATE_ALWAYS;
				else
					openMode = OPEN_ALWAYS;
			}
			else if(mode & File::WriteOnly)
			{
				if(mode &File::Append)
					access = FILE_APPEND_DATA;
				else
					access = GENERIC_WRITE;
					
				if(mode & File::Truncate)
					openMode = CREATE_ALWAYS;
				else
					openMode = OPEN_ALWAYS;
			}
			else
				return false;
				
			wchar_t *path = filePath.wideBuffer();;
			_handle = CreateFileW(path, access, FILE_SHARE_READ, nullptr, openMode, 0, nullptr);
			delete[] path;
			
			return _handle != INVALID_HANDLE_VALUE;
		}
		
		uint64 FileImpl::read(char *buffer, uint64 size)
		{
			DWORD read = 0;
			if(ReadFile(_handle, buffer, size, &read, nullptr))
			{
				return read;
			}
			else
				return 0;
		}
		
		char* FileImpl::read(uint64 size)
		{
			char *buffer;
			if(ReadFile(_handle, buffer, size, nullptr, nullptr))
			{
				return buffer;
			}
			else
			{
				delete buffer;
				return nullptr;
			}
		}
		
		bool FileImpl::setCursorPos(File::CursorPosition pos, uint64 offset)
		{
			DWORD moveMethod;
			switch(pos)
			{
				case File::AtBegin:
					moveMethod = FILE_BEGIN;
					break;
					
				case File::AtCurrent:
					moveMethod = FILE_CURRENT;
					break;
					
				case File::AtEnd:
					moveMethod = FILE_END;
					break;
					
				default:
					// TODO: Throw an exception.
					return false;
			}
			
			LARGE_INTEGER distance;
			distance.QuadPart = offset;
			
			return SetFilePointerEx(_handle, distance, nullptr, moveMethod) != 0;
		}
		
		uint64 FileImpl::write(const char *buffer, uint64 maxSize)
		{
			DWORD written = 0;
			
			LARGE_INTEGER pos;
			pos.QuadPart = cursorPos();
			
			LockFile(_handle, pos.LowPart, pos.HighPart, maxSize, 0);
			WriteFile(_handle, buffer, maxSize, &written, nullptr);
			UnlockFile(_handle, pos.LowPart, pos.HighPart, maxSize, 0);
			
			return written;
		}
		
		uint64 FileImpl::write(const char *buffer)
		{
			DWORD written = 0;
			
			LARGE_INTEGER pos;
			pos.QuadPart = cursorPos();
			
			std::size_t size = std::strlen(buffer);
			
			LockFile(_handle, pos.LowPart, pos.HighPart, size, 0);
			WriteFile(_handle, buffer, size, &written, nullptr);
			UnlockFile(_handle, pos.LowPart, pos.HighPart, size, 0);
		}
		
		bool FileImpl::copy(const String &sourcePath, const String &targetPath)
		{
			wchar_t *path = sourcePath.wideBuffer();
			wchar_t *newPath = targetPath.wideBuffer();
			bool success = CopyFileW(path, newPath, false);
			delete[] path;
			delete[] newPath;
			
			if(success)
				return true;
			else
			{
				// TODO: Throw an exception.
				return false;
			}
		}
		
		bool FileImpl::remove(const String &filePath)
		{
			wchar_t *path = filePath.wideBuffer();;
			bool success = DeleteFileW(path);
			delete[] path;
			
			if(success)
				return true;
			else
			{
				// TODO: Throw an exception.
				return false;
			}
		}
		
		bool FileImpl::exists(const String &filePath)
		{
			wchar_t *path = filePath.wideBuffer();
			HANDLE handle = CreateFileW(path, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
			delete[] path;
			
			if(handle == INVALID_HANDLE_VALUE)
				return false;
				
			CloseHandle(handle);
			
			return true;
		}
		
		std::time_t FileImpl::creationTime(const String &filePath)
		{
			wchar_t *path = filePath.wideBuffer();
			HANDLE handle = CreateFileW(path, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
			delete[] path;
			
			if(handle == INVALID_HANDLE_VALUE)
				return 0;
				
			FILETIME creationTime;
			if(!GetFileTime(handle, &creationTime, nullptr, nullptr))
			{
				// TODO: Throw an exception.
				
				CloseHandle(handle);
				return 0;
			}
			
			CloseHandle(handle);
			
			return FileTimeToTime(&creationTime);
		}
		
		std::time_t FileImpl::lastAccessTime(const String &filePath)
		{
			wchar_t *path = filePath.wideBuffer();
			HANDLE handle = CreateFileW(path, 0, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
			delete[] path;
			
			if(handle == INVALID_HANDLE_VALUE)
				return 0;
				
			FILETIME accessTime;
			if(!GetFileTime(handle, nullptr, &accessTime, nullptr))
			{
				// TODO: Throw an exception.
				
				CloseHandle(handle);
				return 0;
			}
			
			CloseHandle(handle);
			
			return FileTimeToTime(&accessTime);
		}
		
		std::time_t FileImpl::lastWriteTime(const String &filePath)
		{
			wchar_t *path = filePath.wideBuffer();
			HANDLE handle = CreateFileW(path, 0, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
			delete[] path;
			
			if(handle == INVALID_HANDLE_VALUE)
				return 0;
				
			FILETIME writeTime;
			if(!GetFileTime(handle, nullptr, nullptr, &writeTime))
			{
				// TODO: Throw an exception.
				
				CloseHandle(handle);
				return 0;
			}
			
			CloseHandle(handle);
			
			return FileTimeToTime(&writeTime);
		}
		
		uint64 FileImpl::size(const String &filePath)
		{
			wchar_t *path = filePath.wideBuffer();
			HANDLE handle = CreateFileW(path, 0, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
			delete[] path;
			
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
			wchar_t *path = sourcePath.wideBuffer();
			wchar_t *newPath = targetPath.wideBuffer();
			
			bool success = MoveFileExW(path, newPath, MOVEFILE_COPY_ALLOWED) != 0;
			
			delete[] path;
			delete[] newPath;
			
			if(success)
				return true;
			else
			{
				// TODO: Throw an exception.
				return false;
			}
		}
	}
}