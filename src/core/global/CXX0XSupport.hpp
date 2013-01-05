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

#ifndef __WITCHENGINE_CORE_CXX0XSUPPORT_HPP__
#define __WITCHENGINE_CORE_CXX0XSUPPORT_HPP__

// Creating fake alises on missing keywords.
#define override
#define final
#define nullptr				NULL		// Alias to the C-style NULL value.
#define char16_t			wchar_t		// Alias to the old wide-character type.
#define char32_t			wchar_t		// Alias to the old wide-character type.
#define alignas(expression)	

#include <boost/static_assert.hpp>

#define static_assert(Condition, Message)	BOOST_STATIC_ASSERT_MSG(Condition, Message)

#endif // __WITCHENGINE_CORE_CXX0XSUPPORT_HPP__