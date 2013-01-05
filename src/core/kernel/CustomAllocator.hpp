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

#ifndef __WITCHENGINE_CORE_CUSTOMALLOCATOR_HPP__
#define __WITCHENGINE_CORE_CUSTOMALLOCATOR_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#include <cstdlib>
#include <cstddef>
#include <utility>
#include <limits>
#include <memory>
#
#include "Alignment.hpp"
#
#include <WitchCore/TrackerMemoryAllocator.hpp>

namespace WitchEngine
{
	namespace Core
	{
		/**
		 * \class StandardAllocationPolicy
		 * \author Alexandre Valentin Jamet
		 * \date 23 December 2012
		 * \brief This allocator is a reimplementation of std::allocator.
		**/
		template <typename T>
		struct StandardAllocationPolicy
		{
			typedef T value_type;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
			
			template <typename U>
			struct rebind
			{
				typedef StandardAllocationPolicy<U> other;
			};
			
			inline explicit StandardAllocationPolicy() noexcept { };
			inline ~StandardAllocationPolicy() noexcept { };
			inline explicit StandardAllocationPolicy(const StandardAllocationPolicy &) noexcept { }
			template <typename U>
			inline explicit StandardAllocationPolicy(const StandardAllocationPolicy<U> &) noexcept { }
			
			inline pointer allocate(size_type count, typename std::allocator<void>::const_pointer hint = 0)
			{
				return reinterpret_cast<pointer>(::operator new(count * sizeof(T)));
			}
			
			inline void deallocate(pointer p, size_type)
			{
				::operator delete(p);
			}
			
			inline size_type max_size() const
			{
				return std::numeric_limits<size_type>::max();
			}
		};
		
		template <typename T, typename S>
		inline bool operator ==(const StandardAllocationPolicy<T> &, const StandardAllocationPolicy<S> &)
		{
			return true;
		}
		
		template <typename T, typename OtherAllocator>
		inline bool operator ==(const StandardAllocationPolicy<T> &, const OtherAllocator &)
		{
			return false;
		}
	
		/**
		 * \class DebugAllocationPolicy
		 * \author Alexandre Valentin Jamet
		 * \date 23 December 2012
		 * \brief This allocator is a specific to the debug mode, logging information about allocation and deallocation.
		**/
		template <typename T>
		struct DebugAllocationPolicy
		{
			typedef T value_type;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
			
			template <typename U>
			struct rebind
			{
				typedef DebugAllocationPolicy<U> other;
			};
			
			// Declaring the constructors and the destructor.
			inline explicit DebugAllocationPolicy() noexcept { }
			inline ~DebugAllocationPolicy() noexcept { }
			inline explicit DebugAllocationPolicy(const DebugAllocationPolicy &other) noexcept { }
			template <typename U>
			inline explicit DebugAllocationPolicy(const DebugAllocationPolicy<U> &other) noexcept { }
			
			inline pointer allocate(size_type count, typename std::allocator<void>::const_pointer hint = 0)
			{
				// Allocating memory space.
				return TrackerMemoryAllocator::instance()->allocate(count * sizeof(T));
			}
			
			// Override version of the allocate function.
			inline pointer allocate(std::size_t size, uint32 line, const char *file, const char *function, bool array)
			{
				return TrackerMemoryAllocator::instance()->allocate(size, line, file, function, array);
			}
			
			inline void deallocate(pointer p, size_type)
			{
				// Deallocating memory space.
				TrackerMemoryAllocator::instance()->free(p);
			}
			
			// Accessing memory usage statictics.
			size_type totalUsedMemory()
			{
				return TrackerMemoryAllocator::instance()->currentlyUsedMemory();
			}
			
			size_type maxUsedMemory()
			{
				return TrackerMemoryAllocator::instance()->maxAllocatedMemory();
			}
		};
		
		template <typename T, typename S>
		inline bool operator ==(const DebugAllocationPolicy<T> &, const DebugAllocationPolicy<S> &)
		{
			return true;
		}
		
		template <typename T, typename OtherAllocator>
		inline bool operator ==(const DebugAllocationPolicy<T> &, const OtherAllocator &)
		{
			return false;
		}
		
		/**
		 * \class SmallObjectAllocationPolicy
		**/
		template <typename T>
		struct SmallObjectAllocationPolicy
		{
			typedef T value_type;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
			
			template <typename U>
			struct rebind
			{
				typedef SmallObjectAllocationPolicy<U> other;
			};
			
			inline explicit SmallObjectAllocationPolicy() { }
			inline ~SmallObjectAllocationPolicy() { }
			inline explicit SmallObjectAllocationPolicy(const SmallObjectAllocationPolicy &) { }
			template <typename U>
			inline explicit SmallObjectAllocationPolicy(const SmallObjectAllocationPolicy<U> &) { }
			
			inline pointer allocate(size_type count, typename std::allocator<void>::const_pointer hint = 0)
			{
			}
			
			inline void deallocate(pointer p, size_type count)
			{
			}
			
			inline size_type max_size() const
			{
				return std::numeric_limits<size_type>::max() / sizeof(value_type);
			}
		};
		
		template <typename T, typename U>
		inline bool operator ==(const SmallObjectAllocationPolicy<T> &, const SmallObjectAllocationPolicy<U> &)
		{
			return true;
		}
		
		template <typename T, typename OtherAllocator>
		inline bool operator ==(const SmallObjectAllocationPolicy<T> &, const OtherAllocator &)
		{
			return false;
		}
		
		/**
		 * \class AlignedAllocationPolicy
		 * \author Alexandre Valentin Jamet
		 * \date 24 December 2012
		 * \brief This allocator allows you to allocate aligned data.
		**/
		template <typename T>
		struct AlignedAllocationPolicy
		{
			typedef T value_type;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
			
			template <typename U>
			struct rebind
			{
				typedef AlignedAllocationPolicy<U> other;
			};
			
			inline explicit AlignedAllocationPolicy() { }
			inline ~AlignedAllocationPolicy() { }
			inline explicit AlignedAllocationPolicy(const AlignedAllocationPolicy &) { }
			template <typename U>
			inline explicit AlignedAllocationPolicy(const AlignedAllocationPolicy<U> &) { }
			
			inline pointer allocate(size_type size, typename std::allocator<void>::const_pointer hint = 0)
			{
				// We need to allocate a enough memory (size + alignment).
				void *mem = ::std::malloc(size + AlignmentTraits<value_type>::Alignment);
				void **ptr = (void **)((long)(mem + AlignmentTraits<value_type>::Alignment) & ~(AlignmentTraits<value_type>::Alignment - 1));
				
				ptr[-1] = mem;
				
				return reinterpret_cast<pointer>(ptr);
			}
			
			inline void deallocate(pointer p, size_type count)
			{
				// Because of the alignment the pointer doesn't refers to the beginning of the allocated memory space.
				::std::free(((void **)p)[-1]);
			}
			
			inline size_type max_size() const
			{
				return std::numeric_limits<size_type>::max();
			}
		};
		
		template <typename T, typename U>
		inline bool operator ==(const AlignedAllocationPolicy<T> &, const AlignedAllocationPolicy<U> &)
		{
			return true;
		}
		
		template <typename T, typename OtherAllocator>
		inline bool operator ==(const AlignedAllocationPolicy<T> &, const OtherAllocator &)
		{
			return false;
		}
		
		// Modular Allocator template class.
#if WITCHENGINE_DEBUG_MODE == 0x0
		template <typename T, typename Policy = StandardAllocationPolicy<T> >
#else
		template <typename T, typename Policy = DebugAllocationPolicy<T> >
#endif
		struct Allocator : public Policy
		{
			private:
				typedef Policy AllocationPolicy;
				
			public:
				typedef typename AllocationPolicy::size_type size_type;
				typedef typename AllocationPolicy::difference_type difference_type;
				typedef typename AllocationPolicy::pointer pointer;
				typedef typename AllocationPolicy::const_pointer const_pointer;
				typedef typename AllocationPolicy::reference reference;
				typedef typename AllocationPolicy::const_reference const_reference;
				typedef typename AllocationPolicy::value_type value_type;
				
				template <typename U>
				struct rebind
				{
					// typedef Allocator<U, typename AllocationPolicy::rebind<U>::other> other;
				};
				
				inline explicit Allocator() noexcept { }
				inline ~Allocator() noexcept { }
				inline Allocator(const Allocator &other) noexcept : Policy(other) { }
				template <typename U>
				inline Allocator(const Allocator &) noexcept { }
				template <typename U, typename P>
				inline Allocator(const Allocator<U, P> &other) noexcept : Policy(other) { }
				
				inline pointer address(reference x) const noexcept
				{
					return &x;
				}
				
				inline const_pointer adress(const_reference x) const noexcept
				{
					return &x;
				}
				
				template <typename U, typename... Args>
				void construct(U* p, Args&&... args)
				{
					::new ((void *) p) U(std::forward<Args>(args)...);
				}
				
				template <typename U>
				void destroy(U *p)
				{
					p->~U();
				}
		};
		
		// Declaring aliases to custom allocators.
		template <typename Type> using StandardAllocator = Allocator<Type, StandardAllocationPolicy<Type> >;
		template <typename Type> using DebugAllocator = Allocator<Type, DebugAllocationPolicy<Type> >;
		template <typename Type> using SmallObjectAllocator = Allocator<Type, SmallObjectAllocationPolicy<Type> >;
		template <typename Type> using AlignedAllocator = Allocator<Type, AlignedAllocationPolicy<Type> >;
	}
}

#endif // __WITCHENGINE_CORE_CUSTOMALLOCATOR_HPP__