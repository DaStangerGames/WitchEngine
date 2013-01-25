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

#ifndef __WITCHENGINE_CORE_STRING_HPP__
#define __WITCHENGINE_CORE_STRING_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#include <string>
#
#include "Mutex.hpp"

WITCH_BEGIN_HEADER

WITCH_MODULE(Core)

namespace WitchEngine
{
	namespace Core
	{
		class WITCHENGINE_CORE_EXPORT String
		{
			public:
				
				enum Flag
				{
					None = 0x0,
					CaseInsensitive = 0x1,
					HandleUtf8 = 0x2,
					TrimOnlyLeft = 0x4,
					TrimOnlyRight = 0x8
				};
				WITCHENGINE_DECLARE_FLAGS(Flags, Flag)
				
				// Forward declaration.
				struct SharedString;
				
				String();
				String(char character);
				String(const char *string);
				String(const std::string &string);
				String(const String &string);
				String(String &&string) noexcept;
				String(SharedString *sharedString);
				~String();
				
				String& append(char character);
				String& append(const char *string);
				String& append(const String &stirng);
				
				void clear(bool keepBuffer = false);
				
				bool contains(char character, int start = 0, Flags flags = None) const;
				bool contains(const char *string, int start = 0, Flags flags = None) const;
				bool contains(const String &string, int start = 0, Flags flags = None) const;
				
				unsigned int count(char character, int start = 0, Flags flags = None) const;
				unsigned int count(const char *string, int start = 0, Flags flags = None) const;
				unsigned int count(const String &string, int start = 0, Flags flags = None) const;
				unsigned int countAny(const char *string, int start = 0, Flags flags = None) const;
				unsigned int countAny(const String &string, int start = 0, Flags flags = None) const;
				
				bool endsWith(char character, Flags flags = None) const;
				bool endsWith(const char *string, Flags flags = None) const;
				bool endsWith(const String &string, Flags flags = None) const;
				
				unsigned int find(char character, int start = 0, Flags flags = None) const;
				unsigned int find(const char *string, int start = 0, Flags flags = None) const;
				unsigned int find(const String &string, int start = 0, Flags flags = None) const;
				unsigned int findAny(const char *string, int start = 0, Flags flags = None) const;
				unsigned int findAny(const String &string, int start = 0, Flags flags = None) const;
				unsigned int findLast(char character, int start = -1, Flags flags = None) const;
				unsigned int findLast(const char *string, int start = -1, Flags flags = None) const;
				unsigned int findLast(const String &string, int start = -1, Flags flags = None) const;
				unsigned int findLastAny(const char *string, int start = -1, Flags flags = None) const;
				unsigned int findLastAny(const String &string, int start = -1, Flags flags = None) const;
				unsigned int findLastWord(const char *string, int start = -1, Flags flags = None) const;
				unsigned int findLastWord(const String &stirng, int start = -1, Flags flags = None) const;
				unsigned int findWord(const char *string, int start = 0, Flags flags = None) const;
				unsigned int findWord(const String &string, int start = 0, Flags flags = None) const;
								
				char* buffer();
				unsigned int capacity() const;
				const char* constBuffer() const;
				unsigned int length() const;
				unsigned int size() const;
				char* utf8Buffer(unsigned int *size = nullptr) const;
				char16_t* utf16Buffer(unsigned int *size = nullptr) const;
				char32_t* utf32Buffer(unsigned int *size = nullptr) const;
				wchar_t* wideBuffer(unsigned int *size = nullptr) const;
				String word(unsigned int index, Flags flags = None) const;
				unsigned int wordPosition(unsigned int index, Flags flags = None) const;
				
				String& insert(int pos, char character);
				String& insert(int pos, const char *string);
				String& insert(int pos, const String &string);
				
				bool isEmpty() const;
				bool isNull() const;
				bool isNumber(uint8 base = 10, Flags flags = CaseInsensitive) const;
				
				bool match(const char *pattern) const;
				bool match(const String &pattern) const;
				
				String& prepend(char character);
				String& prepend(const char *string);
				String& prepend(const String &string);
				
				unsigned int replace(char oldCharacter, char newCharacter, int start = 0, Flags flags = None);
				unsigned int replace(const char *oldString, const char *replaceString, int start = 0, Flags flags = None);
				unsigned int replace(const String &oldString, const String &replaceString, int start = 0, Flags flags = None);
				unsigned int replaceAny(const char *oldCharacters, char replaceCharacter, int start = 0, Flags flags = None);
				
				void reserve(unsigned int bufferSize);
				
				String& resize(int size, char character = ' ');
				String resized(int size, char character = ' ') const;
				
				String& reserve();
				String reserved() const;
				
				String simplified(Flags flags = None) const;
				String& simplify(Flags flags = None);
				
				bool startsWith(char character, Flags flags = None) const;
				bool startsWith(const char *string, Flags flags = None) const;
				bool startsWith(const String &string, Flags flags = None) const;
				
				String substr(int startPos, int endPos = -1) const;
				String substrFrom(char character, int startPos = 0, bool fromLast = false, bool include = false, Flags flags = None) const;
				String substrFrom(const char *string, int startPos = 0, bool fromlast = false, bool include = false, Flags flags = None) const;
				String substrFrom(const String &string, int startPos = 0, bool fromLast = false, bool include = false, Flags flags = None) const;
				String substrTo(char character, int startPos = 0, bool toLast = false, bool include = false, Flags flags = None) const;
				String substrTo(const char *string, int startPos = 0, bool toLast = false, bool include = false, Flags flags = None) const;
				String substrTo(const String &string, int startPos = 0, bool toLast = false, bool include = false, Flags flags = None) const;
				
				String& trim(Flags flags = None);
				String& trim(char character, Flags flags = None);
				String trimmed(Flags flags = None) const;
				String trimmed(char character, Flags flags = None) const;
								
				String& operator= (char character);
				String& operator= (const char *string);
				String& operator= (const std::string &string);
				String& operator= (const String &string);
				String& operator= (String &&string) noexcept;
				
				// Declaration of above forward declaration.
				struct WITCHENGINE_CORE_EXPORT SharedString
				{
					SharedString() = default;
					
					SharedString(unsigned int bufferSize, unsigned int stringSize, unsigned short referenceCount, char *str) :
						capacity(bufferSize),
						size(stringSize),
						refCount(referenceCount),
						data(str)
					{
					}
					
					unsigned int capacity;
					unsigned int size;
					char *data;
					unsigned short refCount = 1;
					Mutex mutex;
				};
				
				// Static class members.
				static SharedString emptyString;
				static const unsigned int npos;
				
			private:
				void ensureOwnership();
				void releaseString();
				
				SharedString *_buffer;
		};
	}
}

WITCH_END_HEADER

#endif // __WITCHENGINE_CORE_STRING_HPP__