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

#ifndef __WITCHENGINE_CORE_LOG_HPP__
#define __WITCHENGINE_CORE_LOG_HPP__

#include <WitchCore/WitchGlobal.hpp>
#include "../kernel/String.hpp"
#include "../io/File.hpp"

namespace WitchEngine
{
	namespace Core
	{
		class WITCHENGINE_CORE_EXPORT Log
		{
			public:
				
				enum LoggingLevel
				{
					Information,
					
#ifdef WITCHENGINE_DEBUG
					Debug,
#endif
					
					Warning,
					
					Error
				};
				
				Log();
				Log(const String &name);
				~Log();
				
				void setDefaultLoggingLevel(LoggingLevel ll);
				LoggingLevel defaultLoggingLevel() const;
				
				void writeMessage(const String &message, LoggingLevel ll);
				void writeMessage(const String &message);
				
				void setName(const String &name);
				String name() const;
				
			private:
				File _file;
				LoggingLevel _defaultll;
				String _name;
		};
	}
}

#endif // __WITCHENGINE_CORE_LOG_HPP__