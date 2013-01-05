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

#include <WitchCore/String.hpp>
#include <WitchCore/File.hpp>

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#	include "Win32/FileImpl.hpp"
#else
#	include "Posix/FileImpl.hpp"
#endif

namespace WitchEngine
{
	namespace Core
	{
		File::File() : 
			_filename(),
			_impl(nullptr),
			_mode(0)
		{
		}
		
		File::File(const String &filename) : 
			_filename(filename),
			_impl(nullptr),
			_mode(0)
		{
		}
		
		File::File(const String &filename, OpenMode mode) :
			_filename(filename),
			_impl(nullptr),
			_mode(mode)
		{
			open(mode);
		}
		
		File::~File()
		{
			close();
		}
		
		bool File::atEnd() const
		{
			if(!isOpen())
			{
				return false;
			}
			
			return _impl->atEnd();
		}
		
		void File::close()
		{
			if(_impl)
			{
				_impl->close();
				delete _impl;
				_impl = nullptr;
			}
		}
		
		bool File::isOpen() const
		{
			return _impl != nullptr;
		}
		
		bool File::isReadable() const
		{
			return (_mode & ReadOnly);
		}
		
		bool File::isTextModeEnabled() const
		{
			return (_mode & Text);
		}
		
		bool File::isWritable() const
		{
			return (_mode & WriteOnly);
		}
		
		bool File::copy(const String &newName)
		{
			return copy(_filename, newName);
		}
		
		bool File::exists() const
		{
			if(isOpen())
				return true;
			else
				return exists(_filename);
		}
		
		String File::filename() const
		{
			return _filename;
		}
		
		bool File::flush()
		{
			if(!isOpen())
			{
				return false;
			}
			
			if((_mode & ReadWrite) == 0 && (_mode & WriteOnly) == 0)
			{
				return false;
			}
			
			return _impl->flush();
		}
		
		bool File::open(OpenMode mode)
		{
			close();
			
			// if(_filename.isEmpty())
				// return false;
				
			if(mode != 0)
				_mode = mode;
				
			if(mode == 0)
				return false;
				
			_impl = new FileImpl;
			if(!_impl->open(_filename, _mode))
			{
				delete _impl;
				_impl = nullptr;
				
				return false;
			}
			
			return true;
		}
		
		bool File::open(const String &filename, OpenMode mode)
		{
			close();
			
			// if(filename.isEmpty())
				// return false;
				
			_filename = filename;
				
			if(mode != 0)
				_mode = mode;
				
			if(mode == 0)
				return false;
				
			_impl = new FileImpl;
			if(!_impl->open(_filename, _mode))
			{
				delete _impl;
				_impl = nullptr;
				
				return false;
			}
			
			return true;
		}
		
		OpenMode File::openMode() const
		{
			return _mode;
		}
		
		bool File::setOpenMode(OpenMode mode)
		{
			if(mode == 0 || mode == _mode)
				return true;
				
			if(isOpen())
			{
				FileImpl *impl = new FileImpl;
				if(!impl->open(_filename, mode))
				{
					delete impl;
					
					return false;
				}
				
				_impl->close();
				delete _impl;
				
				_impl = impl;
			}
			
			_mode = mode;
			
			return true;
		}
		
		void File::setTextModeEnabled(bool enabled)
		{
		}
		
		// Static methods declaration.
		bool File::copy(const String &sourcePath, const String &targetPath)
		{
			return FileImpl::copy(sourcePath, targetPath);
		}
		
		bool File::remove(const String &filePath)
		{
			return FileImpl::remove(filePath);
		}
		
		bool File::exists(const String &filePath)
		{
			return FileImpl::exists(filePath);
		}
		
		std::time_t File::creationTime(const String &filePath)
		{
			return FileImpl::creationTime(filePath);
		}
		
		std::time_t File::lastAccessTime(const String &filePath)
		{
			return FileImpl::lastAccessTime(filePath);
		}
		
		std::time_t File::lastWriteTime(const String &filePath)
		{
			return FileImpl::lastWriteTime(filePath);
		}
		
		uint64 File::size(const String &filePath)
		{
			return FileImpl::size(filePath);
		}
		
		bool File::rename(const String &sourcePath, const String &targetPath)
		{
			return FileImpl::rename(sourcePath, targetPath);
		}
		
		// Related operators.
		File& operator<< (File &stream, const String &text)
		{
			return stream;
		}
		
		File& operator>> (File &stream, String &text)
		{
			return stream;
		}
	}
}