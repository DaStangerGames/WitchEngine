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

#include "Log.hpp"

namespace WitchEngine
{
	namespace Core
	{
		Log::Log() :
			_file(),
			_defaultll(Information),
			_name()
		{
		}
		
		Log::Log(const String &name) :
			_file("Engine/" + name + ".log"),
			_defaultll(Information),
			_name(name)
		{
			_file.open(File::WriteOnly);
		}
		
		Log::~Log()
		{
			_file.close();
		}
		
		void Log::setDefaultLoggingLevel(LoggingLevel ll)
		{
			_defaultll = ll;
		}
		
		Log::LoggingLevel Log::defaultLoggingLevel() const
		{
			return _defaultll;
		}
		
		void Log::writeMessage(const String &message, LoggingLevel ll)
		{
			String prefix;
			
			switch(ll)
			{
				case Information:
					prefix = "[Information]";
					break;
					
#ifdef WITCHENGINE_DEBUG
				case Debug:
					prefix = "[Debug]";
					break;
#endif

				case Warning:
					prefix = "[Warning]";
					break;
					
				case Error:
					prefix = "[Error]";
					break;
			}
		}
		
		void Log::writeMessage(const String &message)
		{
			writeMessage(message, _defaultll);
		}
		
		void Log::setName(const String &name)
		{
			if(_file.isOpen())
				_file.close();
				
			_file.setPath(String("Engine/").append(name).append(".log"));
			
			if(_file.open(File::WriteOnly))
				_name = name;
		}
		
		String Log::name() const
		{
			return _name;
		}
	}
}