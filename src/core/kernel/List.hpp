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

#ifndef __WITCHENGINE_CORE_LIST_HPP__
#define __WITCHENGINE_CORE_LIST_HPP__

#include "CustomAllocator.hpp"

namespace WitchEngine
{
	namespace Core
	{
		template <typename Type, typename Alloc = Allocator<Type> > 
		class List
		{
			public:
				List();
		};
	}
}

#endif // __WITCHENGINE_CORE_LIST_HPP__