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

#include "LogManager.hpp"
#include "LogManager.hpp"

namespace WitchEngine
{
	namespace Core
	{
		LogManager::LogManager()
		{
			_loggers["Default"] = new Log("Engine");
		}
		
		LogManager::~LogManager()
		{
			for(std::map<std::string, Log *>::iterator it = _loggers.begin();
				it != _loggers.end();
				++it)
			{
				delete it->second;
			}
		}
		
		Log* LogManager::create(const String &logName)
		{
			Log *newLogger = new Log(logName);
			
			_loggers[logName] = newLogger;
		}
		
		Log* LogManager::log(const String &logName)
		{
			return _loggers[logName];
		}
		
		void LogManager::destroy(const String &logName)
		{
			std::map<std::string, Log *>::iterator it = _loggers.find(logName);
			
			Log *log = it->second;
			
			_loggers.erase(it);
			
			delete log;
		}
		
		Log* LogManager::defaultLog()
		{
			return _loggers["Default"];
		}
		
		void LogManager::setDefaultLog(Log *newDefaultLog)
		{
			_loggers["Default"] = newDefaultLog;
		}
	}
}