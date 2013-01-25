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

#include "PluginImpl.hpp"

namespace WitchEngine
{
	namespace Core
	{
		ModuleFunc PluginImpl::getSymbol(const String &symbolName) const
		{
			ModuleFunc symbol = reinterpret_cast<ModuleFunc>(GetProcAddress(_library, symbolName.constBuffer()));
			
			if(!symbol)
				// TODO: Throw an excpetion.
				
			return symbol;
		}
		
		bool PluginImpl::load(const String &filePath)
		{
			String path = filePath;
			
			wchar_t *pathW = path.wideBuffer();
			_library = LoadLibraryExW(pathW, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
			delete[] pathW;
			
			if(_library)
				return true;
			else
			{
				// TODO: Throw an exception.
				return false;
			}
		}
		
		bool PluginImpl::unload()
		{
			FreeLibrary(_library);
		}
	}
}