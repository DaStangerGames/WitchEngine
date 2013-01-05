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

#include <WitchCore/String.hpp>
#include <WitchCore/File.hpp>
#include <WitchCore/LogManager.hpp>
#
#include <WitchCore/Log.hpp>

namespace WitchEngine
{
	namespace Core
	{
		Log::Log(const String &name) : 
			_loggingLevel(Information)
		{
			
		}
		
		Log::~Log()
		{
			// Closing the file.
			_file.close();
		}
		
		void Log::setName(const String &name)
		{
			// We should temporaly close the file to set it to the new file name.
			
			// We should also delete the entry in the LogManager class and
			// add a new one which will refers to this instance.
			_logName = name;
		}
		
		String Log::name() const
		{
			return _logName;
		}
		
		void Log::setLoggingLevel(LoggingLevel ll)
		{
			_loggingLevel = ll;
		}
		
		LoggingLevel Log::loggingLevel() const
		{
			return _loggingLevel;
		}
		
		void Log::writeMessage(const String &message)
		{
			String prefix;
			
			switch(_loggingLevel)
			{
				case Information:
					prefix = "[Information]";
					break;
				
#if WITCHENGINE_DEBUG_MODE == 0x1				
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
			
			// Formatting a string and wrinting it into the log file.
		}
		
		void Log::writeMessage(const String &message, LoggingLevel ll)
		{
			String prefix;
			
			switch(ll)
			{
				case Information:
					prefix = "[Information]";
					break;
				
#if WITCHENGINE_DEBUG_MODE == 0x1				
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
			
			// Formatting a string and wrinting it into the log file.
		}
	}
}