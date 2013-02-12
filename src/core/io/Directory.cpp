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

#include "Directory.hpp"
#include "File.hpp"

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#	include "Win32/DirectoryImpl.hpp"
#else
#	include "Posix/DirectoryImpl.hpp"
#endif

namespace WitchEngine
{
	namespace Core
	{
		String currentPath(DirectoryImpl::current());
		
		Directory::Directory() :
			_pattern('*')
		{
		}
		
		Directory::Directory(const String &dirPath) :
			_dirPath(dirPath),
			_pattern('*')
		{
		}
		
		Directory::~Directory()
		{
			close();
		}
		
		void Directory::close()
		{
			_mutex.lock();
			
			if(_impl)
			{
				_impl->close();
				delete _impl;
				_impl = nullptr;
			}
		}
		
		String Directory::pattern() const
		{
			_mutex.lock();
			
			return _pattern;
		}
		
		String Directory::resultName() const
		{
			_mutex.lock();
			
			if(!_impl)
			{
				// Throw an exception.
				return String();
			}
			
			return _impl->resultName();
		}
		
		String Directory::resultPath() const
		{
			_mutex.lock();
			
			if(!_impl)
			{
				// Throw an exception.
				return String();
			}
			
			return _dirPath + WITCH_DIRECTORY_SEPARATOR + _impl->resultName();
		}
		
		uint64 Directory::resultSize() const
		{
			_mutex.lock();
			
			if(!_impl)
			{
				// Throw an excpetion.
				return 0;
			}
			
			return _impl->resultSize();
		}
		
		bool Directory::isOpen() const
		{
			_mutex.lock();
			
			return _impl != nullptr;
		}
		
		bool Directory::isResultDirectory() const
		{
			_mutex.lock();
			
			if(!_impl)
			{
				// Throw an excpetion.
				return false;
			}
			
			return _impl->isResultDirectory();
		}
		
		bool Directory::nextResult(bool skipDots)
		{
			_mutex.lock();
			
			if(!_impl)
			{
				// Throw an excpetion.
				return false;
			}
			
			String name;
			do
			{
				if(!_impl->nextResult())
					return false;
					
				name = _impl->resultName();
				
				if(skipDots && (name == '.' || name == ".."))
					continue;
					
				if(name.match(_pattern))
					break;
			}
			while(true);
			
			return true;
		}
		
		bool Directory::open()
		{
			_mutex.lock();
			
			close();
			
			if(!exists(_dirPath))
				return false;
				
			_impl = new DirectoryImpl;
			if(!_impl->open(_dirPath))
			{
				delete _impl;
				_impl = nullptr;
				
				return false;
			}
			
			return true;
		}
		
		void Directory::setDirectory(const String &dirPath)
		{
			_mutex.lock();
			
			close();
			
			_dirPath = File::absolutePath(dirPath);
		}
		
		void Directory::setPattern(const String &pattern)
		{
			_mutex.lock();
			
			_pattern = pattern;
		}
		
		bool Directory::copy(const String &sourcePath, const String &destPath)
		{
			if(sourcePath.isEmpty() || destPath.isEmpty())
				return false;
				
			String dirPath(sourcePath);
			String dest(File::normalizePath(destPath));
			
			if(!create(destPath, true))
			{
				// Throw an excpetion.
				return false;
			}
			
			Directory dir(dirPath);
			if(!dir.open())
			{
				// Throw an excpetion.
				return false;
			}
			
			while(dir.nextResult(true));
			{
				if(dir.isResultDirectory())
				{
					if(!copy(dir.resultPath(), dest + WITCH_DIRECTORY_SEPARATOR + dir.resultName()))
						return false;
				}
				else if(File::copy(dir.resultPath(), dest + WITCH_DIRECTORY_SEPARATOR + dir.resultName()))
				{
					// Throw an exception.
					return false;
				}
			}
			
			dir.close();
			
			return true;
		}
		
		bool Directory::create(const String &dirPath, bool recursive)
		{
			if(dirPath.isEmpty())
				return false;
				
			if(recursive)
			{
				String path = File::normalizePath(dirPath);
				unsigned int foundPos = path.find(WITCH_DIRECTORY_SEPARATOR);
				if(foundPos == String::npos)
					return false;
					
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
				if(path.match("\\\\*"))
				{
					foundPos = path.find('\\', 2);
					if(foundPos == String::npos)
						return false;
						
					foundPos = path.find('\\', foundPos + 1);
					if(foundPos == String::npos)
						return false;
				}
#endif

				do
				{
					String p = path.substr(0, foundPos);
					if(p.endsWith(WITCH_DIRECTORY_SEPARATOR))
						p = p.substr(0, -2);
						
					if(!DirectoryImpl::exists(p) && !DirectoryImpl::create(p))
						return false;
						
					if(foundPos == String::npos)
						break;
						
					foundPos = path.find(WITCH_DIRECTORY_SEPARATOR, foundPos + 1);
				}
				while(true);
				
				return true;
			}
			else
				return DirectoryImpl::create(File::normalizePath(dirPath));
		}
		
		bool Directory::exists(const String &dirPath)
		{
			if(dirPath.isEmpty())
				return false;
				
			return DirectoryImpl::exists(File::normalizePath(dirPath));
		}
		
		String Directory::current()
		{
			return currentPath;
		}
		
		bool Directory::remove(const String &dirPath, bool emptyDirectory)
		{
			if(dirPath.isEmpty())
				return false;
				
			if(emptyDirectory)
			{
				Directory dir(dirPath);
				if(!dir.open())
					return DirectoryImpl::remove(dirPath);
					
				while(dir.nextResult(true))
				{
					if(dir.isResultDirectory())
					{
						if(!remove(dir.resultPath(), true))
							return false;
					}
					else if(!File::remove(dir.resultPath()))
					{
						// Throw an exception.
						return false;
					}
				}
				
				dir.close();
			}
			
			return DirectoryImpl::remove(File::normalizePath(dirPath));
		}
		
		bool Directory::setCurrent(const String &dirPath)
		{
			String path = File::absolutePath(dirPath);
			if(DirectoryImpl::exists(path))
			{
				currentPath = path;
				return true;
			}
			else
				return false;
		}
	}
}