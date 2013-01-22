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
#include "../../3rdparty/utf8/utf8.h"
#
#include <cstring>

namespace WitchEngine
{
	namespace Core
	{
		inline unsigned int pow2(unsigned int n)
		{
			unsigned int x = 1;
			
			while(x <= n)
				x <<= 1;
				
			return x;
		}
		
		inline unsigned int newSize(unsigned int newSize)
		{	
			if(newSize < 20)
				return newSize + 4;
			else
			{
				if(newSize < (1 << 12) - 12)
					return pow2(newSize << 1) - 12;
				else
					return newSize + (1 << 11);
			}
		}
		
		inline char toLower(char character)
		{
			if(character >= 'A' && character <= 'Z')
				return character + ('a' - 'A');
			else
				return character;
		}
		
		inline char toUpper(char character)
		{
			if(character >= 'a' && character <= 'z')
				return character + ('A' - 'a');
			else
				return character;
		}
		
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
		
		String& String::append(char character)
		{
			if(character == '\0')
				return *this;
				
			if(_buffer->size == 0 && _buffer->capacity == 0)
				return operator= (character);
				
			if(_buffer->capacity > _buffer->size)
			{
				ensureOwnership();
				
				_buffer->data[_buffer->size] = character;
				_buffer->data[_buffer->size + 1] = '\0';
				_buffer->size++;
			}
			else
			{
				unsigned int size = _buffer->size + 1;
				unsigned int bufferSize = newSize(size);
				
				char *str = new char[bufferSize + 1];
				std::memcpy(str, _buffer->data, _buffer->size);
				str[_buffer->size] = character;
				str[size] = '\0';
				
				releaseString();
				_buffer = new SharedString;
				_buffer->capacity = bufferSize;
				_buffer->size = size;
				_buffer->data = str;
			}
			
			return *this;
		}
		
		String& String::append(const char *string)
		{
			if(!string || !string[0])
				return *this;
				
			unsigned int length = std::strlen(string);
			if(length == 0)
				return *this;
				
			if(_buffer->capacity >= _buffer->size + length)
			{
				ensureOwnership();
				
				std::memcpy(&_buffer->data[_buffer->size], string, length + 1);
				_buffer->size += length;
			}
			else
			{
				unsigned int size = _buffer->size + length;
				unsigned int bufferSize = newSize(size);
				
				char *str = new char[bufferSize + 1];
				std::memcpy(str, _buffer->data, _buffer->size);
				std::memcpy(&str[_buffer->size], string, length + 1);
				
				releaseString();
				
				_buffer = new SharedString;
				_buffer->capacity = bufferSize;
				_buffer->size = size;
				_buffer->data = str;
			}
			
			return *this;
		}
		
		String& String::append(const String &string)
		{
			if(string._buffer->size == 0)
				return *this;
			if(_buffer->size == 0 && _buffer->capacity < string._buffer->size)
				return operator= (string);
				
			if(_buffer->capacity >= _buffer->size + string._buffer->size)
			{
				ensureOwnership();
				
				std::memcpy(&_buffer->data[_buffer->size], string._buffer->data, string._buffer->size + 1);
				_buffer->size += string._buffer->size;
			}
			else
			{
				unsigned int size = _buffer->size + string._buffer->size;
				unsigned int bufferSize = newSize(size);
				
				char *str = new char[bufferSize + 1];
				std::memcpy(str, &_buffer->data, _buffer->size);
				std::memcpy(&str[_buffer->size], string._buffer->data, string._buffer->size + 1);
				
				releaseString();
				_buffer = new SharedString;
				_buffer->capacity = bufferSize;
				_buffer->size = size;
				_buffer->data = str;
			}
			
			return *this;
		}
		
		void String::clear(bool keepBuffer)
		{
			if(keepBuffer)
			{
				ensureOwnership();
				_buffer->size = 0;
			}
			else
				releaseString();
		}
		
		unsigned int String::find(char character, int start, Flags flags) const
		{
			if(character == '\0' || _buffer->size == 0)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			if(flags & CaseInsensitive)
			{
				char ch = toLower(character);
				const char *str = _buffer->data;
				do
				{
					if(toLower(*str) == ch)
						return static_cast<unsigned int>(str - _buffer->data);
				}
				while(*++str);
				
				return npos;
			}
			else
			{
				char *ch = std::strchr(&_buffer->data[pos], character);
				if(ch)
					return static_cast<unsigned int>(ch -_buffer->data);
				else
					return npos;
			}
		}
		
		unsigned int String::find(const char *string, int start, Flags flags) const
		{
			if(!string || !string[0] || _buffer->size == 0)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			char *str = &_buffer->data[pos];
			if(flags & CaseInsensitive)
			{
				if(flags & HandleUtf8)
				{
					while(utf8::internal::is_trail(*str))
						str++;
						
					utf8::unchecked::iterator<const char *> it(str);
					
					const char *t = string;
					// TODO: Complete this part.
				}
				else
				{
					char c = toLower(string[0]);
					do
					{
						if(toLower(*str) == c)
						{
							char *ptrPos = str;
							str++;
							
							const char *ptr = &string[1];
							while(true)
							{
								if(*ptr == '\0')
									return static_cast<unsigned int>(ptrPos - _buffer->data);
									
								if(*str == '\0')
									return npos;
									
								if(toLower(*str) != toLower(*ptr))
									break;
									
								ptr++;
								str++;
							}
						}
					}
					while(*++str);
				}
			}
			else
			{
				char *ch = std::strstr(&_buffer->data[pos], string);
				if(ch)
					return static_cast<unsigned int>(ch - _buffer->data);
			}
			
			return npos;
		}
		
		unsigned int String::find(const String &string, int start, Flags flags) const
		{
			if(string._buffer->size == 0 || string._buffer->size > _buffer->size)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			char *str = &_buffer->data[pos];
			if(flags & CaseInsensitive)
			{
				if(flags & HandleUtf8)
				{
					while(utf8::internal::is_trail(*str))
						str++;
						
					utf8::unchecked::iterator<const char *> it(str);
					
					const char *t = string._buffer->data;
					// TODO: Complete this part.
				}
				else
				{
					char c = toLower(string._buffer->data[0]);
					do
					{
						if(toLower(*str) == c)
						{
							char *ptrPos = str;
							str++;
							
							const char *ptr = &string._buffer->data[1];
							while(true)
							{
								if(*ptr == '\0')
									return static_cast<unsigned int>(ptrPos - _buffer->data);
									
								if(*str == '\0')
									return npos;
									
								if(toLower(*str) != toLower(*ptr))
									break;
									
								ptr++;
								str++;
							}
						}
					}
					while(*++str);
				}
			}
			else
			{
				char *ch = std::strstr(&_buffer->data[pos], string._buffer->data);
				if(ch)
					return static_cast<unsigned int>(ch - _buffer->data);
			}
			
			return npos;
		}
		
		char* String::buffer()
		{
			ensureOwnership();
			
			return _buffer->data;
		}
		
		unsigned int String::capacity() const
		{
			return _buffer->capacity;
		}
		
		const char* String::constBuffer() const
		{
			return _buffer->data;
		}
		
		unsigned int String::length() const
		{
			return utf8::distance(_buffer->data, &_buffer->data[_buffer->size]);
		}
		
		unsigned int String::size() const
		{
			return _buffer->size;
		}
		
		char* String::utf8Buffer(unsigned int *size) const
		{
			if(_buffer->size == 0)
				return nullptr;
				
			char *buffer = new char[_buffer->size + 1];
			std::memcpy(buffer, _buffer->data, _buffer->size + 1);
			
			if(size)
				*size = _buffer->size;
				
			return buffer;
		}
		
		char16_t* String::utf16Buffer(unsigned int *size) const
		{
			if(_buffer->size == 0)
				return nullptr;
				
			std::vector<char16_t> utf16;
			utf16.reserve(_buffer->size);
			
			utf8::utf8to16(_buffer->data, &_buffer->data[_buffer->size], std::back_inserter(utf16));
			
			unsigned int bufferSize = utf16.size();
			if(bufferSize == 0)
				return nullptr;
				
			char16_t* buffer = new char16_t[bufferSize + 1];
			std::memcpy(buffer, &utf16[0], bufferSize * sizeof(char16_t));
			buffer[bufferSize] = '\0';
			
			if(size)
				*size = bufferSize;
				
			return buffer;
		}
		
		char32_t* String::utf32Buffer(unsigned int *size) const
		{
			if(_buffer->size == 0)
				return nullptr;
				
			unsigned int bufferSize = utf8::distance(_buffer->data, &_buffer->data[_buffer->size]);
			if(bufferSize == 0)
				return nullptr;
				
			char32_t* buffer = new char32_t[bufferSize + 1];
			utf8::utf8to32(_buffer->data, &_buffer->data[_buffer->size], buffer);
			buffer[bufferSize] = '\0';
			
			if(size)
				*size = bufferSize;
				
			return buffer;
		}
		
		wchar_t* String::wideBuffer(unsigned int *size) const
		{
			static_assert(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4, "wchar_t size is not supported");
			if(_buffer->size == 0)
				return nullptr;
				
			unsigned int bufferSize = utf8::distance(_buffer->data, &_buffer->data[_buffer->size]);
			if(bufferSize == 0)
				return nullptr;
				
			wchar_t *buffer = new wchar_t[bufferSize + 1];
			if(sizeof(wchar_t) == 4)
				utf8::utf8to32(_buffer->data, &_buffer->data[_buffer->size], buffer);
			else
			{
				wchar_t *ptr = buffer;
				
				utf8::unchecked::iterator<const char *> it(_buffer->data);
				do
				{
					char32_t cp = *it;
					if(cp <= 0xffff && (cp < 0xd800 || cp > 0xdfff))
						*ptr++ = static_cast<wchar_t>(cp);
					else
						*ptr++ = L'?';
				}
				while (*it++);
			}
			
			if(size)
				*size = bufferSize;
				
			return buffer;
		}
		
		bool String::isEmpty() const
		{
			return _buffer->size == 0;
		}
		
		bool String::isNull() const
		{
			return _buffer == &emptyString;
		}
		
		String& String::operator= (char character)
		{
			if(character != '\0')
			{
				if(_buffer->capacity >= 1)
					ensureOwnership();
				else
				{
					releaseString();
					_buffer = new SharedString;
					_buffer->capacity = 1;
					_buffer->data = new char[2];
				}
				
				_buffer->size = 1;
				_buffer->data[0] = character;
				_buffer->data[1] = '\0';
			}
			else
				releaseString();
				
			return *this;
		}
		
		String& String::operator= (const char *string)
		{
			if(string && string[0] != '\0')
			{
				unsigned int size = std::strlen(string);
				if(_buffer->capacity >= size)
					ensureOwnership();
				else
				{
					releaseString();
					
					_buffer = new SharedString;
					_buffer->capacity = size;
					_buffer->data = new char[size + 1];
				}
				
				_buffer->size = size;
				std::memcpy(_buffer->data, string, size + 1);
			}
			else
				releaseString();
				
			return *this;
		}
		
		String& String::operator= (const std::string &string)
		{
			if(string.size() > 0)
			{
				if(_buffer->capacity >= string.size())
					ensureOwnership();
				else
				{
					releaseString();
					
					_buffer = new SharedString;
					_buffer->capacity = string.size();
					_buffer->data = new char[string.size() + 1];
				}
				
				_buffer->size = string.size();
				std::memcpy(_buffer->data, string.c_str(), string.size() + 1);
			}
			else
				releaseString();
				
			return *this;
		}
		
		String& String::operator= (const String &string)
		{
			releaseString();
			
			_buffer = string._buffer;
			if(_buffer != &emptyString)
			{
				_buffer->mutex.lock();	
				_buffer->refCount++;
				_buffer->mutex.unlock();
			}
			
			return *this;
		}
		
		String& String::operator= (String &&string) noexcept
		{
			std::swap(_buffer, string._buffer);
			
			return *this;
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
		const unsigned int String::npos(std::numeric_limits<unsigned int>::max());
	}
}