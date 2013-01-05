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

#ifndef __WITCHENGINE_CORE_LOG_HPP__
#define __WITCHENGINE_CORE_LOG_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#include <WitchCore/WitchEnums.hpp>

namespace WitchEngine
{
	namespace Core
	{
		// Forward declaration of needed classes.
		class File;
		class String;
		
		/**
		 * \class Log
		 * \author Alexandre Valentin Jamet
		 * \date 01 Junuary 2013
		 * \brief 
		**/
		class WITCHENGINE_CORE_EXPORT Log
		{
			public:
				Log(const String &name);
				~Log();
				
				void setName(const String &name);
				String name() const;
				
				void setLoggingLevel(LoggingLevel ll);
				LoggingLevel loggingLevel() const;
				
				void writeMessage(const String &message);
				void writeMessage(const String &message, LoggingLevel ll);
				
			private:
				File _file;
				String _logName;
				LoggingLevel _loggingLevel;
		};
	}
}

#endif // __WITCHENGINE_CORE_LOG_HPP__