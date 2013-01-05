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

#ifndef __WITCHENGINE_CORE_VECTOR2D_HPP__
#define __WITCHENGINE_CORE_VECTOR2D_HPP__

#include <cassert>
#include <utility>
#
#include <WitchCore/WitchGlobal.hpp>

namespace WitchEngine
{
	namespace Core
	{
		/**
		 * \class Vector2D
		 * \author Alexandre Valentin Jamet
		 * \date 29 December 2012
		 * \brief This class represents a 2-dimension vector.
		**/
		class WITCHENGINE_CORE_EXPORT Vector2D
		{
			public:
				float x;	/*!< The x coordinates of the vector. */
				float y;	/*!< The y coordinates of the vector. */
				
				inline Vector2D()
				{
				}
				
				inline Vector2D(float scalar)
					: x(scalar), y(scalar)
				{
				}
				
				inline Vector2D(float fX, float fY) 
				: x(fX), y(fY)
				{
				}
				
				inline Vector2D(float values[2])
					: x(values[0]), y(values[1])
				{
				}
				
				inline Vector2D(const float *values)
					: x(values[0]), y(values[1])
				{
				}
				
				inline Vector2D(const int values[2])
				{
					x = (float) values[0];
					y = (float) values[1];
				}
				
				inline Vector2D(const Vector2D &other)
					: x(other.x), y(other.y)
				{
				}
				
				inline Vector2D(Vector2D &&other) noexcept
				{
					x = std::move(other.x);
					y = std::move(other.y);
				}
				
				inline void swap(Vector2D &other)
				{
					std::swap(x, other.x);
					std::swap(y, other.y);
				}
				
				inline void move(Vector2D &&other) noexcept
				{
					x = std::move(other.x);
					y = std::move(other.y);
				}
				
				inline const float& operator[] (const std::size_t index) const
				{
					assert(index < 2);
					
					return *(&x + index);
				}
				
				inline float& operator[] (const std::size_t index)
				{
					assert(index < 2);
					
					return *(&x + index);
				}
				
				inline float* pointer()
				{
					return &x;
				}
				
				inline const float* pointer() const
				{
					return &x;
				}
				
				inline Vector2D& operator= (const Vector2D &other)
				{
					x = other.x;
					y = other.y;
					
					return *this;
				}
				
				inline Vector2D& operator= (Vector2D &&other) noexcept
				{
					x = std::move(other.x);
					y = std::move(other.y);
					
					return *this;
				}
				
				inline Vector2D& operator= (const float scalar)
				{
					x = scalar;
					y = scalar;
					
					return *this;
				}
				
				inline bool operator== (const Vector2D &other) const
				{
					return (x == other.x && y == other.y);
				}
				
				inline bool operator!= (const Vector2D &other) const
				{
					return (x != other.x || y != other.y);
				}
				
				inline Vector2D operator+ (const Vector2D &other) const
				{
					return Vector2D(
						x + other.x,
						y + other.y);
				}
				
				inline Vector2D operator- (const Vector2D &other) const
				{
					return Vector2D(
						x - other.x,
						y - other.y);
				}
				
				inline Vector2D operator* (const float scalar) const
				{
					return Vector2D(
						x * scalar,
						y * scalar);
				}
				
				inline Vector2D operator* (const Vector2D &other) const
				{
					return Vector2D(
						x * other.x,
						y * other.y);
				}
				
				inline Vector2D operator/ (const float scalar) const
				{
					assert(scalar != 0.0f);
					
					float inv = 1.0f / scalar;
					
					return Vector2D(
						x * inv,
						y * inv);
				}
				
				inline Vector2D operator/ (const Vector2D &other) const
				{
					return Vector2D(
						x / other.x,
						y / other.y);
				}
				
				inline const Vector2D& operator+ () const
				{
					return *this;
				}
				
				inline Vector2D operator- () const
				{
					return Vector2D(-x, -y);
				}
				
				// Overloaded operators to help Vector2D.
				inline friend Vector2D operator* (const float scalar, const Vector2D &vector)
				{
					return Vector2D(
						scalar * vector.x,
						scalar * vector.y);
				}
				
				inline friend Vector2D operator/ (const float scalar, const Vector2D &vector)
				{
					return Vector2D(
						scalar / vector.x,
						scalar / vector.y);
				}
				
				inline friend Vector2D operator+ (const Vector2D &vector, const float scalar)
				{
					return Vector2D(
						vector.x + scalar,
						vector.y + scalar);
				}
				
				inline friend Vector2D operator+ (const float scalar, const Vector2D &vector)
				{
					return Vector2D(
						scalar + vector.x,
						scalar + vector.y);
				}
				
				inline friend Vector2D operator- (const Vector2D &vector, const float scalar)
				{
					return Vector2D(
						vector.x - scalar,
						vector.y - scalar);
				}
				
				inline friend Vector2D operator- (const float scalar, const Vector2D &vector)
				{
					return Vector2D(
						scalar - vector.x,
						scalar - vector.y);
				}
				
				inline Vector2D& operator+= (const Vector2D &other)
				{
					x += other.x;
					y += other.y;
					
					return *this;
				}
				
				inline Vector2D& operator+= (const float scalar)
				{
					x += scalar;
					y += scalar;
					
					return *this;
				}
				
				inline Vector2D& operator-= (const Vector2D &other)
				{
					x -= other.x;
					y -= other.y;
					
					return *this;
				}
				
				inline Vector2D& operator-= (const float scalar)
				{
					x -= scalar;
					y -= scalar;
					
					return *this;
				}
				
				inline Vector2D& operator*= (const Vector2D &other)
				{
					x *= other.x;
					y *= other.y;
					
					return *this;
				}
				
				inline Vector2D& operator*= (const float scalar)
				{
					x *= scalar;
					y *= scalar;
					
					return *this;
				}
				
				inline Vector2D& operator/= (const Vector2D &other)
				{
					x /= other.x;
					y /= other.y;
					
					return *this;
				}
				
				inline Vector2D& operator/= (const float scalar)
				{
					assert(scalar != 0.0f);
					
					float inv = 1.0f / scalar;
					
					x *= inv;
					y *= inv;
					
					return *this;
				}
				
				inline float length() const
				{
					return std::sqrt(x * x + y * y);
				}
				
				inline float squaredLength() const
				{
					return x * x + y * y;
				}
				
				inline float distance(const Vector2D &other) const
				{
					return (*this - other).length();
				}
				
				inline float squaredDistance(const Vector2D &other) const
				{
					return (*this - other).squaredLength();
				}
				
				inline float dotProduct(const Vector2D &other) const
				{
					return x * other.x + y * other.y;
				}
				
				inline void normalise()
				{
					float length = std::sqrt(x * x + y * y);
					
					float inv = 1.0f / length;
					
					x *= inv;
					y *= inv;
				}
				
				inline Vector2D midPoint(const Vector2D &other) const
				{
					return Vector2D(
						(x + other.x) * 0.5f,
						(y + other.y) * 0.5f);
				}
				
				
				inline bool operator< (const Vector2D &other) const
				{
					if(x < other.x && y < other.y)
						return true;
					return false;
				}
				
				inline bool operator> (const Vector2D &other) const
				{
					if(x > other.x && y > other.y)
						return true;
					return false;
				}
				
				inline void makeFloor(const Vector2D &other)
				{
					if(other.x < x) x = other.x;
					if(other.y < y) y = other.y;
				}
				
				inline void makeCeil(const Vector2D &other)
				{
					if(other.x > x) x = other.x;
					if(other.y > y) y = other.y;
				}
				
				inline Vector2D perpendicular() const
				{
					return Vector2D(-y, x);
				}
				
				inline float crossProduct(const Vector2D &other) const
				{
					return x * other.y - y * other.x;
				}
				
				inline bool isZeroLength() const
				{
					float sqlen = (x * x) + (y * y);
					return (sqlen < (1e-06 * 1e-05));
				}
				
				inline Vector2D normaliseCopy() const
				{
					Vector2D ret = *this;
					ret.normalise();
					return ret;
				}
				
				inline Vector2D reflect(const Vector2D &normal) const
				{
					return Vector2D(*this - (2 * this->dotProduct(normal) * normal));
				}
				
				inline bool isNaN() const;
				
				static const Vector2D Zero;
				static const Vector2D Unit_X;
				static const Vector2D Unit_Y;
				static const Vector2D Negative_Unit_X;
				static const Vector2D Negative_Unit_Y;
		};
	}
}

#endif // __WITCHENGINE_CORE_VECTOR2D_HPP__