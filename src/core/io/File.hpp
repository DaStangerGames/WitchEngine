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

#ifndef __WITCHENGINE_CORE_FILE_HPP__
#define __WITCHENGINE_CORE_FILE_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#
#include "../kernel/String.hpp"
#include "../kernel/Mutex.hpp"
#
#include <ctime>

WITCH_BEGIN_HEADER

WITCH_MODULE(Core)

namespace WitchEngine
{
	namespace Core
	{
		// Forward declaration.
		class FileImpl;
		
		class WITCHENGINE_CORE_EXPORT File
		{
			public:
			
				enum CursorPosition
				{
					AtBegin,
					AtCurrent,
					AtEnd
				};
				
				enum OpenModeFlags
				{
					Current = 0x00,
					ReadOnly = 0x01,
					WriteOnly = 0x02,
					ReadWrite = ReadOnly | WriteOnly,
					Append = 0x04,
					Truncate = 0x08,
					Text = 0x10
				};
				WITCHENGINE_DECLARE_FLAGS(OpenMode, OpenModeFlags)
				
				enum Permission
				{
					ReadOwner = 0x4000, WriteOwner = 0x2000, ExeOwner = 0x1000,
					ReadUser = 0x0400, WriteUser = 0x0200, ExeUser = 0x0100,
					ReadGroup = 0x0040, WriteGroup = 0x0020, ExeGroup = 0x0010,
					ReadOther = 0x0004, WriteOther = 0x0002, ExeOther = 0x0001
				};
				WITCHENGINE_DECLARE_FLAGS(Permissions, Permission)
				
				File();
				File(const String &filePath);
				File(const String &filePath, OpenMode openMode);
				~File();
				
				bool copy(const String &newFilePath);
				void close();
				bool remove();
				
				bool endOfFile() const;
				
				bool exists() const;
				
				void flush();
				
				std::time_t creationTime() const;
				uint64 cursorPos() const;
				String directory() const;
				String fileName() const;
				std::time_t lastAccessTime() const;
				std::time_t lastWriteTime() const;
				String path() const;
				uint64 size() const;
				
				bool isOpen() const;
				
				bool open(OpenMode openMode = Current);
				bool open(const String &filePath, OpenMode openMode);
				
				uint64 read(char* buffer, uint64 maxSize);
				char* read(uint64 maxSize);
				
				bool rename(const String &filePath);
				
				bool setCursorPos(CursorPosition position, uint64 offset = 0);
				bool setCursorPos(uint64 offset);
				bool setPath(const String &path);
				bool setOpenMode(OpenMode openMode);
				
				uint64 write(const String &string);
				uint64 write(const char *data, uint64 maxSize);
				uint64 write(const char *data);
				
				static String absolutePath(const String &path);
				static bool copy(const String &sourcePath, const String &targetPath);
				static bool remove(const String &path);
				static bool exists(const String &path);
				static std::time_t creationTime(const String &path);
				static String directory(const String &path);
				static std::time_t lastAccessTime(const String &path);
				static std::time_t lastWriteTime(const String &path);
				static uint64 size(const String &path);
				static bool isAbsolute(const String &path);
				static String normalizePath(const String &path);
				static String normalizeSeparators(const String &path);
				static bool rename(const String &sourcePath, const String &targetPath);
				
			private:
				mutable Mutex _mutex;
				
				String _filePath;
				FileImpl *_impl;
				OpenMode _openMode;
		};
	}
}

WITCH_END_HEADER

#endif // __WITCHENGINE_CORE_FILE_HPP__