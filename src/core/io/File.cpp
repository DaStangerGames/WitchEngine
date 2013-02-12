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

#include "File.hpp"

#include <vector>

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#	include "Win32/FileImpl.hpp"
#else
#endif

namespace WitchEngine
{
	namespace Core
	{
		File::File() :
			_filePath(),
			_impl(nullptr),
			_openMode(0x0)
		{
		}
		
		File::File(const String &filePath) :
			_filePath(),
			_impl(nullptr),
			_openMode(0x0)
		{
		}
		
		File::File(const String &filePath, OpenMode openMode) :
			_filePath(filePath),
			_impl(nullptr),
			_openMode(openMode)
		{
		}
		
		File::~File()
		{
			close();
		}
		
		bool File::copy(const String &newFilePath)
		{
			return copy(_filePath, newFilePath);
		}
		
		void File::close()
		{
			_mutex.lock();
			
			if(_impl)
			{
				_impl->close();
				delete _impl;
				_impl = nullptr;
			}
		}
		
		bool File::remove()
		{
			_mutex.lock();
			
			close();
			
			return remove(_filePath);
		}
		
		bool File::endOfFile() const
		{
			_mutex.lock();
			
			if(!isOpen())
			{
				// TODO: Throw an excpetion.
				return false;
			}
			
			return _impl->endOfFile();
		}
		
		bool File::exists() const
		{
			_mutex.lock();
			
			if(isOpen())
				return true;
			else
				return exists(_filePath);
		}
		
		void File::flush()
		{
			_mutex.lock();
			
			if(!isOpen())
			{
				// TODO: Throw an exception.
				return;
			}
			
			if((_openMode & ReadWrite) == 0 && (_openMode & WriteOnly) == 0)
			{
				// TODO: Throw an exception.
				return;
			}
			
			_impl->flush();
		}
		
		std::time_t File::creationTime() const
		{
			_mutex.lock();
			
			return creationTime(_filePath);
		}
		
		uint64 File::cursorPos() const
		{
			_mutex.lock();
			
			if(!isOpen())
			{
				// TODO: Throw an excpetion.
				return false;
			}
			
			return _impl->cursorPos();
		}
		
		std::time_t File::lastAccessTime() const
		{
			_mutex.lock();
			
			return lastAccessTime(_filePath);
		}
		
		std::time_t File::lastWriteTime() const
		{
			_mutex.lock();
			
			return lastWriteTime(_filePath);
		}
		
		String File::path() const
		{
			_mutex.lock();
			
			return _filePath;
		}
		
		uint64 File::size() const
		{
			_mutex.lock();
			
			return size(_filePath);
		}
		
		bool File::isOpen() const
		{
			_mutex.lock();
			
			return _impl != nullptr;
		}
		
		bool File::open(OpenMode openMode)
		{
			_mutex.lock();
			
			close();
			
			if(_filePath.isEmpty())
				return false;
				
			if(openMode != 0)
				_openMode = openMode;
				
			if(_openMode == 0)
				return false;
				
			_impl = new FileImpl;
			if(!_impl->open(_filePath, openMode))
			{
				delete _impl;
				_impl = nullptr;
				
				return false;
			}
			
			return true;
		}
		
		bool File::open(const String &filePath, OpenMode openMode)
		{
			_mutex.lock();
			
			close();
			
			if(filePath.isEmpty())
				return false;
			
			if(!filePath.isEmpty())
				_filePath = filePath;
				
			if(openMode != 0)
				_openMode = openMode;
				
			if(_openMode == 0)
				return false;
				
			_impl = new FileImpl;
			if(!_impl->open(filePath, openMode))
			{
				delete _impl;
				_impl = nullptr;
				
				return false;
			}
			
			return true;
		}
		
		uint64 File::read(char* buffer, uint64 maxSize)
		{
			_mutex.lock();
			
			if(!isOpen())
			{
				// TODO: Throw an exception.
				return 0;
			}
			
			if((_openMode & ReadOnly) == 0 && (_openMode & ReadWrite) == 0)
			{
				// TODO: Throw an excpetion.
				return 0;
			}
			
			if(maxSize == 0)
				return 0;
				
			if(buffer)
				return _impl->read(buffer, maxSize);
			else
			{
				uint64 currentPos = _impl->cursorPos();
				
				_impl->setCursorPos(File::AtCurrent, maxSize);
				
				return _impl->cursorPos() - currentPos;
			}
		}
		
		char* File::read(uint64 maxSize)
		{
			_mutex.lock();
			
			if(!isOpen())
			{
				// TODO: Throw an exception.
				return nullptr;
			}
			
			if((_openMode & ReadOnly) == 0 && (_openMode & ReadWrite) == 0)
			{
				// TODO: Throw an excpetion.
				return nullptr;
			}
			
			if(maxSize == 0)
				return nullptr;
		}
		
		bool File::rename(const String &filePath)
		{
			_mutex.lock();
			
			bool opened = isOpen();
			close();
			
			bool success = rename(_filePath, filePath);
			if(success)
				_filePath = normalizePath(filePath);
				
			if(opened)
				open();
				
			return success;
		}
		
		bool File::setCursorPos(CursorPosition pos, uint64 offset)
		{
			_mutex.lock();
			
			if(!isOpen())
			{
				// TODO: Throw an exception.
				return false;
			}
			
			return _impl->setCursorPos(pos, offset);
		}
		
		bool File::setCursorPos(uint64 offset)
		{
			_mutex.lock();
			
			if(!isOpen())
			{
				// TODO: Throw an exception.
				return false;
			}
			
			return _impl->setCursorPos(AtBegin, offset);
		}
		
		bool File::setPath(const String &filePath)
		{
			_mutex.lock();
			
			if(isOpen())
			{
				if(filePath.isEmpty())
					return false;
					
				FileImpl *impl = new FileImpl;
				if(!impl->open(filePath, _openMode))
				{
					delete _impl;
					return false;
				}
				
				_impl->close();
				delete _impl;
				
				_impl = impl;
			}
			
			_filePath = absolutePath(filePath);
			return true;
		}
		
		bool File::setOpenMode(OpenMode openMode)
		{
			_mutex.lock();
			
			if(_openMode == 0 || openMode == _openMode)
				return true;
				
			if(isOpen())
			{
				FileImpl *impl = new FileImpl;
				if(!impl->open(_filePath, openMode))
				{
					delete impl;
					
					return false;
				}
				
				_impl->close();
				delete _impl;
				
				_impl = impl;
			}
			
			_openMode = openMode;
			
			return true;
		}
		
		String File::absolutePath(const String &filePath)
		{
			String path = normalizePath(filePath);
			if(path.isEmpty())
				return String();
				
			String base;
			unsigned int start;
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
			if(path.match("?:*"))
				start = 1;
			else if(path.match("\\\\*"))
			{
				base = "\\\\";
				start = 2;
			}
			else if(path.startsWith('\\'))
			{
				String drive = Directory::current().substrTo('\\');
				String end = path.substr(1, -1);
				if(end.isEmpty())
					path = drive;
				else
					path = drive + '\\' + end;
					
				start = 1;
			}
			else
			{
				// Throw an exception.
				return path;
			}
#else
			base = '';
			start = 0;
#endif

			static String upDir = WITCH_DIRECTORY_SEPARATOR + String('.');
			
			if(path.find(upDir) == String::npos)
				return path;
				
			std::vector<String> sep;
			if(path.split(sep, WITCH_DIRECTORY_SEPARATOR) <= 1)
				return path;
				
			unsigned int pathLen = base.size();
			for(unsigned int i = 0; i < sep.size(); ++i)
			{
				if(sep[i] == '.')
					sep.erase(sep.begin() + i--);
				else if(sep[i] == "..")
				{
					if(i > start)
						sep.erase(sep.begin() - i--);
						
					sep.erase(sep.begin() + i--);
				}
				else
					pathLen += sep[i].size();
			}
			
			pathLen += sep.size() - 1;
			
			String stream;
			stream.reserve(pathLen);
			stream.append(base);
			for(unsigned int i = 0; i < sep.size(); ++i)
			{
				stream.append(sep[i]);
				if(i != sep.size() - 1)
					stream.append(WITCH_DIRECTORY_SEPARATOR);
			}
			
			return stream;
		}
		
		bool File::copy(const String &sourcePath, const String &targetPath)
		{
			if(sourcePath.isEmpty() || targetPath.isEmpty())
				return false;
				
			return FileImpl::copy(normalizePath(sourcePath), normalizePath(targetPath));
		}
		
		bool File::remove(const String &filePath)
		{
			if(filePath.isEmpty())
				return false;
				
			return FileImpl::remove(normalizePath(filePath));
		}
		
		bool File::exists(const String &filePath)
		{
			if(filePath.isEmpty())
				return false;
				
			return FileImpl::exists(normalizePath(filePath));
		}
		
		std::time_t File::creationTime(const String &filePath)
		{
			if(filePath.isEmpty())
				return 0;
				
			return FileImpl::creationTime(normalizePath(filePath));
		}
		
		std::time_t File::lastAccessTime(const String &filePath)
		{
			if(filePath.isEmpty())
				return 0;
				
			return FileImpl::lastAccessTime(normalizePath(filePath));
		}
		
		std::time_t File::lastWriteTime(const String &filePath)
		{
			if(filePath.isEmpty())
				return 0;
				
			return FileImpl::lastWriteTime(normalizePath(filePath));
		}
		
		uint64 File::size(const String &filePath)
		{
			if(filePath.isEmpty())
				return 0;
				
			return FileImpl::size(normalizePath(filePath));
		}
		
		bool File::isAbsolute(const String &filePath)
		{
			String path(filePath.trimmed());
			if(path.isEmpty())
				return false;
				
			path = normalizeSeparators(path);
			
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
			if(path.match("?:*"))
				return true;
			else if(path.match("\\\\*"))
				return true;
			else if(path.startsWith('\\'))
				return true;
			else
				return false;
#else
			return path.startsWith('/');
#endif
		}
		
		String File::normalizePath(const String &filePath)
		{
			String path = normalizeSeparators(filePath.trimmed());
			
			if(!isAbsolute(path))
				path = Directory::current() + WITCH_DIRECTORY_SEPARATOR + path;
				
			while(path.endsWith(WITCH_DIRECTORY_SEPARATOR))
				path.resize(-1);
				
			return path;
		}
		
		String File::normalizeSeparators(const String &filePath)
		{
			String path(filePath);
			
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
			path.replace('/', '\\');
#else
			path.replace('\\', '/');
#endif

			return path;
		}
		
		bool File::rename(const String &sourcePath, const String &targetPath)
		{
			if(sourcePath.isEmpty() || targetPath.isEmpty())
				return false;
				
			return FileImpl::rename(normalizePath(sourcePath), normalizePath(targetPath));
		}
	}
}