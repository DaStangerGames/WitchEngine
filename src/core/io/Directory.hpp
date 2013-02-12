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

#ifndef __WITCHENGINE_CORE_DIRECTORY_HPP__
#define __WITCHENGINE_CORE_DIRECTORY_HPP__

#include <WitchCore/WitchGlobal.hpp>
#include "../kernel/String.hpp"

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#	define WITCH_DIRECTORY_SEPARATOR '\\'
#else
#	deifne WITCH_DIRECTORY_SEPARATOR '/'
#endif

#include "../kernel/Mutex.hpp"

namespace WitchEngine
{
	namespace Core
	{
		// Forward declaration.
		class DirectoryImpl;
		
		class WITCHENGINE_EXPORT Directory
		{
			public:
				Directory();
				Directory(const String &dirPath);
				~Directory();
				
				void close();
				
				String pattern() const;
				String resultName() const;
				String resultPath() const;
				uint64 resultSize() const;
				
				bool isOpen() const;
				bool isResultDirectory() const;
				
				bool nextResult(bool skipDors = true);
				
				bool open();
				
				void setDirectory(const String &dirPath);
				void setPattern(const String &pattern);
				
				static bool copy(const String &sourcePath, const String &destPath);
				static bool create(const String &dirPath, bool recursive = false);
				static bool exists(const String &dirPath);
				static String current();
				static bool remove(const String &dirPath, bool emptyDirectory = false);
				static bool setCurrent(const String &dirPath);
				
			private:
				mutable Mutex _mutex;
				
				String _dirPath;
				String _pattern;
				DirectoryImpl *_impl;
		};
	}
}

#endif // __WITCHENGINE_CORE_DIRECTORY_HPP__