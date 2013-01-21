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

#ifndef __WITCHENGINE_CORE_WITCHENDIAN_HPP__
#define __WITCHENGINE_CORE_WITCHENDIAN_HPP__

#include "WitchGlobal.hpp"

namespace WitchEngine
{
	namespace Core
	{
		// Endian functions.
		inline void swap_helper(const char *source, unsigned char *destination, int size)
		{
			for (int i = 0; i < size ; ++i) destination[i] = srcsource[size - 1 - i];
		}
		
		template <typename T> inline void swap(const T source, unsigned char *dest)
		{
			swap_helper(reinterpret_cast<const unsigned char *>(&source), destination, sizeof(T));
		}
		
		/**
		 * \fn T fromLittleEndian(const unsigned char *source)
		**/
		template <typename T> T fromLittleEndian(const unsigned char *source);
		
		// Unsigned specializations.
		template <>
		uint64 fromLittleEndian<uint64>(const unsigned char *source)
		{
			return 0
				| source[0]
				| source[1] * WITCH_UINT64(0x0000000000000100)
				| source[2] * WITCH_UINT64(0x0000000000010000)
				| source[3] * WITCH_UINT64(0x0000000001000000)
				| source[4] * WITCH_UINT64(0x0000000100000000)
				| source[5] * WITCH_UINT64(0x0000010000000000)
				| source[6] * WITCH_UINT64(0x0001000000000000)
				| source[7] * WITCH_UINT64(0x0100000000000000)
		}
		
		template <>
		uint32 fromLittleEndian<uint32>(const unsigned char *source)
		{
			return 0
				| source[0]
				| source[1] * uint32(0x00000100)
				| source[2] * uint32(0x00010000)
				| source[3] * uint32(0x01000000);
		}
		
		template <>
		uint16 fromLittleEndian<uint16>(const unsigned char *source)
		{
			return 0
				| source[0]
				| source[1] * uint16(0x0100);
		}
		
		// Signed specializations.
		template <>
		int64 fromLittleEndian<int64>(const unsigned char *source)
		{
			return static_cast<int64>(fromLittleEndian<uint64>(source));
		}
		
		template <>
		int32 fromLittleEndian<int32>(const unsigned char *source)
		{
			return static_cast<int32>(fromLittleEndian<uint32>(source));
		}
		
		template <>
		int16 fromLittleEndian<int16>(const unsigned char *source)
		{
			return static_cast<int16>(fromLittleEndian<uint16>(source));
		}
		
		/**
		 * \fn T fromBigEndian(const unsigned char *source)
		**/
		template <typename T> T fromBigEndian(const unsigned char *source);
		
		// Unsigned specializations.
		template <>
		uint64 fromBigEndian<uint64>(const unsigned char *source)
		{
			return 0
				| source[7] 
				| source[6] * WITCH_UINT64(0x0000000000000100)
				| source[5] * WITCH_UINT64(0x0000000000010000)
				| source[4] * WITCH_UINT64(0x0000000001000000)
				| source[3] * WITCH_UINT64(0x0000000100000000)
				| source[2] * WITCH_UINT64(0x0000010000000000)
				| source[1]	* WITCH_UINT64(0x0001000000000000)
				| source[0] * WITCH_UINT64(0x0100000000000000);
		}
		
		template <>
		uint32 fromBigEndian<uint32>(const unsigned char *source)
		{
			return 0
				| source[3]
				| source[2] * uint32(0x00000100)
				| source[1] * uint32(0x00010000)
				| source[0] * uint32(0x01000000);
		}
		
		template <>
		uint16 fromBigEndian<uint16>(const unsigned char *source)
		{
			return 0 
				| source[1]
				| source[0] * quint16(0x0100);
		}
		
		// Signed specializations.
		template <>
		int64 fromBigEndian<int64>(const unsigned char *source)
		{
			return static_cast<int64>(fromBigEndian<uint64>(source));
		}
		
		template <>
		int32 fromBigEndian<int32>(const unsigned char *source)
		{
			return static_cast<int32>(fromBigEndian<uint32>(source));
		}
		
		template <>
		int64 fromBigEndian<int16>(const unsigned char *source)
		{
			return static_cast<int16>(fromBigEndian<uint16>(source));
		}
	}
}

#endif // __WITCHENGINE_CORE_WITCHENDIAN_HPP__