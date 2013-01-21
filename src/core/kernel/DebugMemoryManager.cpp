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

#include "DebugMemoryManager.hpp"

namespace WitchEngine
{
	namespace Core
	{
		DebugMemoryManager::DebugMemoryManager() :
			_blocks()
		{
		}
		
		DebugMemoryManager::~DebugMemoryManager()
		{
			if(!_blocks.empty())
			{
				reportLeaks();
			}
			else
			{
			}
		}
		
		void* DebugMemoryManager::allocate(std::size_t size, uint32 line, const char *file, bool array)
		{
			void *pointer = ::std::malloc(size);
			
			MemoryBlock NewBlock(size, line, file, array);
			
			_blocks[pointer] = NewBlock;
			
			return pointer;
		}
		
		void DebugMemoryManager::free(void *pointer, bool array)
		{
			MemoryBlockMap::iterator it = _blocks.find(pointer);
			
			if(it == _blocks.end())
			{
				::std::free(pointer);
				return;
			}
			
			if(it->second.Array != array)
			{
				// TODO: Throwing an excpetion.
			}
			
			_blocks.erase(it);
			
			::std::free(pointer);
		}
		
		void DebugMemoryManager::reportLeaks()
		{
			// Leaks details.
			std::size_t totalSize = 0;
			for(MemoryBlockMap::iterator it = _blocks.begin();
				it != _blocks.end();
				it++)
			{
				totalSize += it->second.Size;
				
				::std::free(it->first);
			}
		}
	}
}