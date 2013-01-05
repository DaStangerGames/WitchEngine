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

#ifndef __WITCHENGINE_CORE_FILEIMPL_HPP__
#define __WITCHENGINE_CORE_FILEIMPL_HPP__

#include <windows.h>
#include <ctime>
#
#include <WitchCore/WitchGlobal.hpp>
#
#include <WitchCore/WitchEnums.hpp>

namespace WitchEngine
{
	namespace Core
	{
		// Forward declaration of needed classes.
		class String;
		
		class FileImpl
		{
			public:
				void close();
				bool atEnd() const;
				uint64 getCursorPos() const;
				bool open(const String &filename, OpenMode mode);
				bool flush();
				std::size_t read(char *buffer, std::size_t size);
				std::size_t write(const char *buffer, std::size_t size);
				
				static bool copy(const String &sourcePath, const String &targetPath);
				static bool remove(const String &filePath);
				static bool exists(const String &filePath);
				static std::time_t creationTime(const String &filePath);
				static std::time_t lastAccessTime(const String &filePath);
				static std::time_t lastWriteTime(const String &filePath);
				static uint64 size(const String &filePath);
				static bool rename(const String &sourcePath, const String &targetPath);
				
			private:
				HANDLE _handle;
		};
	}
}

#endif // __WITCHENGINE_CORE_FILEIMPL_HPP__