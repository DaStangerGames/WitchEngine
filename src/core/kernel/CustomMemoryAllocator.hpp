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

#ifndef __WITCHENGINE_CORE_CUSTOMMEMORYALLOCATOR_HPP__
#define __WITCHENGINE_CORE_CUSTOMMEMORYALLOCATOR_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#include "DebugMemoryManager.hpp"
#
#include <utility>

WITCH_BEGIN_HEADER

WITCH_MODULE(Core)

namespace WitchEngine
{
	namespace Core
	{
		/**
		 * \struct StandardAllocator
		**/
		template <typename Type>
		struct StandardAllocator
		{
			/* Defining to match with the Standard template library. */
			typedef Type value_type;
			typedef Type* pointer;
			typedef const Type* const_pointer;
			typedef Type& reference;
			typedef const Type& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
			
			template <typename OtherType>
			struct rebind
			{
				typedef StandardAllocator<OtherType> other;
			};
			
			/* Function declarations. */
			inline explicit StandardAllocator()
			{
			}
			
			inline explicit StandardAllocator(const StandardAllocator &other)
			{
			}
			
			template <typename OtherType>
			inline explicit StandardAllocator(const StandardAllocator<OtherType> &other)
			{
			}
			
			inline ~StandardAllocator()
			{
			}
			
			inline pointer address(reference object) noexcept
			{
				return &object;
			}
			
			inline const_pointer address(const_reference object) noexcept
			{
				return &object;
			}
			
			inline pointer allocate(size_type n)
			{
				return reinterpret_cast<pointer>(::operator new(n * sizeof(Type)));
			}
			
			inline void deallocate(pointer p, size_type)
			{
				::operator delete(p);
			}
			
			inline size_type max_size() const
			{
				return std::numeric_limits<size_type>::max() / sizeof(Type);
			}
			
			template <typename OtherType, typename... Args>
			void construct(OtherType *pointer, Args&&... args)
			{
				::new ((void *) pointer) OtherType (std::forward<Args>(args)...);
			}
			
			template <typename OtherType>
			void destroy(OtherType *pointer)
			{
				pointer->~OtherType();
			}
		};
		
		/**
		 * \struct DebugAllocator
		**/
		template <typename Type>
		struct DebugAllocator
		{
			/* Defining to match with the Standard template library. */
			typedef Type value_type;
			typedef Type* pointer;
			typedef const Type* const_pointer;
			typedef Type& reference;
			typedef const Type& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
			
			template <typename OtherType>
			struct rebind
			{
				typedef DebugAllocator<OtherType> other;
			};
			
			/* Function declarations. */
			inline explicit DebugAllocator()
			{
				_memoryManager = DebugMemoryManager::instance();
			}
			
			inline explicit DebugAllocator(const DebugAllocator &other)
			{
				_memoryManager = other._memoryManager;
			}
			
			template <typename OtherType>
			inline explicit DebugAllocator(const DebugAllocator<OtherType> &other)
			{
				_memoryManager = other._memoryManager;
			}
			
			inline ~DebugAllocator()
			{
			}
			
			inline pointer address(reference object) noexcept
			{
				return &object;
			}
			
			inline const_pointer address(const_reference object) noexcept
			{
				return &object;
			}
			
			inline pointer allocate(size_type n)
			{
				// TODO: Finish the implementation of this function.
				return reinterpret_cast<pointer>(_memoryManager->allocate(n, __LINE__, __FILE__, false));
			}
			
			inline void deallocate(pointer p, size_type)
			{
				// TODO: Finish the implementation of this function.
				_memoryManager->free(p, false);
			}
			
			template <typename OtherType, typename... Args>
			void construct(OtherType *pointer, Args&&... args)
			{
				::new ((void *) pointer) OtherType (std::forward<Args>(args)...);
			}
			
			template <typename OtherType>
			void destroy(OtherType *pointer)
			{
				pointer->~OtherType();
			}
			
			private:
				/* TODO: Define the debug memory amanger class. */
				DebugMemoryManager *_memoryManager;
		};
		
		/**
		 * \struct SmallObjectAllocator
		**/
		template <typename Type>
		struct SmallObjectAllocator
		{
			/* Defining to match with the Standard template library. */
			typedef Type value_type;
			typedef Type* pointer;
			typedef const Type* const_pointer;
			typedef Type& reference;
			typedef const Type& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
			
			template <typename OtherType>
			struct rebind
			{
				typedef SmallObjectAllocator<OtherType> other;
			};
			
			/* Function declarations. */
			inline explicit SmallObjectAllocator()
			{
			}
			
			inline explicit SmallObjectAllocator(const SmallObjectAllocator &other)
			{
			}
			
			template <typename OtherType>
			inline explicit SmallObjectAllocator(const SmallObjectAllocator<OtherType> &other)
			{
			}
			
			inline ~SmallObjectAllocator()
			{
			}
			
			inline pointer address(reference object) noexcept
			{
				return &object;
			}
			
			inline const_pointer address(const_reference object) noexcept
			{
				return &object;
			}
			
			inline pointer allocate(size_type n)
			{
				// TODO: Finish the implementation of this function.
			}
			
			inline void deallocate(pointer p, size_type)
			{
				// TODO: Finish the implementation of this function.
			}
			
			template <typename OtherType, typename... Args>
			void construct(OtherType *pointer, Args&&... args)
			{
				::new ((void *) pointer) OtherType (std::forward<Args>(args)...);
			}
			
			template <typename OtherType>
			void destroy(OtherType *pointer)
			{
				pointer->~OtherType();
			}
			
			private:
				/* TODO: define the small object manager class. */
		};
	}
}

WITCH_END_HEADER

#endif // __WITCHENGINE_CORE_CUSTOMMEMORYALLOCATOR_HPP__