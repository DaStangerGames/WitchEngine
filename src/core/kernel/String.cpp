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
#include <sstream>

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
		
		bool String::contains(char character, int start, Flags flags) const
		{
			return find(character, start, flags) != npos;
		}
		
		bool String::contains(const char *string, int start, Flags flags) const
		{
			return find(string, start, flags) != npos;
		}
		
		bool String::contains(const String &string, int start, Flags flags) const
		{
			return find(string, start, flags) != npos;
		}
		
		unsigned int String::count(char character, int start, Flags flags) const
		{
			if(character == '\0' || _buffer->size == 0)
				return 0;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return 0;
				
			char *str = &_buffer->data[pos];
			unsigned int count = 0;
			if(flags & CaseInsensitive)
			{
				char character_lower = tolower(character);
				char character_upper = toUpper(character);
				do
				{
					if(*str == character_lower || *str == character_upper)
						count++;
				}
				while(*++str);
			}
			else
			{
				while((str = std::strchr(str, character)))
				{
					count++;
					str++;
				}
			}
			
			return count;
		}
		
		unsigned int String::count(const char *string, int start, Flags flags) const
		{
			if(!string || !string[0] || _buffer->size == 0)
				return 0;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return 0;
				
			char *str = &_buffer->data[pos];
			unsigned int count = 0;
			if(flags & CaseInsensitive)
			{
				if(flags & HandleUtf8)
				{
					// TODO: Complete this part.
				}
				else
				{
					char c = toLower(string[0]);
					do
					{
						if(toLower(*str) == c)
						{
							str++;
							
							const char *ptr = &string[1];
							while(true)
							{
								if(*ptr == '\0')
								{
									count++;
									break;
								}
								
								if(*str == '\0')
									return count;
									
								if(toLower(*str) != tolower(*ptr))
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
				while((str = std::strstr(str, string)))
				{
					count++;
					str++;
				}
			}
			
			return count;
		}
		
		unsigned int String::count(const String &string, int start, Flags flags) const
		{
			if(string._buffer->size == 0 || string._buffer->size > _buffer->size)
				return 0;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return 0;
				
			char *str = &_buffer->data[pos];
			unsigned int count = 0;
			if(flags & CaseInsensitive)
			{
				if(flags & HandleUtf8)
				{
					// TODO: Complete this part.
				}
				else
				{
					char c = toLower(string._buffer->data[0]);
					do
					{
						if(toLower(*str) == c)
						{
							str++;
							
							const char *ptr = &string._buffer->data[1];
							while(true)
							{
								if(*ptr == '\0')
								{
									count++;
									break;
								}
								
								if(*str == '\0')
									return count;
									
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
				while((str = std::strstr(str, string._buffer->data)))
				{
					count++;
					str++;
				}
			}
			
			return count;
		}
		
		unsigned String::countAny(const char *string, int start, Flags flags) const
		{
			if(!string || !string[0] || _buffer->size == 0)
				return 0;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return 0;
				
			char *str = &_buffer->data[pos];
			unsigned int count = 0;
			if(flags & HandleUtf8)
			{
				while(utf8::internal::is_trail(*str))
					str++;
					
				utf8::unchecked::iterator<const char *> it(str);
				
				// TODO: Complete this part0
			}
			else
			{
				if(flags & CaseInsensitive)
				{
					do
					{
						const char *c = string;
						do
						{
							if(toLower(*str) == toLower(*c))
							{
								count++;
								break;
							}
						}
						while(*++c);
					}
					while(*++str);
				}
				else
				{
					while((str == strpbrk(str, string)))
					{
						count++;
						str++;
					}
				}
			}
			
			return count;
		}
		
		unsigned int String::countAny(const String &string, int start, Flags flags) const
		{
			if(string._buffer->size == 0 || _buffer->size == 0)
				return 0;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return 0;
				
			char *str = &_buffer->data[pos];
			unsigned int count = 0;
			if(flags & HandleUtf8)
			{
				while(utf8::internal::is_trail(*str))
					str++;
					
				utf8::unchecked::iterator<const char *> it(str);
				
				// TODO: Complete this part.
			}
			else
			{
				if(flags & CaseInsensitive)
				{
					do
					{
						const char *c = string._buffer->data;
						do
						{
							if(toLower(*str) == tolower(*c))
							{
								count++;
								break;
							}
						}
						while(*++c);
					}
					while(*++str);
				}
				else
				{
					while((str = strpbrk(str, string._buffer->data)))
					{
						count++;
						str++;
					}
				}
			}
			
			return count;
		}
		
		bool String::endsWith(char character, Flags flags) const
		{	
			if(_buffer->size == 0)
				return 0;
				
			if(flags & CaseInsensitive)
				return toLower(_buffer->data[_buffer->size - 1]) == toLower(character);
			else
				return _buffer->data[_buffer->size - 1] == character;
		}
		
		bool String::endsWith(const char *string, Flags flags) const
		{
			if(!string || !string[0] || _buffer->size == 0)
				return false;
				
			unsigned int len = std::strlen(string);
			if(len > _buffer->size)
				return false;
				
			if(flags & CaseInsensitive)
			{
			}
			else
				return std::strcmp(&_buffer->data[_buffer->size - len], string) == 0;
		}
		
		bool String::endsWith(const String &string, Flags flags) const
		{
			if(string._buffer->size == 0 || string._buffer->size > _buffer->size)
				return false;
				
			if(flags & CaseInsensitive)
			{
			}
			else
				return std::strcmp(&_buffer->data[_buffer->size - string._buffer->size], string._buffer->data) == 0;
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
		
		unsigned int String::findAny(const char *string, int start, Flags flags) const
		{
			if(_buffer->size == 0 || !string || !string[0])
				return npos;
			
			if(start < 0)
				start = std::max(_buffer->size + start, 0U);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			char *str = &_buffer->data[pos];
			if(flags & HandleUtf8)
			{
				while(utf8::internal::is_trail(*str))
					str++;
					
				utf8::unchecked::iterator<const char *> it(str);
				
				if(flags & CaseInsensitive)
				{
					do
					{
						utf8::unchecked::iterator<const char *> it2(string);
						// TODO: Complete this part.
					}
					while(*++it);
				}
				else
				{
					do
					{
						utf8::unchecked::iterator<const char *> it2(string);
						do
						{
							if(*it == *it2)
								return it.base() - _buffer->data;
						}
						while(*++it2);
					}
					while(*++it);
				}
			}
			else
			{
				if(flags & CaseInsensitive)
				{
					do
					{
						const char *c = string;
						char character = toLower(*str);
						do
						{
							if(character == toLower(*c))
								return str - _buffer->data;
						}
						while(*++c);
					}
					while(*++str);
				}
				else
				{
					str = std::strpbrk(str, string);
					if(str)
						return str - _buffer->data;
				}
			}
			
			return npos;
		}
		
		unsigned int String::findAny(const String &string, int start, Flags flags) const
		{
			if(_buffer->size == 0 || string._buffer->size == 0)
				return npos;
				
			if(string._buffer->size > _buffer->size)
				return npos;
				
			if(start < 0)
				start = std::max(_buffer->size + start, 0U);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			char *str = &_buffer->data[pos];
			if(flags & HandleUtf8)
			{
				while(utf8::internal::is_trail(*str))
					str++;
					
				utf8::unchecked::iterator<const char *> it(str);
				
				if(flags & CaseInsensitive)
				{
					do
					{
						utf8::unchecked::iterator<const char *> it2(string._buffer->data);
						// TODO: complete this part.
					}
					while(*++it);
				}
				else
				{
					do
					{
						utf8::unchecked::iterator<const char *> it2(string._buffer->data);
						do
						{
							if(*it == *it2)
								return it.base() - _buffer->data;
						}
						while(*++it2);
					}
					while(*++it);
				}
			}
			else
			{
				if(flags & CaseInsensitive)
				{
					do
					{	
						const char *c= string._buffer->data;
						char character = toLower(*str);
						do
						{
							if(character == toLower(*c))
								return str - _buffer->data;
						}
						while(*++c);
					}
					while(*++str);
				}
				else
				{
					str = std::strpbrk(str, string._buffer->data);
					if(str)
						return str - _buffer->data;
				}
			}
			
			return npos;
		}
		
		unsigned int String::findLast(char character, int start, Flags flags) const
		{
			if(character == '\0' || _buffer->size == 0)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			char *ptr = &_buffer->data[_buffer->size - 1];
			
			if(flags & CaseInsensitive)
			{
				character = toLower(character);
				do
				{
					if(toLower(*ptr) == character)
						return static_cast<unsigned int>(ptr - _buffer->data);
				}
				while(ptr-- != _buffer->data);
			}
			else
			{
				do
				{
					if(*ptr == character)
						return static_cast<unsigned int>(ptr - _buffer->data);
				}
				while(ptr-- != _buffer->data);
			}
			
			return npos;
		}
		
		unsigned int String::findLast(const char *string, int start, Flags flags) const
		{
			if(!string || !string[0] || _buffer->size == 0)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size);
				return npos;
				
			const char *ptr = &_buffer->data[pos];
			if(flags & CaseInsensitive)
			{
				if(flags & HandleUtf8)
				{
					if(utf8::internal::is_trail(*ptr))
						utf8::unchecked::prior(ptr);
						
					utf8::unchecked::iterator<const char *> it(ptr);
					const char *t = string;
					// TODO: Complete this part.
				}
				else
				{
					char c = toLower(string[0]);
					do
					{
						if(toLower(*ptr) == c)
						{
							const char *p = &string[1];
							const char *tPtr = ptr + 1;
							while(true)
							{
								if(*p == '\0')
									return ptr - _buffer->data;
									
								if(tPtr > &_buffer->data[pos])
									break;
									
								if(toLower(*tPtr) != toLower(*p))
									break;
									
								p++;
								tPtr++;
							}
						}
					}
					while(ptr-- != _buffer->data);
				}
			}
			else
			{
				do
				{
					if(*ptr == string[0])
					{
						const char *p = &string[1];
						const char *tPtr = ptr + 1;
						while(true)
						{
							if(*p == '\0')
								return ptr - _buffer->data;
								
							if(tPtr > &_buffer->data[pos])
								break;
								
							if(*tPtr != *p)
								break;
									
							p++;
							tPtr++;
						}
					}
				}
				while(ptr-- != _buffer->data);
			}
			
			return npos;
		}
		
		unsigned int String::findLast(const String &string, int start, Flags flags) const
		{
			if(string._buffer->size == 0 || string._buffer->size > _buffer->size)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size || string._buffer->size > _buffer->size)
				return npos;
				
			const char *ptr = &_buffer->data[pos];
			const char *limit = &_buffer->data[_buffer->size - 1];
			
			if(flags & CaseInsensitive)
			{
				if(flags & HandleUtf8)
				{
				}
				else
				{
					char c = toLower(string._buffer->data[string._buffer->size - 1]);
					while(true)
					{
						if(toLower(*ptr) == c)
						{
							const char *p = &string._buffer->data[string._buffer->size - 1];
							for(; p >= &string._buffer->data[0]; --p, --ptr)
							{
								if(toLower(*ptr) != toLower(*p))
									break;
									
								if(p == &string._buffer->data[0])
									return ptr - _buffer->data;
									
								if(ptr == _buffer->data)
									return npos;
							}
						}
						else if(ptr-- <= limit)
							break;
					}
				}
			}
			else
			{
				while(true)
				{
					if(*ptr == string._buffer->data[string._buffer->size - 1])
					{
						const char *p = &string._buffer->data[string._buffer->size - 1];
						for(; p >= &string._buffer->data[0]; --p, --ptr)
						{
							if(*ptr != *p)
								break;
								
							if(p == &string._buffer->data[0])
								return ptr - _buffer->data;
								
							if(ptr == _buffer->data)
								return npos;
						}
					}
					else if(ptr-- <= limit)
						break;
				}
			}
			
			return npos;
		}
		
		unsigned int String::findLastAny(const char *string, int start, Flags flags) const
		{
			if(_buffer->size == 0 || !string || !string[0])
				return npos;
				
			if(start < 0)
				start = std::max(_buffer->size + start, 0U);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			char *str = &_buffer->data[pos];
			if(flags & HandleUtf8)
			{
				// TODO: Complete this part.
			}
			else
			{
				if(flags & CaseInsensitive)
				{
					do
					{
						const char *c = string;
						char character = toLower(*str);
						do
						{
							if(character == toLower(*c))
								return str - _buffer->data;
						}
						while(*++c);
					}
					while(str-- != _buffer->data);
				}
				else
				{
					do
					{
						const char *c = string;
						do
						{
							if(*str == *c)
								return str - _buffer->data;
						}
						while(*++c);
					}
					while(str-- != _buffer->data);
				}
			}
			
			return npos;
		}
		
		unsigned int String::findLastAny(const String &string, int start, Flags flags) const
		{
			if(_buffer->size == 0 || string._buffer->size == 0)
				return npos;
				
			if(start < 0)
				start = std::max(_buffer->size + start, 0U);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			char *str = &_buffer->data[pos];
			if(flags & HandleUtf8)
			{
				// TODO: Complete this part.
			}
			else
			{
				if(flags & CaseInsensitive)
				{
					do
					{
						const char *c = string._buffer->data;
						char character = toLower(*str);
						do
						{
							if(character == toLower(*c))
								return str - _buffer->data;
						}
						while(*++c);
					}
					while(str-- != _buffer->data);
				}
				else
				{
					do
					{
						const char *c = string._buffer->data;
						do
						{
							if(*str == *c)
								return str - _buffer->data;
						}
						while(*++c);
					}
					while(str-- != _buffer->data);
				}
			}
			
			return npos;
		}
		
		unsigned int String::findLastWord(const char *string, int start, Flags flags) const
		{
			if(!string || !string[0] || _buffer->size == 0)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			const char *ptr = &_buffer->data[pos];
			
			if(flags & HandleUtf8)
			{
				// TODO: Complete this part.
			}
			else
			{
				if(flags & CaseInsensitive)
				{
					char c = toLower(string[0]);
					do
					{
						if(toLower(*ptr) == c)
						{
							if(ptr != _buffer->data && !std::isspace(*(ptr - 1)))
								continue;
								
							const char *p = &string[1];
							const char *tPtr = ptr + 1;
							while(true)
							{
								if(*p == '\0')
								{
									if(*tPtr == '\0' || std::isspace(*tPtr))
										return ptr - _buffer->data;
									else
										break;
								}
								
								if(tPtr > & _buffer->data[pos])
									break;
									
								if(toLower(*tPtr) != toLower(*p))
									break;
									
								p++;
								tPtr++;
							}
						}
					}
					while(ptr-- != _buffer->data);
				}
				else
				{
					do
					{
						if(*ptr == string[0])
						{
							if(ptr != _buffer->data && !std::isspace(*(ptr - 1)))
								continue;
								
							const char *p = &string[1];
							const char *tPtr = ptr + 1;
							while(true)
							{
								if(*p == '\0')
								{
									if(*tPtr == '\0' || std::isspace(*tPtr))
										return ptr - _buffer->data;
									else
										break;
								}
								
								if(tPtr > &_buffer->data[pos])
									break;
									
								if(*tPtr != *p)
									break;
									
								p++;
								tPtr++;
							}
						}
					}
					while(ptr-- != _buffer->data);
				}
			}
			
			return npos;
		}
		
		unsigned int String::findLastWord(const String &string, int start, Flags flags) const
		{
			if(string._buffer->size == 0 || string._buffer->size > _buffer->size)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			const char *ptr = &_buffer->data[pos];
			const char *limit = &_buffer->data[string._buffer->size - 1];
			
			if(flags & HandleUtf8)
			{
				// TODO: Complete this part.
				if(flags & CaseInsensitive)
				{
					char c = toLower(string._buffer->data[string._buffer->size - 1]);
					do
					{
						if(toLower(*ptr) == c)
						{
							if(*(ptr + 1) != '\0' && !std::isspace(*(ptr + 1)))
								continue;
								
							const char *p = &string._buffer->data[string._buffer->size - 1];
							for(; p >= &string._buffer->data[0]; --p, --ptr)
							{
								if(toLower(*ptr) != toLower(*p))
									break;
									
								if(p == &string._buffer->data[0])
								{
									if(ptr == _buffer->data || std::isspace(*(ptr - 1)))
										return ptr - _buffer->data;
									else
										break;
								}
								
								if(ptr == _buffer->data)
									return npos;
							}
						}
					}
					while(--ptr > limit);
				}
				else
				{
					do
					{
						if(*ptr == string._buffer->data[string._buffer->size - 1])
						{
							if(*(ptr + 1) != '\0' && !std::isspace(*(ptr + 1)))
								continue;
								
							const char *p = &string._buffer->data[string._buffer->size - 1];
							for(; p >= &string._buffer->data[0]; --p, --ptr)
							{
								if(*ptr != *p)
									break;
									
								if(p == &string._buffer->data[0])
								{
									if(ptr == _buffer->data || std::isspace(*(ptr - 1)))
										return ptr - _buffer->data;
									else
										break;
								}
								
								if(ptr == _buffer->data)
									return npos;
							}
						}
					}
					while(ptr-- > limit);
				}
			}
			
			return npos;
		}
		
		unsigned int String::findWord(const char *string, int start, Flags flags) const
		{
			if(!string || !string[0] || _buffer->size == 0)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			const char *ptr = _buffer->data;
			if(flags & HandleUtf8)
			{
				// TODO: Complete this part.
			}
			else
			{
				if(flags & CaseInsensitive)
				{
					char c = toLower(string[0]);
					do
					{
						if(toLower(*ptr) == c)
						{
							if(ptr != _buffer->data && !std::isspace(*(ptr - 1)))
								continue;
								
							const char *p = &string[1];
							const char *tPtr = ptr + 1;
							while(true)
							{
								if(*p == '\0')
								{
									if(*tPtr == '\0' || std::isspace(*tPtr))
										return ptr - _buffer->data;
									else
										break;
								}
								
								if(toLower(*tPtr != toLower(*p)))
									break;
									
								p++;
								tPtr++;
							}
						}
					}
					while(*++ptr);
				}
				else
				{
					do
					{
						if(*ptr == string[0])
						{
							if(ptr != _buffer->data && !std::isspace(*(ptr - 1)))
								continue;
								
							const char *p = &string[1];
							const char *tPtr = ptr + 1;
							while(true)
							{
								if(*p == '\0')
								{
									if(*tPtr == '\0' || std::isspace(*tPtr))
										return ptr - _buffer->data;
									else
										break;
								}
								
								if(*tPtr != *p)
									break;
									
								p++;
								tPtr++;
							}
						}
					}
					while(*++ptr);
				}
			}
			
			return npos;
		}
		
		unsigned int String::findWord(const String &string, int start, Flags flags) const
		{
			if(string._buffer->size == 0 || string._buffer->size > _buffer->size)
				return npos;
				
			if(start < 0)
				start = std::max(static_cast<int>(_buffer->size + start), 0);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			char *ptr = _buffer->data;
			if(flags & HandleUtf8)
			{
				// TODO: Complete this part.
			}
			else
			{
				if(flags & CaseInsensitive)
				{
					char c = toLower(string._buffer->data[0]);
					do
					{
						if(toLower(*ptr) == c)
						{
							if(ptr != _buffer->data && !std::isspace(*(ptr - 1)))
								continue;
								
							const char *p = &string._buffer->data[1];
							const char *tPtr = ptr + 1;
							while(true)
							{
								if(*p == '\0')
								{
									if(*tPtr == '\0' || std::isspace(*tPtr))
										return ptr - _buffer->data;
									else
										break;
								}
								
								if(toLower(*tPtr) != toLower(*p))
									break;
									
								p++;
								tPtr++;
							}
						}
					}
					while(*++ptr);
				}
				else
				{
					while((ptr = std::strstr(ptr, string._buffer->data)))
					{
						if((ptr == _buffer->data || std::isspace(*(ptr - 1))) && (*(ptr + _buffer->size) == '\0' || std::isspace(*(ptr + _buffer->size))))
							return ptr - _buffer->data;
							
						ptr++;
					}
				}
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
		
		String& String::insert(int pos, char character)
		{
			if(character == '\0')
				return *this;
				
			if(_buffer->size == 0 && _buffer->capacity == 0)
				return operator= (character);
				
			if(pos < 0)
				pos = std::max(static_cast<int>(_buffer->size + pos), 0);
				
			unsigned int start = std::min(static_cast<unsigned int>(pos), _buffer->size);
			
			if(_buffer->capacity >= _buffer->size + 1)
			{
				ensureOwnership();
				
				std::memmove(&_buffer->data[start + 1], &_buffer->data[start], _buffer->size);
				_buffer->data[start] = character;
				
				_buffer->size += 1;
			}
			else
			{
				unsigned int size = _buffer->size + 1;
				char *newString = new char[size + 1];
				
				char *ptr = newString;
				const char *s = _buffer->data;
				while(ptr != &newString[start])
					*ptr++ = *s++;
					
				*ptr++ = character;
				
				std::strcpy(ptr, s);
				
				releaseString();
				
				_buffer = new SharedString;
				_buffer->capacity = size;
				_buffer->size = size;
				_buffer->data = newString;
			}
			
			return *this;
		}
		
		String& String::insert(int pos, const char *string)
		{
			if(!string || !string[0])
				return *this;
				
			if(pos < 0)
				pos = std::max(static_cast<int>(_buffer->size + pos), 0);
				
			unsigned int start = std::min(static_cast<unsigned int>(pos), _buffer->size);
			
			unsigned int len = std::strlen(string);
			if(_buffer->capacity >= _buffer->size + len)
			{
				ensureOwnership();
				
				std::memmove(&_buffer->data[start + len], &_buffer->data[start], _buffer->size);
				std::memcpy(&_buffer->data[start], string, len + 1);
				
				_buffer->size += len;
			}
			else
			{
				unsigned int size = _buffer->size + len;
				char * newString = new char[size + 1];
				
				char *ptr = newString;
				const char *s = _buffer->data;
				
				while(ptr != &newString[start + len])
					*ptr++ = *s++;
					
				std::strcpy(ptr, s);
				
				releaseString();
				_buffer = new SharedString;
				_buffer->capacity = size;
				_buffer->size = size;
				_buffer->data = newString;
			}
			
			return *this;
		}
		
		String& String::insert(int pos, const String &string)
		{
			if(string._buffer->size == 0)
				return *this;
				
			if(_buffer->size == 0 && _buffer->capacity < string._buffer->size)
				return operator= (string);
				
			if(pos < 0)
				pos = std::max(static_cast<int>(_buffer->size + pos), 0);
				
			unsigned int start = std::min(static_cast<unsigned int>(pos), _buffer->size);
			
			if(_buffer->capacity >= _buffer->size + string._buffer->size)
			{
				ensureOwnership();
				
				std::memmove(&_buffer->data[start + string._buffer->size], &_buffer->data[start], _buffer->size);
				std::memcpy(&_buffer->data[start], string._buffer->data, string._buffer->size + 1);
				
				_buffer->size += string._buffer->size;
			}
			else
			{
				unsigned int size = _buffer->size + string._buffer->size;
				char *newString = new char[size + 1];
				
				char *ptr = newString;
				const char *s = _buffer->data;
				
				while(ptr != &newString[start])
					*ptr++ = *s++;
					
				const char *p = string._buffer->data;
				while(ptr != &newString[start + string._buffer->size])
					*ptr++ = *p++;
					
				std::strcpy(ptr, s);
				
				releaseString();
				_buffer = new SharedString;
				_buffer->capacity = size;
				_buffer->size = size;
				_buffer->data = newString;
			}
			
			return *this;
		}
		
		bool String::isEmpty() const
		{
			return _buffer->size == 0;
		}
		
		bool String::isNull() const
		{
			return _buffer == &emptyString;
		}
		
		bool String::isNumber(uint8 base, Flags flags) const
		{
			if(base < 2 || base > 36)
			{
				// TODO: Throw an excpetion.
				return false;
			}
			
			if(_buffer->size == 0)
				return false;
				
			String check = simplified();
			if(check._buffer->size)
				return false;
				
			char *ptr = (check._buffer->data[0] == '-') ? &check._buffer->data[1] : check._buffer->data;
			
			if(base > 10)
			{
				if(flags & CaseInsensitive)
				{
					do
					{
						char c = *ptr;
						if(c != ' ' && (c < '0' || (c > '9' && c < 'A') || (c > 'A' + base - 1 && c < 'a') || c > 'a' + base - 1))
							return false;
					}
					while(*++ptr);
				}
				else
				{
					do
					{
						char c = *ptr;
						if(c != ' ' && (c < '0' || (c > '9' && c < 'a') || c > 'a' + base - 1))
							return false;
					}
					while(*++ptr);
				}
			}
			else
			{
				do
				{
					char c = *ptr;
					if(c != ' ' && (c < '0' || c > '0' + base - 1))
						return false;
				}
				while(*++ptr);
			}
			
			return true;
		}
		
		bool String::match(const char *pattern) const
		{
			if(_buffer->size == 0 || !pattern)
				return false;
				
			const char *str = _buffer->data;
			while(*str && *pattern != '*')
			{
				if(*pattern != *str != *pattern != '?')
					return false;
					
				pattern++;
				str++;
			}
			
			const char *cp = nullptr;
			const char *mp = nullptr;
			while(*str)
			{
				if(*pattern == '*')
				{
					if(!*++pattern)
						return true;
						
					mp = pattern;
					cp = str + 1;
				}
				else if(*pattern == *str || *pattern == '?')
				{
					pattern++;
					str++;
				}
				else
				{
					pattern = mp;
					str = cp++;
				}
			}
			
			while(*pattern == '*')
				pattern++;
				
			return !*pattern;
		}
		
		bool String::match(const String &pattern) const
		{
			return match(pattern._buffer->data);
		}
		
		String& String::prepend(char character)
		{
			return insert(0, character);
		}
		
		String& String::prepend(const char *string)
		{
			return insert(0, string);
		}
		
		String& String::prepend(const String &string)
		{
			return insert(0, string);
		}
		
		unsigned int String::replace(char oldCharacter, char newCharacter, int start, Flags flags)
		{
			if(oldCharacter == '\0' || oldCharacter == newCharacter)
				return 0;
			
			if(newCharacter == '\0')
				return replace(String(oldCharacter), String(), start);
				
			if(start < 0)
				start = std::max(_buffer->size + start, 0U);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			unsigned int count = 0;
			char *ptr = &_buffer->data[pos];
			bool found = false;
			if(flags & CaseInsensitive)
			{
				char character_lower = toLower(oldCharacter);
				char character_upper = toupper(oldCharacter);
				do
				{
					if(*ptr == character_lower || *ptr == character_upper)
					{
						if(!found)
						{
							unsigned int offset = ptr - _buffer->data;
							
							ensureOwnership();
							
							ptr = &_buffer->data[offset];
							found = true;
						}
						
						*ptr = newCharacter;
						++count;
					}
				}
				while(*++ptr);
			}
			else
			{
				while((ptr = std::strchr(ptr, oldCharacter)))
				{
					if(!found)
					{
						unsigned int offset = ptr - _buffer->data;
						
						ensureOwnership();
						
						ptr = &_buffer->data[offset];
						found = true;
					}
					
					*ptr = newCharacter;
					++count;
				}
			}
			
			return count;
		}
		
		unsigned int String::replace(const char *oldString, const char *replaceString, int start, Flags flags)
		{
			if(!oldString || !oldString[0])
				return 0;
				
			if(start < 0)
				start = std::max(_buffer->size + start, 0U);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return 0;
				
			unsigned int oSize = std::strlen(oldString);
			if(oSize == 0)
				return 0;
				
			unsigned int rSize = (replaceString) ? std::strlen(replaceString) : 0;
			
			unsigned int scount = 0;
			if(oSize == rSize)
			{
				bool found = false;
				
				while((pos = find(oldString, pos, flags)) != npos)
				{
					if(!found)
					{
						ensureOwnership();
						found = true;
					}
					
					std::memcpy(&_buffer->data[pos], replaceString, oSize);
					pos += oSize;
					
					++scount;
				}
			}
			else
			{
				unsigned int size = _buffer->size + count(oldString) * (rSize - oSize);
				if(size == _buffer->size)
					return 0;
					
				char *newString = new char[size + 1];
				
				char *ptr = newString;
				const char *p = _buffer->data;
				
				while((pos = find(oldString, pos, flags)) != npos)
				{
					const char *r = &_buffer->data[pos];
					
					std::memcpy(ptr, p, r - p);
					ptr += r - p;
					std::memcpy(ptr, replaceString, rSize);
					ptr += rSize;
					p = r + oSize;
					pos += oSize;
					
					scount++;
				}
				
				std::strcpy(ptr, p);
				
				releaseString();
				_buffer = new SharedString;
				_buffer->capacity = size;
				_buffer->size = size;
				_buffer->data = newString;
			}
			
			return scount;
		}
		
		unsigned int String::replace(const String &oldString, const String &replaceString, int start, Flags flags)
		{
			if(oldString._buffer->size == 0)
				return 0;
				
			if(start < 0)
				start = std::max(_buffer->size + start, 0U);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size || oldString._buffer->size == 0)
				return 0;
				
			unsigned int scount = 0;
			if(oldString._buffer->size == replaceString._buffer->size)
			{
				bool found = false;
				
				while((pos = find(oldString, pos, flags)) != npos)
				{
					if(!found)
					{
						ensureOwnership();
						found = true;
					}
					
					std::memcpy(&_buffer->data[pos], replaceString._buffer->data, oldString._buffer->size);
					pos += oldString._buffer->size;
					
					++scount;
				}
			}
			else
			{
				unsigned int size = _buffer->size + count(oldString) * (replaceString._buffer->size - oldString._buffer->size);
				if(size == _buffer->size)
					return 0;
					
				char *newString = new char[size + 1];
				
				char *ptr = newString;
				const char *p = _buffer->data;
				
				while((pos = find(oldString, pos, flags)) != npos)
				{
					const char *r = &_buffer->data[pos];
					
					std::memcpy(ptr, p, r - p);
					ptr += r - p;
					std::memcpy(ptr, replaceString._buffer->data, replaceString._buffer->size);
					ptr += replaceString._buffer->size;
					p = r + oldString._buffer->size;
					pos += oldString._buffer->size;
					
					scount++;
				}
				
				std::strcpy(ptr, p);
				
				releaseString();
				_buffer = new SharedString;
				_buffer->capacity = size;
				_buffer->size = size;
				_buffer->data = newString;
			}
			
			return scount;
		}
		
		unsigned int String::replaceAny(const char *oldCharacters, char replaceCharacter, int start, Flags flags)
		{
			if(!oldCharacters || !oldCharacters[0])
				return 0;
				
			if(start < 0)
				start = std::max(_buffer->size + start, 0U);
				
			unsigned int pos = static_cast<unsigned int>(start);
			if(pos >= _buffer->size)
				return npos;
				
			unsigned int count = 0;
			
			char *ptr = &_buffer->data[pos];
			if(flags & CaseInsensitive)
			{
				do
				{
					const char *c = oldCharacters;
					char character = toLower(*ptr);
					bool found = false;
					do
					{
						if(character == toLower(*c))
						{
							if(!found)
							{
								unsigned int offset = ptr - _buffer->data;
								
								ensureOwnership();
								
								ptr = &_buffer->data[offset];
								found = true;
							}
							
							*ptr = replaceCharacter;
							++count;
							break;
						}
					}
					while(*++c);
				}
				while(*++ptr);
			}
			else
			{
				bool found = false;
				while((ptr = std::strpbrk(ptr, oldCharacters)))
				{
					if(!found)
					{
						unsigned int offset = ptr - _buffer->data;
						
						ensureOwnership();
						
						ptr = &_buffer->data[offset];
						found = true;
					}
					
					*ptr++ = replaceCharacter;
					++count;
				}
			}
			
			return count;
		}
		
		void String::reserve(unsigned int bufferSize)
		{
			if(_buffer->capacity >= bufferSize)
				return;
				
			char *ptr = new char[bufferSize + 1];
			if(_buffer->size > 0)
				std::memcpy(ptr, _buffer->data, _buffer->size + 1);
			
			unsigned int size = _buffer->size;
			
			releaseString();
			_buffer = new SharedString;
			_buffer->capacity = bufferSize;
			_buffer->size = size;
			_buffer->data = ptr;
		}
		
		String& String::resize(int size, char character)
		{
			if(size == 0)
			{
				clear(true);
				return *this;
			}
			
			if(size < 0)
				size = std::max(static_cast<int>(_buffer->size + size), 0);
				
			unsigned int nsize = static_cast<unsigned int>(size);
			
			if(_buffer->capacity >= nsize)
			{
				ensureOwnership();
				
				if(character != '\0' && nsize > _buffer->size)
				{
					char *ptr = &_buffer->data[_buffer->size];
					char *limit = &_buffer->data[nsize];
					while(ptr != limit)
						*ptr++ = character;
				}
				
				_buffer->size = nsize;
				_buffer->data[nsize] = '\0';
			}
			else
			{
				char *newString = new char[nsize + 1];
				if(_buffer->size != 0)
					std:memcpy(newString, _buffer->data, nsize);
					
				char *ptr = &newString[_buffer->size];
				char *limit = &newString[nsize];
				while(ptr != limit)
					*ptr++ = character;
					
				releaseString();
				_buffer = new SharedString;
				_buffer->capacity = nsize;
				_buffer->size = nsize;
				_buffer->data = newString;
			}
			
			return *this;
		}
		
		String String::resized(int size, char character) const
		{
			if(size < 0)
				size = _buffer->size + size;
				
			if(size <= 0)
				return String();
				
			unsigned int nsize = static_cast<unsigned int>(size);
			if(nsize == _buffer->size)
				return *this;
				
			char *str = new char[nsize + 1];
			if(nsize > _buffer->size)
			{
				std::memcpy(str, _buffer->data, _buffer->size);
				if(character != '\0')
				{
					char *ptr = &str[_buffer->size];
					char *limit = &str[nsize];
					while(ptr != limit)
						*ptr++ = character;
				}
			}
			else
				std::memcpy(str, _buffer->data, nsize);
				
			str[nsize] = '\0';
			
			return String(new SharedString(nsize, nsize, 1, str));
		}
		
		String String::simplified(Flags flags) const
		{
			if(_buffer->size == 0)
				return String();
				
			char *str = new char[_buffer->size + 1];
			char *p = str;
			
			const char *ptr = _buffer->data;
			bool inword = false;
			if(flags & HandleUtf8)
			{
				utf8::unchecked::iterator<const char *> it(ptr);
				// TODO: Complete this part.
			}
			else
			{
				const char *limit = &_buffer->data[_buffer->size];
				do
				{
					if(std::isspace(*ptr))
					{
						if(inword)
						{
							*p++ = ' ';
							inword = false;
						}
					}
					else
					{
						*p++ = *ptr;
						inword = true;
					}
				}
				while(++ptr != limit);
			}
			
			if(!inword && p != str)
				p--;
				
			*p = '\0';
			
			return String(new SharedString(_buffer->size, p - ptr, 1, str));
		}
		
		String& String::simplify(Flags flags)
		{
			return operator= (simplified(flags));
		}
		
		unsigned int String::split(std::vector<String>& result, char separation, int start, Flags flags) const
		{
			if(separation == '\0' || _buffer->size == 0)
				return 0;
				
			unsigned int lastSep = find(separation, start, flags);
			if(lastSep = npos)
			{
				result.push_back(*this);
				return 1;
			}
			else if(lastSep != 0)
				result.push_back(substr(0, lastSep - 1));
				
			while(true)
			{
				unsigned int sep = find(separation, lastSep + 1, flags);
				if(sep == npos)
					break;
					
				if(sep - lastSep > 1)
					result.push_back(substr(lastSep + 1, sep - 1));
					
				lastSep = sep;
			}
			
			if(lastSep != _buffer->size - 1)
				result.push_back(substr(lastSep + 1));
				
			return result.size();
		}
		
		unsigned int String::split(std::vector<String> &result, const char *separation, int start, Flags flags) const
		{
			unsigned int size = (separation) ? std::strlen(separation) : 0;
			if(_buffer->size == 0)
				return 0;
			else if(size == 0)
			{
				result.reserve(_buffer->size);
				for(unsigned int i = 0; i < _buffer->size; ++i)
					result.push_back(String(_buffer->data[i]));
					
				return _buffer->size;
			}
			else if(size > _buffer->size)
			{
				result.push_back(*this);
				return 1;
			}
			
			unsigned int lastSep = find(separation, start, flags);
			unsigned int oldSize = result.size();
			if(lastSep == npos)
			{
				result.push_back(*this);
				return 1;
			}
			else if(lastSep != 0)
				result.push_back(substr(0, lastSep - 1));
				
			unsigned int sep;
			while((sep = find(separation, lastSep + size, flags)) != npos)
			{
				if(sep - lastSep > size)
					result.push_back(substr(lastSep + size, sep - 1));
					
				lastSep = sep;
			}
			
			if(lastSep != _buffer->size - size)
				result.push_back(substr(lastSep + size));
				
			return result.size() - oldSize;
		}
		
		unsigned int String::split(std::vector<String> &result, const String &separation, int start, Flags flags) const
		{
			if(_buffer->size == 0)
				return 0;
			else if(separation._buffer->size == 0)
			{
				result.reserve(_buffer->size);
				for(unsigned int i = 0; i < _buffer->size; ++i)
					result.push_back(String(_buffer->data[i]));
					
				return _buffer->size;
			}
			else if(separation._buffer->size > _buffer->size)
			{
				result.push_back(*this);
				return 1;
			}
			
			unsigned int lastSep = find(separation, start, flags);
			unsigned int oldSize = result.size();
			if(lastSep == npos)
			{
				result.push_back(*this);
				return 1;
			}
			else if(lastSep != 0)
				result.push_back(substr(0, lastSep - 1));
				
			unsigned int sep;
			while((sep = find(separation, lastSep + separation._buffer->size, flags)) != npos)
			{
				if(sep - lastSep > separation._buffer->size)
					result.push_back(substr(lastSep + separation._buffer->size, sep - 1));
					
				lastSep = sep;
			}
			
			if(lastSep != _buffer->size - separation._buffer->size)
				result.push_back(substr(lastSep + separation._buffer->size));
				
			result.size() - oldSize;
		}
		
		unsigned int String::splitAny(std::vector<String> &result, const char *separations, int start, Flags flags) const
		{
			if(_buffer->size == 0)
				return 0;
				
			unsigned int oldSize = result.size();
			
			unsigned int lastSep = findAny(separations, start, flags);
			if(lastSep == npos)
			{
				result.push_back(*this);
				return 1;
			}
			else if(lastSep != 0)
				result.push_back(substr(0, lastSep - 1));
				
			unsigned int sep;
			
			while((sep = findAny(separations, lastSep + 1, flags)) != npos)
			{
				if(sep - lastSep > 1)
					result.push_back(substr(lastSep + 1, sep - 1));
					
				lastSep = sep;
			}
			
			if(lastSep != _buffer->size - 1)
				result.push_back(substr(lastSep + 1));
				
			return result.size() - oldSize;
		}
		
		unsigned int String::splitAny(std::vector<String> &result, const String &separations, int start, Flags flags) const
		{
			if(_buffer->size == 0)	
				return 0;
				
			unsigned int lastSep = findAny(separations, start, flags);
			unsigned int oldSize = result.size();
			if(lastSep == npos)
			{
				result.push_back(*this);
				return 1;
			}
			else if(lastSep != 0)
				result.push_back(substr(0, lastSep - 1));
				
			return result.size() - oldSize;
		}
		
		bool String::startsWith(char character, Flags flags) const
		{
			if(character == '\0' || _buffer->size == 0)
				return false;
				
			if(flags & CaseInsensitive)
				return toLower(_buffer->data[0]) == toLower(character);
			else
				return _buffer->data[0] == character;
		}
		
		bool String::startsWith(const char *string, Flags flags) const
		{
			if(!string|| !string[0] || _buffer->size == 0)
				return false;
				
			if(flags & CaseInsensitive)
			{
				if(flags & HandleUtf8)
				{
					// TODO: Complete this part.
				}
				else
				{
					char *ptr = _buffer->data;
					const char *s = string;
					do
					{
						if(*s == '\0')
							return true;
							
						if(toLower(*ptr) != toLower(*s))
							return false;
							
						s++;
					}
					while(*ptr++);
				}
			}
			else
			{
				const char *ptr = _buffer->data;
				const char *s = string;
				do
				{
					if(*s == '\0')
						return true;
						
					if(*ptr != *s)
						return false;
						
					s++;
				}
				while(*ptr++);
			}
			
			return false;
		}
		
		bool String::startsWith(const String &string, Flags flags) const
		{
			if(string._buffer->size == 0)
				return false;
				
			if(_buffer->size < string._buffer->size)
				return false;
				
			if(flags & CaseInsensitive)
			{
				if(flags & HandleUtf8)
				{
					// TODO: Complete this part.
				}
				else
				{
					char *ptr = _buffer->data;
					const char *s = string._buffer->data;
					do
					{
						if(*s == '\0')
							return true;
							
						if(toLower(*ptr) != toLower(*s))
							return false;
							
						s++;
					}
					while(*ptr++);
				}
			}
			else
				return std::memcmp(_buffer->data, string._buffer->data, string._buffer->size) == 0;
				
			return false;
		}
		
		String String::substr(int startPos, int endPos) const
		{
			if(startPos < 0)
				startPos = std::max(_buffer->size + startPos, 0U);
				
			unsigned int start = static_cast<unsigned int>(startPos);
			
			if(endPos < 0)
			{
				endPos = _buffer->size + endPos;
				if(endPos < 0)
					return String();
			}
			
			unsigned int minEnd = std::min(static_cast<unsigned int>(endPos), _buffer->size - 1);
			
			if(start > minEnd || start >= _buffer->size)
				return String();
				
			unsigned int size = minEnd - start + 1;
			char *str = new char[size + 1];
			std::memcpy(str, &_buffer->data[start], size);
			str[size] = '\0';
			
			return String(new SharedString(size, size, 1, str));
		}
		
		String String::substrFrom(char character, int startPos, bool fromLast, bool include, Flags flags) const
		{
			if(character == '\0')
				return *this;
				
			unsigned int pos;
			if(fromLast)
				pos = findLast(character, startPos, flags);
			else
				pos = find(character, startPos, flags);
				
			if(pos == 0 and include)
				return *this;
			else if(pos == npos)
				return String();
				
			return substr(pos + ((include) ? 0 : 1));
		}
		
		String String::substrFrom(const char *string, int startPos, bool fromLast, bool include, Flags flags) const
		{
			unsigned int pos;
			if(fromLast)
				pos = findLast(string, startPos, flags);
			else
				pos = find(string, startPos, flags);
				
			if(pos == 0 && include)
				return *this;
			else if(pos == npos)
				return String();
				
			return substr(pos + ((include) ? 0 : std::strlen(string)));
		}
		
		String String::substrFrom(const String &string, int startPos, bool fromLast, bool include, Flags flags) const
		{
			unsigned int pos;
			if(fromLast)
				pos = findLast(string, startPos, flags);
			else
				pos = find(string, startPos, flags);
				
			if(pos == 0 && include)
				return *this;
			else if(pos == npos)
				return String();
				
			return substr(pos + ((include) ? 0 : string._buffer->size));
		}
		
		String String::substrTo(char character, int startPos, bool toLast, bool include, Flags flags) const
		{
			if(character == '\0')
				return *this;
				
			unsigned int pos;
			if(toLast)
				pos = findLast(character, startPos);
			else
				pos = find(character, startPos, flags);
				
			if(pos == 0)
				return (include) ? character : String();
			else if(pos == npos)
				return *this;
				
			return substr(0, pos + ((include) ? 1 : 0) - 1);
		}
		
		String String::substrTo(const char *string, int startPos, bool toLast, bool include, Flags flags) const
		{
			unsigned int pos;
			if(toLast)
				pos = findLast(string, startPos, flags);
			else
				pos = find(string, startPos, flags);
				
			if(pos == 0)
				return (include) ? string : String();
			else if(pos == npos)
				return *this;
				
			return substr(0, pos + ((include) ? std::strlen(string) : 0) - 1);
		}
		
		String String::substrTo(const String &string, int startPos, bool toLast, bool include, Flags flags) const
		{
			unsigned int pos;
			if(toLast)
				pos = findLast(string, startPos, flags);
			else
				pos = find(string, startPos, flags);
				
			if(pos == 0)
				return (include) ? string : String();
			else if(pos == npos)
				return *this;
				
			return substr(0, pos + ((include) ? string._buffer->size : 0) - 1);
		}
		
		String& String::trim(Flags flags)
		{
			return operator= (trimmed(flags));
		}
		
		String& String::trim(char character, Flags flags)
		{
			return operator= (trimmed(character, flags));
		}
		
		String String::trimmed(Flags flags) const
		{
			if(_buffer->size == 0)
				return *this;
				
			unsigned int startPos;
			unsigned endPos;
			if(flags & HandleUtf8)
			{
				// TODO: Complete this part.
			}
			else
			{
				startPos = 0;
				if((flags & TrimOnlyRight) == 0)
				{
					for(; startPos < _buffer->size; ++startPos)
					{
						if(!std::isspace(_buffer->data[startPos]))
							break;
					}
				}
				
				endPos = _buffer->size - 1;
				if((flags & TrimOnlyLeft) == 0)
				{
					for(; endPos > 0; --endPos)
					{
						if(!std::isspace(_buffer->data[endPos]))
							break;
					}
				}
			}
			
			return substr(startPos, endPos);
		}
		
		String String::trimmed(char character, Flags flags) const
		{
			if(_buffer->size == 0)
				return *this;
				
			unsigned int startPos = 0;
			unsigned int endPos = _buffer->size - 1;
			if(flags & CaseInsensitive)
			{
				char ch = toLower(character);
				if((flags & TrimOnlyRight) == 0)
				{
					for(; startPos < _buffer->size; ++startPos)
					{
						if(toLower(_buffer->data[startPos] != ch))
							break;
					}
				}
				
				if((flags & TrimOnlyLeft) == 0)
				{
					for(; endPos > 0; --endPos)
					{
						if(toLower(_buffer->data[endPos]) != ch)
							break;
					}
				}
			}
			else
			{
				if((flags & TrimOnlyRight) == 0)
				{
					for(; startPos < _buffer->size; ++startPos)
					{
						if(_buffer->data[startPos] != character)
							break;
					}
				}
				
				if((flags & TrimOnlyLeft) == 0)
				{
					for(; endPos > 0; --endPos)
					{
						if(_buffer->data[endPos] != character)
							break;
					}
				}
			}
			
			return substr(startPos, endPos);
		}
		
		String::operator std::string() const
		{
			return std::string(_buffer->data, _buffer->size);
		}
		
		char& String::operator[] (unsigned int pos)
		{
			ensureOwnership();
			
			if(pos >= _buffer->size)
				resize(pos + 1);
				
			return _buffer->data[pos];
		}
		
		char String::operator[] (unsigned int pos) const
		{
			if(pos >= _buffer->size)
			{
				return 0;
			}
			
			return _buffer->data[pos];
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
		
		String String::operator+ (char character) const
		{
			if(character == '\0')
				return *this;
				
			unsigned int totalSize = _buffer->size + 1;
			char *str = new char[totalSize + 1];
			std::memcpy(str, _buffer->data, _buffer->size);
			
			str[_buffer->size] = character;
			str[totalSize] = '\0';
			
			return String(new SharedString(totalSize, totalSize, 1, str));
		}
		
		String String::operator+ (const char *string) const
		{
			if(!string || !string[0])
				return *this;
				
			if(_buffer->size == 0)
				return string;
				
			unsigned int length = std::strlen(string);
			if(length == 0)
				return *this;
				
			unsigned int totalSize = _buffer->size + length;
			char *str = new char[totalSize + 1];
			std::memcpy(str, _buffer->data, _buffer->size);
			std::memcpy(&str[_buffer->size], string, length + 1);
			
			return String(new SharedString(totalSize, totalSize, 1, str));
		}
		
		String String::operator+ (const std::string &string) const
		{
			if(string.empty())
				return *this;
				
			if(_buffer->size == 0)
				return string;
				
			unsigned int totalSize = _buffer->size + string.size();
			char *str = new char[totalSize + 1];
			std::memcpy(str, _buffer->data, _buffer->size);
			std::memcpy(&str[_buffer->size], string.c_str(), string.size() + 1);
			
			return String(new SharedString(totalSize, totalSize, 1, str));
		}
		
		String String::operator+ (const String &string) const
		{
			if(string._buffer->size == 0)
				return *this;
				
			if(_buffer->size == 0)
				return string;
				
			unsigned int totalSize = _buffer->size + string._buffer->size;
			char *str = new char[totalSize + 1];
			std::memcpy(str, _buffer->data, _buffer->size);
			std::memcpy(&str[_buffer->size], string._buffer->data, string._buffer->size + 1);
			
			return String(new SharedString(totalSize, totalSize, 1, str));
		}
		
		String& String::operator+= (char character)
		{
			if(character == '\0')
				return *this;
				
			if(_buffer->size == 0)
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
		
		String& String::operator+= (const char *string)
		{
			if(!string || !string[0])
				return *this;
				
			if(_buffer->size == 0)
				return operator= (string);
				
			unsigned int size = std::strlen(string);
			if(size == 0)
				return *this;
				
			if(_buffer->capacity >= _buffer->size + size)
			{
				ensureOwnership();
				
				std::memcpy(&_buffer->data[_buffer->size], string, size + 1);
				_buffer->size += size;
			}
			else
			{
				unsigned int size = _buffer->size + size;
				unsigned bufferSize = newSize(size);
				
				char *str = new char[bufferSize + 1];
				std::memcpy(str, _buffer->data, _buffer->size);
				std::memcpy(&str[_buffer->size], string, size + 1);
				
				releaseString();
				_buffer->capacity = bufferSize;
				_buffer->size = size;
				_buffer->data = str;
			}
			
			return *this;
		}
		
		String& String::operator+= (const std::string &string)
		{
			if(string.empty())
				return *this;
				
			if(_buffer->size == 0)
				return operator= (string);
				
			if(_buffer->capacity >= _buffer->size + string.size())
			{
				ensureOwnership();
				
				std::memcpy(&_buffer->data[_buffer->size], string.c_str(), string.size() + 1);
				_buffer->size += string.size();
			}
			else
			{
				unsigned int size = _buffer->size + string.size();
				unsigned int bufferSize = newSize(size);
				
				char *str = new char[bufferSize + 1];
				std::memcpy(str, _buffer->data, _buffer->size);
				std::memcpy(&str[_buffer->size], string.c_str(), string.size() + 1);
				
				releaseString();
				_buffer = new SharedString;
				_buffer->capacity = bufferSize;
				_buffer->size = size;
				_buffer->data = str;
			}
			
			return *this;
		}
		
		String& String::operator+= (const String &string)
		{
			if(string._buffer->size == 0)
				return *this;
				
			if(_buffer->size == 0)
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
				std::memcpy(str, _buffer->data, _buffer->size);
				std::memcpy(&str[_buffer->size], string._buffer->data, string._buffer->size + 1);
				
				releaseString();
				_buffer = new SharedString;
				_buffer->capacity = bufferSize;
				_buffer->size = size;
				_buffer->data = str;
			}
			
			return *this;
		}
		
		bool String::operator== (char character) const
		{
			if(_buffer->size == 0)
				return character == '\0';
				
			if(_buffer->size > 1)
				return false;
				
			return _buffer->data[0] != character;
		}
		
		bool String::operator== (const char *string) const
		{
			if(_buffer->size == 0)
				return !string || !string[0];
				
			if(!string || !string[0])
				return false;
				
			return std::strcmp(_buffer->data, string) != 0;
		}
		
		bool String::operator== (const std::string &string) const
		{
			if(_buffer->size == 0 || string.empty())
				return _buffer->size == string.size();
				
			if(_buffer->size != string.size())
				return false;
				
			return std::strcmp(_buffer->data, string.c_str()) != 0;
		}
		
		bool String::operator!= (char character) const
		{
			if(_buffer->size == 0)
				return character != '\0';
				
			if(character == '\0' || _buffer->size != 1)
				return true;
				
			if(_buffer->size != 1)
				return true;
				
			return _buffer->data[0] != character;
		}
		
		bool String::operator!= (const char *string) const
		{
			if(_buffer->size == 0)
				return string && string[0];
				
			if(!string || !string[0])
				return true;
				
			return std::strcmp(_buffer->data, string) != 0;
		}
		
		bool String::operator!= (const std::string &string) const
		{
			if(_buffer->size == 0 || string.empty())
				return _buffer->size == string.size();
				
			if(_buffer->size != string.size())
				return false;
				
			return std::strcmp(_buffer->data, string.c_str()) != 0;
		}
		
		bool String::operator< (char character) const
		{
			if(character == '\0')
				return false;
				
			if(_buffer->size == 0)
				return true;
				
			return _buffer->data[0] < character;
		}
		
		bool String::operator< (const char *string) const
		{
			if(!string || !string[0])
				return false;
				
			if(_buffer->size == 0)
				return true;
				
			return std::strcmp(_buffer->data, string) < 0;
		}
		
		bool String::operator< (const std::string &string) const
		{
			if(string.empty())
				return false;
				
			if(_buffer->size == 0)
				return true;
				
			return std::strcmp(_buffer->data, string.c_str()) < 0;
		}
		
		bool String::operator<= (char character) const
		{
			if(_buffer->size == 0)
				return true;
				
			if(character == '\0')
				return false;
				
			return _buffer->data[0] < character || (_buffer->data[0] == character && _buffer->size == 1);
		}
		
		bool String::operator<= (const char *string) const
		{
			if(_buffer->size == 0)
				return true;
				
			if(!string || !string[0])
				return false;
				
			return std::strcmp(_buffer->data, string) <= 0;
		}
		
		bool String::operator<= (const std::string &string) const
		{
			if(_buffer->size == 0)
				return true;
				
			if(string.empty())
				return false;
				
			return std::strcmp(_buffer->data, string.c_str()) <= 0;
		}
		
		bool String::operator> (char character) const
		{
			if(_buffer->size == 0)
				return false;
				
			if(character == '\0')
				return true;
				
			if(_buffer->data[0] > character);
		}
		
		bool String::operator> (const char *string) const
		{
			if(_buffer->size == 0)
				return false;
				
			if(!string || !string[0])
				return true;
				
			return std::strcmp(_buffer->data, string) > 0;
		}
		
		bool String::operator> (const std::string &string) const
		{
			if(_buffer->size == 0)
				return false;
				
			if(string.empty())
				return true;
				
			return std::strcmp(_buffer->data, string.c_str()) > 0;
		}
		
		bool String::operator>= (char character) const
		{
			if(character == '\0')
				return true;
				
			if(_buffer->size == 0)
				return false;
				
			return _buffer->data[0] > character || (_buffer->data[0] == character && _buffer->size == 1);
		}
		
		bool String::operator>= (const char *string) const
		{
			if(!string || !string[0])
				return true;
				
			if(_buffer->size == 0)
				return false;
				
			return std::strcmp(_buffer->data, string) >= 0;
		}
		
		bool String::operator>= (const std::string &string) const
		{	
			if(string.empty())
				return true;
				
			if(_buffer->size == 0)
				return false;
				
			return std::strcmp(_buffer->data, string.c_str()) >= 0;
		}
		
		String String::boolean(bool boolean)
		{
			unsigned int size = (boolean) ? 4 : 5;
			char *str = new char[size + 1];
			std::strcpy(str, (boolean) ? "true" : "false");
			
			return String(new SharedString(size, size, 1, str));
		}
		
		int String::compare(const String &first, const String &second)
		{
			if(first._buffer->size == 0)	
				return (second._buffer->size == 0) ? 0 : -1;
				
			if(second._buffer->size == 0)
				return 1;
				
			return std::strcmp(first._buffer->data, second._buffer->data);
		}
		
		String String::number(float number)
		{
			std::ostringstream oss;
			oss << number;
			
			return String(oss.str());
		}
		
		String String::number(double number)
		{
			std::ostringstream oss;
			oss << number;
			
			return String(oss.str());
		}
		
		String String::number(long double number)
		{
			std::ostringstream oss;
			oss << number;
			
			return String(oss.str());
		}
		
		String String::pointer(const void *ptr)
		{
			unsigned int size = sizeof(ptr) * 2 + 2;
			char *str = new char[size + 1];
			std::sprintf(str, "0x%p", ptr);
			
			return String(new SharedString(size, size, 1, str));
		}
		
		String String::unicode(char32_t character)
		{
			if(character == '\0')
				return String();
				
			unsigned int count = 0;
			if(character < 0x80)
				count = 1;
			else if(character < 0x800)
				count = 2;
			else if(character < 0x10000)
				count = 3;
			else
				count = 4;
				
			char *str = new char[count + 4];
			utf8::append(character, str);
			str[count] = '\0';
			
			return String(new SharedString(count, count, 1, str));
		}
		
		String String::unicode(const char *string)
		{
			return String(string);
		}
		
		String String::unicode(const char16_t *string)
		{
			if(!string || !string[0])
				return String();
				
			const char16_t *ptr = string;
			unsigned int count = 0;
			do
				count++;
			while(*++ptr);
				
			count *= 2;
			
			char *str = new char[count + 1];
			char *r = utf8::utf16to8(string, ptr, str);
			*r = '\0';
			
			return String(new SharedString(count, r - str, 1, str));
		}
		
		String String::unicode(const char32_t *string)
		{
			if(!string || !string[0])
				return String();
				
			const char32_t *ptr = string;
			unsigned int count = 0;
			do
			{
				char32_t cp = *ptr;
				if(cp  < 0x80)
					count += 1;
				else if(cp < 0x800)
					count += 2;
				else if(cp < 0x10000)
					count += 3;
				else
					count += 4;
			}
			while(*++ptr);
			
			char *str = new char[count + 1];
			char *r = utf8::utf32to8(string, ptr, str);
			*r = '\0';
			
			return String(new SharedString(count, count, 1, str));
		}
		
		String String::unicode(const wchar_t *string)
		{
			if(!string || !string[0])
				return String();
				
			const wchar_t *ptr = string;
			unsigned int count = 0;
			do
			{
				char32_t cp = *ptr;
				if(cp < 0x80)
					count += 1;
				else if(cp < 0x800)
					count += 2;
				else if(cp < 0x10000)
					count += 3;
				else
					count += 4;
			}
			while(*++ptr);
			
			char *str = new char[count + 1];
			char *r = utf8::utf32to8(string, ptr, str);
			*r = '\0';
			
			return String(new SharedString(count, count, 1, str));
		}
		
		String operator+ (char character, const String &string)
		{
			if(character == '\0')
				return string;
				
			if(string.isEmpty())
				return character;
				
			unsigned int totalSize = string._buffer->size + 1;
			char *str = new char[totalSize + 1];
			str[0] = character;
			std::memcpy(str, string._buffer->data, string._buffer->size + 1);
			
			return String(new String::SharedString(totalSize, totalSize, 1, str));
		}
		
		String operator+ (const char *string, const String &nstring)
		{
			if(!string || !string[0])
				return nstring;
				
			if(nstring.isEmpty())
				return string;
				
			unsigned int size = std::strlen(string);
			unsigned int totalSize = size + nstring._buffer->size;
			char *str = new char[totalSize + 1];
			std::memcpy(str, string, size);
			std::memcpy(&str[size], nstring._buffer->data, nstring._buffer->size + 1);
			
			return String(new String::SharedString(totalSize, totalSize, 1, str));
		}
		
		String operator+ (const std::string &string, const String &nstring)
		{
			if(string.empty())
				return nstring;
				
			if(nstring._buffer->size == 0)
				return string;
				
			unsigned int totalSize = string.size() + nstring._buffer->size;
			char *str = new char[totalSize + 1];
			std::memcpy(str, string.c_str(), string.size());
			std::memcpy(&str[string.size()], nstring._buffer->data, nstring._buffer->size + 1);
			
			return String(new String::SharedString(totalSize, totalSize, 1, str));
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