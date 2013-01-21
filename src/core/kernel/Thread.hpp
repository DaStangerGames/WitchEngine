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

#ifndef __WITCHENGINE_CORE_THREAD_HPP__
#define __WITCHENGINE_CORE_THREAD_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#include "Functor.hpp"

WITCH_BEGIN_HEADER

WITCH_MODULE(Core)

namespace WitchEngine
{
	namespace Core
	{
		// Forward declaration.
		class ThreadImpl;
		
		/**
		 * \class Thread
		 * \author Alexandre Valentin Jamet
		 * \date 13 Junuary 2013
		 * \brief Thread class is a home made implementation for multi-threading.
		 * 
		**/
		class WITCHENGINE_CORE_EXPORT Thread
		{
			public:
				Thread() = default;
				template <typename FunctionType> Thread(FunctionType function)
				{
					CreateImpl(new FunctorWithoutArgs<FunctionType>(function));
				}
				
				template <typename FunctionType, typename... Args> Thread(FunctionType function, Args... args)
				{
					CreateImpl(new FunctorWithArgs<FunctionType, Args...>(function, args...));
				}
				
				template <typename Class> Thread(void (Class::*function)(), Class *object)
				{
					CreateImpl(new MemberWithoutArgs<Class>(function, object));
				}
				
				template <typename Class, typename... Args> Thread(void (Class::*function)(), Class *object, Args... args)
				{
					CreateImpl(new MemberWithArgs<Class, Args...>(function, object, args...));
				}
				
				~Thread();
				
				Thread(const Thread &) = delete;
				Thread(Thread &&) = delete;
				void operator= (const Thread &) = delete;
				void operator= (Thread &&) = delete;
				
				void detach();
				bool isJoinable() const;
				void join();
								
			private:
				void createImpl(Functor *functor);
				
				ThreadImpl *_impl = nullptr;
		};
	}
}

WITCH_END_HEADER

#endif // __WITCHENGINE_CORE_THREAD_HPP__