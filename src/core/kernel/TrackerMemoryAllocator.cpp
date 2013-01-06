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

#include <cstdlib>
#
#include <WitchCore/TrackerMemoryAllocator.hpp>
#include <WitchCore/Exception.hpp>
#include <WitchCore/File.hpp>

namespace WitchEngine
{
	namespace Core
	{
		TrackerMemoryAllocator::TrackerMemoryAllocator() :
			_currentlyUsedMemory(0),
			_maxAllocatedMemory(0),
			_blocks()
		{
			_file = new File("Engine/memory.log", ReadOnly);
		}
		
		TrackerMemoryAllocator::~TrackerMemoryAllocator()
		{
			if(!_blocks.empty())
			{
				reportLeaks();
			}
			
			_file->close();
			
			delete _file;
		}
		
		void* TrackerMemoryAllocator::allocate(std::size_t size)
		{
			return ::std::malloc(size);
		}
		
		void* TrackerMemoryAllocator::allocate(std::size_t size, uint32 line, const char *file, const char *function, bool array)
		{
			void *ptr = ::std::malloc(size);
			
			MemoryBlock NewBlock(size, line, file, function, array);
			
			_blocks[ptr] = NewBlock;
			
			return ptr;
		}
		
		void TrackerMemoryAllocator::free(void *ptr)
		{
			return ::std::free(ptr);
		}
		
		void TrackerMemoryAllocator::free(void *ptr, bool array)
		{
			MemoryBlockMap::iterator it = _blocks.find(ptr);
			
			if(it == _blocks.end())
			{
				::std::free(ptr);
				return;
			}
				
			if(it->second.Array != array)
			{
				throw BadAllocationException(ptr, !array, it->second.Line, it->second.File, it->second.Function);
			}
				
			_blocks.erase(it);
			
			::std::free(ptr);
		}
		
		void TrackerMemoryAllocator::reportLeaks()
		{
		}
		
		uint32 TrackerMemoryAllocator::currentlyUsedMemory() const
		{
			return _currentlyUsedMemory;
		}
		
		uint32 TrackerMemoryAllocator::maxAllocatedMemory() const
		{
			return _maxAllocatedMemory;
		}
	}
}