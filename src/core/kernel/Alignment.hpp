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

#ifndef __WITCHENGINE_CORE_ALIGNMENT_HPP__
#define __WTICHENGINE_CORE_ALIGNMENT_HPP__

/*
 This file defines all tools and functionnality
 needed to correctly handle variables alignment.
 
 Features:
	- Alignment value for differents type.
*/
namespace WitchEngine
{
	namespace Core
	{
		/**
		 * \struct AlignementsTraits
		 * \author Alexandre Valentin Jamet
		 * \date 24 December 2012
		 * \brief This structure is a type traits which provides informations about the alignment of a specified type.
		 * This structure should be specialized for all needed type.
		**/
		template <typename Type> 
		struct AlignmentTraits
		{
			enum
			{
				Alignment = Type::Alignment	// Implcitly means that Type should define Alignment in her body otherwise a compile-time error while be generated.
			};
		};
		
#	define WITCHENGINE_SPECIFY_ALIGNMENT(Value) \
	enum \
	{ \
		Alignment = Value \
	}
		
		// Specialization for unsigned char.
		template <>
		struct AlignmentTraits<unsigned char>
		{
			enum
			{
				Alignment = 1
			};
		};
		
		// Specialization for unsigned char.
		template <>
		struct AlignmentTraits<char>
		{
			enum
			{
				Alignment = 1
			};
		};
		
		// Specialization for unsigned char.
		template <>
		struct AlignmentTraits<unsigned short>
		{
			enum
			{
				Alignment = 2
			};
		};
		
		// Specialization for unsigned char.
		template <>
		struct AlignmentTraits<short>
		{
			enum
			{
				Alignment = 2
			};
		};
		
		// Specialization for unsigned char.
		template <>
		struct AlignmentTraits<unsigned int>
		{
			enum
			{
				Alignment = 4
			};
		};
		
		// Specialization for unsigned char.
		template <>
		struct AlignmentTraits<int>
		{
			enum
			{
				Alignment = 4
			};
		};
		
		// Specialization for unsigned char.
		template <>
		struct AlignmentTraits<float>
		{
			enum
			{
				Alignment = 4
			};
		};
		
		// Specialization for unsigned char.
		template <>
		struct AlignmentTraits<double>
		{
			enum
			{
				Alignment = 4
			};
		};
	}
}

#endif // __WITCHENGINE_CORE_ALIGNMENT_HPP__