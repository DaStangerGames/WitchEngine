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

#include "ThreadImpl.hpp"
#
#include "../Functor.hpp"
#
#include <process.h>

namespace WitchEngine
{
	namespace Core
	{
		ThreadImpl::ThreadImpl(Functor *functor)
		{
			_handle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, &ThreadImpl::threadProc, functor, 0, nullptr));
			if(!_handle)
			{
				// TODO: Throwing an exception.
			}
		}
		
		void ThreadImpl::detach()
		{
			CloseHandle(_handle);
		}
		
		void ThreadImpl::join()
		{
			WaitForSingleObject(_handle, INFINITE);
			CloseHandle(_handle);
		}
		
		unsigned int __stdcall ThreadImpl::threadProc(void *userData)
		{
			Functor *func = static_cast<Functor *>(userData);
			func->run();
			delete func;
			
			return 0;
		}
		
		void ThreadImpl::sleep(uint32 time)
		{
			::Sleep(time);
		}
	}
}