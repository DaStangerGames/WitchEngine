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

#include "DirectoryImpl.hpp"

namespace WitchEngine
{
	namespace Core
	{
		void DirectoryImpl::close()
		{
			FindClose(_handle);
		}
		
		String DirectoryImpl::resultName() const
		{
			return String::unicode(_result.cFileName);
		}
		
		uint64 DirectoryImpl::resultSize() const
		{
			LARGE_INTEGER size;
			size.HighPart = _result.nFileSizeHigh;
			size.LowPart = _result.nFileSizeLow;
		}
		
		bool DirectoryImpl::isResultDirectory() const
		{
			if(_result.dwFileAttributes != INVALID_FILE_ATTRIBUTES)
				return (_result.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			else
				return false;
		}
		
		bool DirectoryImpl::nextResult()
		{
			if(_firstCall)
			{
				_firstCall = false;
				return true;
			}
			
			if(FindNextFileW(_handle, &_result))
				return true;
			else
			{
				if(GetLastError() != ERROR_NO_MORE_FILES)
				{
					// Throw an exception.
				}
				
				return false;
			}
		}
		
		bool DirectoryImpl::open(const String &dirPath)
		{
			String searchPath = dirPath + "\\*";
			wchar_t *path = searchPath.wideBuffer();
			_handle = FindFirstFileW(path, &_result);
			delete[] path;
			
			if(_handle == INVALID_HANDLE_VALUE)
			{
				// Throw an exception.
				return false;
			}
			
			_firstCall = true;
			
			return true;
		}
		
		bool DirectoryImpl::create(const String &dirPath)
		{
			wchar_t *path = dirPath.wideBuffer();
			bool success = CreateDirectoryW(path, nullptr) != 0;
			delete[] path;
			
			return success || GetLastError() == ERROR_ALREADY_EXISTS;
		}
		
		bool DirectoryImpl::exists(const String &dirPath)
		{
			wchar_t *path = dirPath.wideBuffer();
			DWORD attributes = GetFileAttributesW(path);
			delete[] path;
			
			if(attributes != INVALID_FILE_ATTRIBUTES)
				return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			else
				return false;
		}
		
		String DirectoryImpl::current()
		{
			String currentPath;
			wchar_t *path = new wchar_t[MAX_PATH];
			
			unsigned int size = GetCurrentDirectoryW(MAX_PATH, path);
			if(size > MAX_PATH)
			{
				delete[] path;
				
				path = new wchar_t[size];
				if(GetCurrentDirectoryW(size, path) == 0)
				{
					// Throw an excpetion.
				}
				else
					currentPath = String::unicode(path);
			}
			else if(size == 0)
			{
				// Throw an excption.
			}
			else
				currentPath = String::unicode(path);
				
			delete[] path;
			
			return currentPath;
		}
		
		bool DirectoryImpl::remove(const String &dirPath)
		{
			wchar_t *path = dirPath.wideBuffer();
			bool success = RemoveDirectoryW(path) != 0;
			delete[] path;
			
			DWORD error = GetLastError();
			return success || error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND;
		}
	}
}