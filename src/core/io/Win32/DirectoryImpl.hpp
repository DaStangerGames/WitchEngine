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

#ifndef __WITCHENGINE_CORE_DIRECTORY_IMPL_HPP__
#define __WITCHENGINE_CORE_DIRECTORY_IMPL_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#include "../../kernel/String.hpp"
#
#include <Windows.h>

namespace WitchEngine
{
	namespace Core
	{
		class DirectoryImpl
		{
			public:
				void close();
				
				String resultName() const;
				uint64 resultSize() const;
				
				bool isResultDirectory() const;
				
				bool nextResult();
				
				bool open(const String &dirPath);
				
				static bool create(const String &dirPath);
				static bool exists(const String &dirPath);
				static String current();
				static bool remove(const String &dirPath);
				
			private:
				HANDLE _handle;
				WIN32_FIND_DATAW _result;
				bool _firstCall;
		};
	}
}

#endif // __WITCHENGINE_CORE_DIRECTORY_HPP__