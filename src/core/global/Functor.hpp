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

#ifndef __WITCHENGINE_CORE_FUNCTOR_HPP__
#define __WITCHENGINE_CORE_FUNCTOR_HPP__

#include <tuple>

namespace WitchEngine
{
	namespace Core
	{
		struct Functor
		{
			virtual ~Functor() { }
			
			virtual void execute() = 0;
		};
		
		template <typename FunctionType>
		struct FunctorWithoutArguments : public Functor
		{
			FunctorWithoutArguments(FunctionType func);
			
			void execute();
			
			private:
				FunctionType _func;
		};
		
		template <typename FunctionType, typename... Args>
		struct FunctionWithArguments : public Functor
		{
			FunctorWithArguments(FunctionType func, Args&... args);
			
			void execute();
			
			private:
				FunctionType _func;
				std::tuple<Args...> _args;
		};
	}
}

#endif // __WITCHENGINE_CORE_FUNCTOR_HPP__