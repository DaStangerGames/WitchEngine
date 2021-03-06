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

#include "Time.hpp"

namespace WitchEngine
{
	namespace Core
	{
		std::time_t FileTimeToTime(FILETIME *time)
		{
			SYSTEMTIME stUTC, stLocal;
			SystemTimeToTzSpecificLocalTime(nullptr, &stUTC, &stLocal);
			
			std::tm timeinfo;
			timeinfo.tm_sec = stLocal.wSecond;
			timeinfo.tm_min = stLocal.wMinute;
			timeinfo.tm_hour = stLocal.wHour;
			timeinfo.tm_mday = stLocal.wDay;
			timeinfo.tm_mon = stLocal.wMonth - 1;
			timeinfo.tm_year = stLocal.wYear - 1900;
			timeinfo.tm_isdst = -1;
			
			return std::mktime(&timeinfo);
		}
	}
}