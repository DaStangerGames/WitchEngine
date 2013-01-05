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
#include <WitchCore/Log.hpp>
#
#include <WitchCore/LogManager.hpp>

namespace WitchEngine
{
	namespace Core
	{
		LogManager::LogManager()
		{
			_container["Default"] = new Log("");
		}
		
		LogManager::~LogManager()
		{
			for(Map<std::string, Log *>::iterator it = _container.begin();
				it != _container.end();
				++it)
			{
				delete it->second;
			}
		}
		
		Log* LogManager::createLog(const String &name)
		{
			Log *newLog = new Log(name);
		}
	}
}