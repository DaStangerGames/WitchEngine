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

#include "../../3rdparty/utf8/utf8.h"
#
#include <WitchCore/String.hpp>
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
		
		inline unsigned int getNewSize(unsigned int newSize)
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
		
		String::StringBuffer String::emptyString(0, 0, 0, nullptr);
		const unsigned int String::npos(std::numeric_limits<unsigned int>::max());
		SmallObjectAllocator<char> String::StringBuffer::_allocator = SmallObjectAllocator<char>();
		
		String::String() :
			_buffer(&emptyString)
		{
		}
		
		String::String(char character)
		{
			if(character = '\0')
				_buffer = &emptyString;
			else
			{
				_buffer = new StringBuffer;
				_buffer->capacity = 1;
				_buffer->size = 1;
				_buffer->data = StringBuffer::_allocator.allocate(2);
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
					_buffer = new StringBuffer;
					_buffer->capacity = size;
					_buffer->size = size;
					_buffer->data = StringBuffer::_allocator.allocate(size + 1);
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
				_buffer = new StringBuffer;
				_buffer->capacity = string.capacity();
				_buffer->size = string.size();
				_buffer->data = StringBuffer::_allocator.allocate(string.capacity() + 1);
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
				// Locking access.
				_buffer->referenceCounter++;
				// Unlocking access.
			}
		}
		
		String::String(String &&string) noexcept :
			_buffer(string._buffer)
		{
			string._buffer = &emptyString;
		}
		
		String::String(StringBuffer *buffer) :
			_buffer(buffer)
		{
		}
		
		String::~String()
		{
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
				unsigned int newSize = _buffer->size + 1;
				unsigned int bufferSize = getNewSize(newSize);
				
				char *str = StringBuffer::_allocator.allocate(bufferSize + 1);
				std::memcpy(str, _buffer->data, _buffer->size);
				str[_buffer->size] = character;
				str[newSize] = '\0';
				
				releaseString();
				_buffer = new StringBuffer;
				_buffer->capacity = bufferSize;
				_buffer->size = newSize;
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
				unsigned int newSize = _buffer->size + length;
				unsigned int bufferSize = getNewSize(newSize);
				
				char *str = StringBuffer::_allocator.allocate(bufferSize + 1);
				
				std::memcpy(str, _buffer->data, _buffer->size);
				std::memcpy(&str[_buffer->size], string, length + 1);
				
				releaseString();
				
				_buffer = new StringBuffer;
				_buffer->capacity = bufferSize;
				_buffer->size = newSize;
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
				unsigned int newSize = _buffer->size + string._buffer->size;
				unsigned int bufferSize = getNewSize(newSize);
				
				char *str = StringBuffer::_allocator.allocate(bufferSize + 1);
				std::memcpy(str, _buffer->data, _buffer->size);
				std::memcpy(&str[_buffer->size], string._buffer->data, string._buffer->size + 1);
				
				releaseString();
				
				_buffer = new StringBuffer;
				_buffer->capacity = bufferSize;
				_buffer->size = newSize;
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
					char character_lower = toLower(character);
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
		}
		
		unsigned String::count(const char *string, int start, Flags flags) const
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
					while(utf8::internal::is_trail(*str))
						str++;
						
					utf8::unchecked::iterator<const char *> it(str);
					
					const char *t = string;
					char32_t c;
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
					while(utf8::internal::is_trail(*str))
						str++;
						
					utf8::unchecked::iterator<const char *> it(str);
					
					const char *t = string._buffer->data;
					char32_t c;
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
		
		unsigned int String::countAny(const char *string, int start, Flags flags) const
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
				
				if(flags & CaseInsensitive)
				{
					do
					{
					}
					while(*++str);
				}
				else
				{
					do
					{
						utf8::unchecked::iterator<const char *> it2(string);
						do
						{
							if(*it == *it2)
							{
								count++;
								break;
							}
						}
						while(*++it2);
					}
					while(*++str);
				}
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
						while (*++c);
					}
					while(*++str);
				}
				else
				{
					while((str = std::strpbrk(str, string)))
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
				
				if(flags & CaseInsensitive)
				{
					do
					{
						utf8::unchecked::iterator<const char *> it2(string._buffer->data);
						do
						{
						}
						while(*++it2);
					}
					while(*++str);
				}
				else
				{
					do
					{
						utf8::unchecked::iterator<const char *> it2(string._buffer->data);
						do
						{
							if(*it == *it2)
							{
								count++;
								break;
							}
						}
						while(*++it2);
					}
					while(*++str);
				}
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
					while((str = std::strpbrk(str, string._buffer->data)))
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
				if(flags & HandleUtf8)
				{
				}
				else
				{
				}
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
				if(flags & HandleUtf8)
				{
				}
				else
				{
				}
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
					return static_cast<unsigned int>(ch - _buffer->data);
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
					
					const char *t;
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
					char32_t c;
					do
					{
					}
					while(*++it);
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
				
			char* str = &_buffer->data[pos];
			if(flags & HandleUtf8)
			{
				while(utf8::internal::is_trail(*str))
					str++;
					
				utf8::unchecked::iterator<const char *> it(str);
				
				if(flags & CaseInsensitive)
				{
					do
					{
					}
					while(*++it);
				}
				else
				{
					do
					{
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
					}
					while(*++it);
				}
				else
				{
					do
					{
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
						const char *c = string._buffer->data;
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
		
		char* String::buffer()
		{
			return _buffer->data;
		}
		
		const char* String::constBuffer() const
		{
			return _buffer->data;
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
					_buffer = new StringBuffer;
					_buffer->capacity = 1;
					_buffer->data = StringBuffer::_allocator.allocate(2);
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
					
					_buffer = new StringBuffer;
					_buffer->capacity = size;
					_buffer->data = StringBuffer::_allocator.allocate(size + 1);
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
					
					_buffer = new StringBuffer;
					_buffer->capacity = string.size();
					_buffer->data = StringBuffer::_allocator.allocate(string.size() + 1);
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
				// Locking the buffer.
				_buffer->referenceCounter++;
				// Unlocking the buffer.
			}
			
			return *this;
		}
		
		String& String::operator= (String &&string) noexcept
		{
			_buffer = string._buffer;
			string._buffer = &emptyString;
			
			return *this;
		}
		
		void String::ensureOwnership()
		{
			if(_buffer == &emptyString)
				return;
				
			// Locking the buffer.
			if(_buffer->referenceCounter > 1)
			{
				_buffer->referenceCounter--;
				
				char *string = StringBuffer::_allocator.allocate(_buffer->capacity + 1);
				std::memcpy(string, _buffer->data, _buffer->size + 1);
				
				_buffer = new StringBuffer(1, _buffer->capacity, _buffer->size, string);
			}
		}
		
		void String::releaseString()
		{
			if(_buffer == &emptyString)
				return;
				
			// Locking the buffer.
			bool freeSharedString = (--_buffer->referenceCounter == 0);
			// Unlocking the buffer.
			
			if(freeSharedString)
			{
				StringBuffer::_allocator.deallocate(_buffer->data, _buffer->capacity);
				delete _buffer;
			}
			
			_buffer = &emptyString;
		}
	}
}