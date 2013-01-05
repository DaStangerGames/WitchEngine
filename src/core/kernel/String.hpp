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

#ifndef __WITCHENGINE_CORE_STRING_HPP__
#define __WITCHENGINE_CORE_STRING_HPP__

#include <WitchCore/WitchGlobal.hpp>
#include <WitchCore/CustomAllocator.hpp>
#
#include <string>

namespace WitchEngine
{
	namespace Core
	{
		/**
		 * \class String
		 * \author Alexandre Valentin Jamet
		 * \date 23 December 2012
		 * \brief Framework implementation of the String container.
		**/
		class WITCHENGINE_CORE_EXPORT String
		{
			public:
				enum Flag
				{
					None =				0x00,
					CaseInsensitive =	0x01,
					HandleUtf8 =		0x02,
					TrimOnlyLeft =		0x04,
					TrimInlyRight =		0x08
				};
				WITCHENGINE_DECLARE_FLAGS(Flags, Flag)
				
				struct StringBuffer;	// Forward declaration of data container.
				
				String();
				String(char character);
				String(const char *string);
				String(const std::string &string);
				String(const String &string);
				String(String &&string) noexcept;
				String(StringBuffer *buffer);
				~String();
				
				String& append(char character);
				String& append(const char *string);
				String& append(const String &string);
				
				void clear(bool keepBuffer = false);
				
				bool contains(char character, int start = 0, Flags flags = None) const;
				bool contains(const char *string, int start = 0, Flags flags = None) const;
				bool contains(const String &string, int start = 0, Flags flags = None) const;
				
				uint32 count(char character, int start = 0, Flags flags = None) const;
				uint32 count(const char *string, int start = 0, Flags flags = None) const;
				uint32 count(const String &string, int start = 0, Flags flags = None) const;
				uint32 countAny(const char *string, int start = 0, Flags flags = None) const;
				uint32 countAny(const String &string, int start = 0, Flags flags = None) const;
				
				bool endsWith(char character, Flags flags = None) const;
				bool endsWith(const char *string, Flags flags = None) const;
				bool endsWith(const String &string, Flags flags = None) const;
				
				uint32 find(char charecter, int start = 0, Flags flags = None) const;
				uint32 find(const char *string, int start = 0, Flags flags = None) const;
				uint32 find(const String &string, int start = 0, Flags flags = None) const;
				uint32 findAny(const char *string, int start = 0, Flags flags = None) const;
				uint32 findAny(const String &string, int start = 0, Flags flags = None) const;
				uint32 findLast(char character, int start = -1, Flags flags = None) const;
				uint32 findLast(const char *string, int start = -1, Flags flags = None) const;
				uint32 findLast(const String &string, int start = -1, Flags flags = None) const;
				uint32 findLastAny(const char *string, int start = -1, Flags flags = None) const;
				uint32 findLastAny(const String &string, int start = -1, Flags flags = None) const;
				uint32 findLastWord(const char *string, int start = -1, Flags flags = None) const;
				uint32 findLastWord(const String &string, int start = 0, Flags flags = None) const;
				uint32 findWord(const char *string, int start = 0, Flags flags = None) const;
				uint32 findWord(const String &string, int start = 0, Flags flags = None) const;
				
				// Functions to directly access the raw data.
				char* buffer();
				uint32 capacity() const;
				const char* constBuffer() const;
				uint32 length() const;
				uint32 size() const;
				char* utf8Buffer(unsigned int *size = nullptr) const;
				char16_t* utf16Buffer(unsigned int *size = nullptr) const;
				char32_t* utf32Buffer(unsigned int *size = nullptr) const;
				wchar_t* wideBuffer(unsigned int *size = nullptr) const;
				
				String& insert(int pos, char character);
				String& insert(int pos, const char *string);
				String& insert(int pos, const String &string);
				
				bool isEmpty() const;
				bool isNull() const;
				
				String& prepend(char charecter);
				String& prepend(const char *string);
				String& prepend(const String &string);
				
				uint32 replace(char oldCharacter, char newCharacter, int start = 0, Flags flags = None);
				uint32 replace(const char *oldString, const char *newString, int start = 0, Flags flags = None);
				uint32 replace(const String &oldString, const String &newString, int start = 0, Flags flags = None);
				uint32 replaceAny(const char *oldCharacters, const char*newCharacters, int start = 0, Flags flags = None);
				
				bool startWith(char character, Flags flags = None) const;
				bool startWith(const char *string, Flags flags = None) const;
				bool startWith(const String &string, Flags flags = None) const;
				
				String substr(int startPos, int endPos = -1) const;
				String substrFrom(char character, int startPos = 0, bool fromLast = false, bool include = false, Flags flags = None) const;
				String substrFrom(const char *string, int startPos = 0, bool fromLast = false, bool include = false, Flags flags = None) const;
				String substrFrom(const String &string, int startPos = 0, bool fromLast = false, bool include = false, Flags flags = None) const;
				String substrTo(char character, int startPos = 0, bool toLast = false, bool include = false, Flags flags = None) const;
				String substrTo(const char *string, int startPos = 0, bool toLast = false, bool include = false, Flags flags = None) const;
				String substrTo(const String &string, int startPos = 0, bool toLast = false, bool include = false, Flags flags = None) const;
				
				// Standard methods.
				char* begin();
				const char* begin() const;
				char* end();
				const char* end() const;
				void push_front(char c);
				void push_back(char c);
				
				typedef char	value_type;
				typedef std::size_t size_type;
				typedef std::ptrdiff_t difference_type;
				typedef value_type&	reference;
				typedef const value_type& const_reference;
				typedef value_type* pointer;
				typedef const value_type* const_pointer;
				
				operator std::string() const;
				
				char& operator[] (uint32 index);
				char operator[] (uint32 index) const;
				
				String& operator= (char character);
				String& operator= (const char *string);
				String& operator= (const std::string &string);
				String& operator= (const String &string);
				String& operator= (String &&string) noexcept;
				
				String operator+ (char character) const;
				String operator+ (const char *string) const;
				String operator+ (const std::string &string) const;
				String operator+ (const String &string) const;
				
				String& operator+= (char character);
				String& operator+= (const char *string);
				String& operator+= (const std::string &string);
				String& operator+= (const String &string);
				
				bool operator== (char character) const;
				bool operator== (const char *string) const;
				bool operator== (const std::string &string) const;
				
				bool operator!= (char character) const;
				bool operator!= (const char *string) const;
				bool operator!= (const std::string &string) const;
				
				bool operator< (char character) const;
				bool operator< (const char *string) const;
				bool operator< (const std::string &string) const;
				
				bool operator<= (char character) const;
				bool operator<= (const char *string) const;
				bool operator<= (const std::string &string) const;
				
				bool operator> (char character) const;
				bool operator> (const char *string) const;
				bool operator> (const std::string &string) const;
				
				bool operator>= (char character) const;
				bool operator>= (const char *string) const;
				bool operator>= (const std::string &string) const;
				
				struct WITCHENGINE_CORE_EXPORT StringBuffer
				{
					StringBuffer() :
						referenceCounter(1),
						capacity(0),
						size(0),
						data(nullptr)
					{
					}
					
					StringBuffer(unsigned refCount, unsigned int bufferSize, unsigned int stringSize, char *string) :
						referenceCounter(refCount),
						capacity(bufferSize),
						size(stringSize),
						data(string)
					{
					}
					
					unsigned short referenceCounter;	/*!< Reference counting. */
					unsigned int capacity;				/*!< The size of buffer. */
					unsigned int size;					/*!< The size of the string. */
					char *data;							/*!< The string data. */
					
					/*
					 We need an allocator that can efficiently handle
					 the allocation of a lot of small objects.
					*/
					static SmallObjectAllocator<char> _allocator;
				};
				
				static StringBuffer emptyString;	/*!< Represents an empty buffer. */
				static const unsigned int npos;
				
				void releaseString();
				void ensureOwnership();
				
			private:
				StringBuffer *_buffer;	/*!< Pointer to the string's buffer. */
		};
	}
}

#endif // __WITCHENGINE_CORE_STRING_HPP__