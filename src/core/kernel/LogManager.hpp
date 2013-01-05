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

#ifndef __WITCHENGINE_CORE_LOGMANAGER_HPP__
#define __WITCHENGINE_CORE_LOGMANAGER_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#include <WitchCore/WitchEnums.hpp>

namespace WitchEngine
{
	namespace Core
	{
		// Forward declarations of needed classes.
		class String;
		class Log;
		
		class WITCHENGINE_CORE_EXPORT LogManager : public Singleton<LogManager>
		{
			friend class Singleton<LogManager>;
			
			private:
				LogManager();
				~LogManager();
				
			public:
				Log* createLog(const String &name);
				Log* log(const String &name);
				Log* getDefaultLog();
				void destroyLog(const String &name);
				void destroyLog(Log *log);
				Log* setDefaultLog(Log *newDefaultLog);
				void logMessage(const String &message, LoggingLevel ll);
			
			private:
				Map<std::string, Log *> _container;
		};
	}
}

#endif // __WITCHENGINE_CORE_LOGMANAGER_HPP__