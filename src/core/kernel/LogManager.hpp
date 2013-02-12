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

#ifndef __WITCHENGINE_CORE_LOGMANAGER_HPP__
#define __WITCHENGINE_CORE_LOGMANAGER_HPP__

#include <WitchCore/WitchGlobal.hpp>
#
#include "String.hpp"
#include "Log.hpp"

namespace WitchEngine
{
	namespace Core
	{
		class WITCHENGINE_CORE_EXPORT LogManager : public Singleton<LogManager>
		{
			friend class Singleton<LogManager>;
			
			private:
				LogManager();
				~LogManager();
				
			public:
				Log* create(const String &logName);
				Log* log(const String &logName);
				void destroy(const String &logName);
				
				Log* defaultLog();
				void setDefaultLog(Log *newDefaultLog);
				
			private:
				std::map<std::string, Log *> _loggers;
		};
	}
}

#endif // __WITCHENGINE_CORE_LOGMANAGER_HPP__