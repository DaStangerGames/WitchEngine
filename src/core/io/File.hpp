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

#ifndef __WITCHENGINE_CORE_FILE_HPP__
#define __WITCHENGINE_CORE_FILE_HPP__

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
		class FileImpl; 	// Special forward declaration, private implementation.
		
		/**
		 * \class File
		 * \author Alexandre Valentin Jamet
		 * \date 29 December 2012
		 * \brief This class provides a way to handle file.
		**/
		class WITCHENGINE_CORE_EXPORT File
		{
			public:
			
				File();
				File(const String &filename);
				File(const String &filename, OpenMode mode);
				~File();
				
				bool atEnd() const;
				void close();
				bool isOpen() const;
				bool isReadable() const;
				bool isTextModeEnabled() const;
				bool isWritable() const;
				bool copy(const String &newName);
				bool exists() const;
				String filename() const;
				bool flush();
				int handle() const;
				bool open(OpenMode mode);
				bool open(const String &filename, OpenMode mode);
				OpenMode openMode() const;
				bool setOpenMode(OpenMode mode);
				uint64 peek(char *data, uint64 maxSize);
				void setTextModeEnabled(bool enabled);
				uint64 size() const;
				uint64 read(char *data, uint64 maxSize);
				char* read(uint64 maxSize);
				char* readAll();
				uint64 write(const char *data, uint64 maxSize);
				uint64 write(const char *data);
				
				static String absolutePath(const String &filePath);
				static bool copy(const String &sourcePath, const String &targetPath);
				static bool remove(const String &filePath);
				static bool exists(const String &filePath);
				static std::time_t creationTime(const String &filePath);
				static String directorty(const String &filePath);
				static std::time_t lastAccessTime(const String &filePath);
				static std::time_t lastWriteTime(const String &filePath);
				static uint64 size(const String &filePath);
				static bool isAbsolute(const String &filePath);
				static bool rename(const String &sourcePath, const String &targetPath);
				
			private:
				String _filename;
				FileImpl *_impl;
				OpenMode _mode;
		};
		
		WITCHENGINE_CORE_EXPORT File& operator<< (File &stream, const String &text);
		WITCHENGINE_CORE_EXPORT File& operator>> (File &stream, String &text);
	}
}

#endif // __WITCHENGINE_CORE_FILE_HPP__