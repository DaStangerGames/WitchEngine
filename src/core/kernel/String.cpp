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

#include "String.hpp"
#
#include <cstring>

namespace WitchEngine
{
	namespace Core
	{
		String::String() :
			_buffer(&emptyString)
		{
		}
		
		String::String(char character)
		{
			if(character == '\0')
				_buffer = &emptyString;
			else
			{
				_buffer = new SharedString;
				_buffer->capacity = 1;
				_buffer->size = 1;
				_buffer->data = new char[2];
				_buffer->data[0] = character;
				_buffer->data[1] = '\0';
			}
		}
		
		String::String(const char *string)
		{
			if(string)
			{
				unsigned int size = std::strlen(string);
				if(size > 0)
				{
					_buffer = new SharedString;
					_buffer->capacity = size;
					_buffer->size = size;
					_buffer->data = new char[size + 1];
					std::memcpy(_buffer->data, string, size + 1);
				}
				else
					_buffer = &emptyString;
			}
			else
				_buffer = &emptyString;
		}
		
		String::String(const std::string &string)
		{
			if(string.size() > 0)
			{
				_buffer = new SharedString;
				_buffer->capacity = string.capacity();
				_buffer->size = string.size();
				_buffer->data = new char[string.capacity() + 1];
				std::memcpy(_buffer->data, string.c_str(), string.size() + 1);
			}
			else
				_buffer = &emptyString;
		}
		
		String::String(const String &string) :
			_buffer(string._buffer)
		{
			if(_buffer != &emptyString)
			{
				_buffer->mutex.lock();
				_buffer->refCount++;
				_buffer->mutex.unlock();
			}
		}
		
		String::String(String &&string) noexcept :
			_buffer(string._buffer)
		{
			string._buffer = &emptyString;
		}
		
		String::String(SharedString *sharedString) :
			_buffer(sharedString)
		{
		}
		
		String::~String()
		{
			releaseString();
		}
		
		void String::ensureOwnership()
		{
			if(_buffer == &emptyString)
				return;
				
			_buffer->mutex.lock();
			if(_buffer->refCount > 1)
			{
				_buffer->refCount--;
				
				char *string = new char[_buffer->capacity + 1];
				std::memcpy(string, _buffer->data, _buffer->size + 1);
				
				_buffer = new SharedString(_buffer->capacity, _buffer->size, 1, string);
			}
		}
		
		void String::releaseString()
		{
			if(_buffer == &emptyString)
				return;
				
			_buffer->mutex.lock();
			bool freeSharedString = (--_buffer->refCount == 0);
			_buffer->mutex.unlock();
			
			if(freeSharedString)
			{
				delete[] _buffer->data;
				delete _buffer;
			}
			
			_buffer = &emptyString;
		}
		
		String::SharedString String::emptyString(0, 0, 0, nullptr);
	}
}