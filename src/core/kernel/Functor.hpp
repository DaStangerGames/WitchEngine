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

#ifndef __WITCHENGINE_CORE_FUNCTOR_HPP__
#define __WITCHENGINE_CORE_FUNCTOR_HPP__

namespace WitchEngine
{
	namespace Core
	{
		struct Functor
		{
			virtual ~Functor() { }
			
			virtual void run() = 0;
		};
		
		template <typename FunctionType>
		struct FunctorWithoutArgs : public Functor
		{
			FunctorWithoutArgs(FunctionType func) :
				_func(func)
			{
			}
			
			void run()
			{
				_func();
			}
			
			private:
				FunctionType _func;
		};
		
		template <typename FunctionType, typename... Args>
		struct FunctorWithArgs : public Functor
		{
			FunctorWithArgs(FunctionType func, Args&... args) :
				_func(func),
				_args(args...)
			{
			}
			
			void run()
			{
				// TODO : complete this part of the code with a method to unpack tuple containers.
			}
			
			private:
				FunctionType _func;
				Tuple<Args...> _args;
		};
		
		template <typename Class>
		struct MemberWithoutArgs : public Functor
		{
			MemberWithoutArgs(void (Class::*func)(), Class *object) :
				_func(func),
				_object(object)
			{
			}
			
			void run()
			{
				(_object->*_func)();
			}
			
			private:
				void (Class::*_func)();
				Class *_object;
		};
		
		template <typename Class, typename... Args>
		struct MemberWithArgs : public Functor
		{
			MemberWithArgs(void (Class::*func)(Args...), Class *object, Args&... args) :
				_func(func),
				_object(object),
				_args(args...)
			{
			}
			
			void run()
			{
				// TODO : complete this part of the code with a method to unpack tuple containers.
			}
			
			private:
				void (Class::*_func)(Args...);
				Tuple<Args...> _args;
				Class *_object;
		};
	}
}

#endif // __WITCHENGINE_CORE_FUNCTOR_HPP__