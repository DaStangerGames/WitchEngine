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

#ifndef __WITCHENGINE_CORE_TRACKERMEMORYALLOCATOR_HPP__
#define __WITCHENGINE_CORE_TRACKERMEMORYALLOCATOR_HPP__

#include <WitchCore/WitchGlobal.hpp>

namespace WitchEngine
{
	namespace Core
	{
		class WITCHENGINE_CORE_EXPORT TrackerMemoryAllocator : public Singleton<TrackerMemoryAllocator>
		{
			friend class Singleton<TrackerMemoryAllocator>;
			
			private:
			
				// Internal structure to handle the memory blocks. Doesn't need to be exported.
				struct MemoryBlock
				{
					std::size_t		Size;		/*!< The size of the memory block. */
					uint32			Line;		/*!< The line where the block were allocated. */
					const char *	File;		/*!< The file where the block were allocated. */
					const char *	Function;	/*!< The function where the block were allocated. */
					bool			Array;		/*!< Is the memory block an array ? */
					
					MemoryBlock() :
						Size(0),
						Line(0),
						File(nullptr),
						Function(nullptr),
						Array(false)
					{
					}
					
					MemoryBlock(std::size_t size, uint32 line = 0, const char *file = nullptr, const char *function = nullptr, bool array = false) :
						Size(size),
						Line(line),
						File(file),
						Function(function),
						Array(array)
					{
					}
				};
				
				typedef Map<void *, MemoryBlock> MemoryBlockMap;
			
				TrackerMemoryAllocator();
				~TrackerMemoryAllocator();
				
			public:
				void* allocate(std::size_t size);
				void* allocate(std::size_t size, uint32 line, const char *file, const char *function, bool array);
				void free(void *ptr);
				void free(void *ptr, bool array);
				
				void reportLeaks();
				
				uint32 currentlyUsedMemory() const;
				uint32 maxAllocatedMemory() const;
				
			private:
				uint32 _currentlyUsedMemory;
				uint32 _maxAllocatedMemory;
				MemoryBlockMap _blocks;
		};
	}
}

#endif // __WITCHENGINE_CORE_TRACKERMEMORYALLOCATOR_HPP__