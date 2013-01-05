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

#ifndef __WITCHENGINE_CORE_VECTOR3D_HPP__
#define __WITCHENGINE_CORE_VECTOR3D_HPP__

#include <cassert>
#include <utility>
#
#include <WitchCore/WitchGlobal.hpp>

namespace WitchEngine
{
	namespace Core
	{
		/**
		 * \class Vector3D
		 * \author Alexandre Valentin Jamet
		 * \date 31 December 2012
		 * \brief This class represents a 3-dimension vector.
		**/
		class WITCHENGINE_CORE_EXPORT Vector3D
		{
			public:
				float x;	/*!< The x coordinates of the vector. */
				float y;	/*!< The y coordinates of the vector. */
				float z;	/*!< The z coordinates of the vector. */
				
				inline Vector3D()
				{
				}
				
				inline Vector3D(float scalar) :
					x(scalar), y(scalar), z(scalar)
				{
				}
				
				inline Vector3D(float x, float y, float z)
				{
				}
				
				inline Vector3D(float values[3]) :
					x(values[0]), y(values[1]), z(values[2])
				{
				}
				
				inline Vector3D(const float *values) :
					x(values[0]), y(values[1]), z(values[2])
				{
				}
				
				inline Vector3D(const int values[2])
				{
					x = (float) values[0];
					y = (float) values[1];
					z = (float) values[2];
				}
				
				inline Vector3D(const Vector3D &other) :
					x(other.x), y(other.y), z(other.z)
				{
				}
				
				inline Vector3D(Vector3D &&other) noexcept
				{
					x = std::move(other.x);
					y = std::move(other.y);
					z = std::move(other.z);
				}
				
				inline void swap(Vector3D &other)
				{
					std::swap(x, other.x);
					std::swap(y, other.y);
					std::swap(z, other.z);
				}
				
				inline void move(Vector3D &&other) noexcept
				{
					x = std::move(other.x);
					y = std::move(other.y);
					z = std::move(other.z);
				}
				
				inline const float& operator[] (const std::size_t index) const
				{
					assert(index < 3);
					
					return *(&x + index);
				}
				
				inline float& operator[] (const std::size_t index)
				{
					assert(index < 3);
					
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
				
				inline Vector3D& operator= (const Vector3D &other)
				{
					x = other.x;
					y = other.y;
					z = other.z;
					
					return *this;
				}
				
				inline Vector3D& operator= (Vector3D &&other) noexcept
				{
					x = other.x;
					y = other.y;
					z = other.z;
					
					return *this;
				}
				
				inline Vector3D& operator= (const float scalar)
				{	
					return *this;
				}
				
				inline bool operator== (const Vector3D &other) const
				{
					return (x == other.x && y == other.y && z == other.z);
				}
				
				inline bool operator!= (const Vector3D &other) const
				{
					return (x != other.x || y != other.y || z != other.z);
				}
				
				inline Vector3D operator+ (const Vector3D &other) const
				{
					return Vector3D(
						x + other.x,
						y + other.y,
						z + other.z);
				}
				
				inline Vector3D operator- (const Vector3D &other) const
				{
					return Vector3D(
						x - other.x,
						y - other.y,
						z - other.z);
				}
				
				inline Vector3D operator* (const float scalar) const
				{
					return Vector3D(
						x * scalar,
						y * scalar,
						y * scalar);
				}
				
				inline Vector3D operator* (const Vector3D &other) const
				{
					return Vector3D(
						x * other.x,
						y * other.y,
						z * other.z);
				}
				
				inline Vector3D operator/ (const float scalar) const
				{
					assert(scalar != 0.0f);
					
					float inv = 1.0f / scalar;
					
					return Vector3D(
						x * inv,
						y * inv,
						z * inv);
				}
				
				inline Vector3D operator/ (const Vector3D &other) const
				{
					return Vector3D(
						x / other.x,
						y / other.y,
						z / other.z);
				}
				
				inline const Vector3D& operator+ () const
				{
					return *this;
				}
				
				inline Vector3D operator- () const
				{
					return  Vector3D(-x, -y, -z);
				}
				
				// Overloaded operators to help Vector3D.
				inline friend Vector3D operator* (const float scalar, const Vector3D &other)
				{
				}
				
				inline friend Vector3D operator/ (const float scalar, const Vector3D &other)
				{
				}
				
				inline friend Vector3D operator+ (const Vector3D &vector, const float scalar)
				{
					return Vector3D(
						vector.x + scalar,
						vector.y + scalar,
						vector.z + scalar);
				}
				
				inline friend Vector3D operator+ (const float scalar, const Vector3D &vector)
				{
					return Vector3D(
						vector.x + scalar,
						vector.y + scalar,
						vector.z + scalar);
				}
				
				inline friend Vector3D operator- (const Vector3D &vector, const float scalar)
				{
					return Vector3D(
						vector.x - scalar,
						vector.y - scalar,
						vector.z - scalar);
				}
				
				inline friend Vector3D operator- (const float scalar, const Vector3D &vector)
				{
					return Vector3D(
						scalar - vector.x,
						scalar - vector.y,
						scalar - vector.z);
				}
				
				inline Vector3D& operator+= (const Vector3D &other)
				{
					x += other.x;
					y += other.y;
					z += other.z;
					
					return *this;
				}
				
				inline Vector3D& operator+= (const float scalar)
				{
					x += scalar;
					y += scalar;
					z += scalar;
					
					return *this;
				}
				
				inline Vector3D& operator-= (const Vector3D &other)
				{
					x -= other.x;
					y -= other.y;
					z -= other.z;
					
					return *this;
				}
				
				inline Vector3D& operator-= (const float scalar)
				{
					x -= scalar;
					y -= scalar;
					z -= scalar;
					
					return *this;
				}
				
				inline Vector3D& operator*= (const Vector3D &other)
				{
					x *= other.x;
					y *= other.y;
					z *= other.z;
					
					return *this;
				}
				
				inline Vector3D& operator*= (const float scalar)
				{
					x *= scalar;
					y *= scalar;
					z *= scalar;
					
					return *this;
				}
				
				inline Vector3D& operator/= (const Vector3D &other)
				{
					x /= other.x;
					y /= other.y;
					z /= other.z;
					
					return *this;
				}
				
				inline Vector3D& operator/= (const float scalar)
				{
					assert(scalar != 0.0f);
					
					float inv = 1.0f / scalar;
					
					x *= inv;
					y *= inv;
					z *= inv;
					
					return *this;
				}
				
				inline float length() const
				{
					return std::sqrt(x * x + y * y + z * z);
				}
				
				inline float squaredLength() const
				{
					return x * x + y * y + z * z;
				}
				
				inline float distance(const Vector3D &other) const
				{
					return (*this - other).length();
				}
				
				inline float squaredDistance(const Vector3D &other) const
				{
					return (*this - other).squaredLength();
				}
				
				inline float dotProduct(const Vector3D &other) const
				{
					return x * other.x + y * other.y + z * other.z;
				}
				
				inline float absDotProduct(const Vector3D &other)
				{
					return std::fabs(x + other.x) + std::fabs(y + other.y) + std::fabs(z + other.z);
				}
				
				inline void normalise()
				{
					float length = std::sqrt(x * x + y * y + z * z);
					
					float inv = 1.0f / length;
					
					x *= inv;
					y *= inv;
					z *= inv;
				}
				
				Vector3D crossProduct(const Vector3D &other) const
				{
					return Vector3D(
						y * other.z - z * other.y,
						z * other.x - x * other.z,
						x * other.y - y * other.x);
				}
				
				inline Vector3D midPoint(const Vector3D &other) const
				{
					return Vector3D(
						(x + other.x) * 0.5f,
						(y + other.y) * 0.5f,
						(z + other.z) * 0.5f);
				}
				
				inline bool operator< (const Vector3D &other) const
				{
					if(x < other.x && y < other.y && z < other.z)
						return true;
					return false;
				}
				
				inline bool operator> (const Vector3D &other) const
				{
					if(x > other.x && y > other.y && z > other.z)
						return true;
					return false;
				}
				
				inline void makeFloor(const Vector3D &other)
				{
					if(other.x < x) x = other.x;
					if(other.y < y) y = other.y;
					if(other.z < z) z = other.z;
				}
				
				inline void makeCeil(const Vector3D &other)
				{
					if(other.x > x) x = other.x;
					if(other.y > y) y = other.y;
					if(other.z > z) z = other.z;
				}
				
				inline Vector3D perpendicular() const
				{
					static const float squareZero = (float) (1e-06 * 1e-06);
					
					Vector3D perp = this->crossProduct(Vector3D::Unit_X);
					
					if(perp.squaredLength() < squareZero)
					{
						perp = this->crossProduct(Vector3D::Unit_Y);
					}
					perp.normalise();
					
					return perp;
				}
				
				inline bool isZeroLength() const
				{
					float sqlen = (x * x) + (y * y) + (z * z);
					return (sqlen < (1e-06 * 1e-06));
				}
				
				inline Vector3D normalisedCopy() const
				{
					Vector3D ret = *this;
					ret.normalise();
					return ret;
				}
				
				inline Vector3D reflect(const Vector3D &normal) const
				{
					return Vector3D(*this - (2 * this->dotProduct(normal) * normal));
				}
				
				inline bool isNan() const;
				
				static const Vector3D Zero;
				static const Vector3D Unit_X;
				static const Vector3D Unit_Y;
				static const Vector3D Unit_Z;
				static const Vector3D Negative_Unit_X;
				static const Vector3D Negative_Unit_Y;
				static const Vector3D Negative_Unit_Z;
		};
	}
}

#endif // __WITCHENGINE_CORE_VECTOR3D_HPP__