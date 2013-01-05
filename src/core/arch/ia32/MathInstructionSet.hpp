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

#ifndef __WITCHENGINE_CORE_MATHINSTRUCTIONSET_HPP__
#define __WITCHENGINE_CORE_MATHINSTRUCTIONSET_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#include <smmintrin.h>	// Including the SSE 4.2 functions.

namespace WitchEngine
{
	namespace Core
	{
		class WITCHENGINE_CORE_EXPORT SIMDEncapsulation
		{
			public:
				SIMDEncapsulation()
				{
					_data = _mm_setzero_ps();
				}
				
				SIMDEncapsulation(float scalar)
				{
					_data = _mm_set_ps1(scalar);
				}
				
				SIMDEncapsulation(float x, float y, float z, float w)
				{
					_data = _mm_set_ps(x, y, z, w);
				}
				
				SIMDEncapsulation(float values[4])
				{
					_data = _mm_load_ps(values);
				}
				
				SIMDEncapsulation(const float *values)
				{
					_data = _mm_load_ps(values);
				}
				
				SIMDEncapsulation(const SIMDEncapsulation &other) :
					_data(other._data)
				{
				}
				
				SIMDEncapsulation(SIMDEncapsulation &&other) noexcept :
					_data(other._data)
				{
					_other._data = _mm_setzero_ps();
				}
						
				SIMDEncapsulation& operator= (const SIMDEncapsulation &other)
				{
					_data = other._data;
					
					return *this;
				}
				
				SIMDEncapsulation& operator= (SIMDEncapsulation &&other) noexcept
				{
					_data = other._data;
					other._data = _mm_setzero_ps();
					
					return *this;
				}
				
				SIMDEncapsulation& operator= (const float *values)
				{
					_data = _mm_load_ps(values);
					return *this;
				}
				
				SIMDEncapsulation& operator= (float scalar)
				{
					_data = _mm_set_ps1(scalar);
					
					return *this;
				}
				
				// Arithmetics operators.
				SIMDEncapsulation operator+ (const SIMDEncapsulation &other)
				{
					return _mm_add_ps(_data, other._data);
				}
				
				SIMDEncapsulation operator- (const SIMDEncapsulation &other)
				{
					return _mm_sub_ps(_data, other._data);
				}
				
				SIMDEncapsulation operator* (const SIMDEncapsulation &other)
				{
					return _mm_mul_ps(_data, other._data);
				}
				
				SIMDEncapsulation operator/ (const SIMDEncapsulation &other)
				{
					return _mm_div_ps(_data, other._data);
				}
				
				// Update arithmetic operators.
				SIMDEncapsulation& operator+= (const SIMDEncapsulation &other)
				{
					_data = _mm_add_ps(_data, other._data);
					
					return *this;
				}
				
				SIMDEncapsulation& operator-= (const SIMDEncapsulation &other)
				{
					_data = _mm_sub_ps(_data, other._data);
					
					return *this;
				}
				
				SIMDEncapsulation& operator*= (const SIMDEncapsulation &other)
				{
					_data = _mm_mul_ps(_data, other._data);
					
					return *this;
				}
				
				SIMDEncapsulation& operator/= (const SIMDEncapsulation &other)
				{
					_data = _mm_div_ps(_data, other._data);
					
					return *this;
				}
				
				// Comparison operators.
				bool operator== (const SIMDEncapsulation &other)
				{
					
				}
				
				bool operator!= (const SIMDEncapsulation &other)
				{
				}
				
				bool operator<= (const SIMDEncapsulation &other)
				{
				}
				
				bool operator>= (const SIMDEncapsulation &other)
				{
				}
				
				bool operator< (const SIMDEncapsulation &other)
				{
				}
				
				bool operator> (const SIMDEncapsulation &other)
				{
				}
				
				// Logical operators.
				SIMDEncapsulation operator& (const SIMDEncapsulation &other)
				{
				}
				
				SIMDEncapsulation operator| (const SIMDEncapsulation &other)
				{
				}
				
				SIMDEncapsulation operator~ (const SIMDEncapsulation &other)
				{
				}
				
				SIMDEncapsulation operator^ (const SIMDEncapsulation &other)
				{
				}
				
				// Update logical operators.
				SIMDEncapsulation& operator&= (const SIMDEncapsulation &other)
				{
					return *this;
				}
				
				SIMDEncapsulation& operator|= (const SIMDEncapsulation &other)
				{
					return *this;
				}
				
				SIMDEncapsulation& operator~= (const SIMDEncapsulation &other)
				{
					return *this;
				}
				SIMDEncapsulation& operator^= (const SIMDEncapsulation &other)
				{
					return *this;
				}
				
			private:
				__m128 _data;
		};
	}
}

#endif // __WITCHENGINE_CORE_MATHINSTRUCTIONSET_HPP__