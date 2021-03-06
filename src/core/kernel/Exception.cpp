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

#include "Exception.hpp"

namespace WitchEngine
{
	namespace Core
	{
		WitchException::WitchException(const String &message, const String &file, const String &function, uint32 line) throw() :
			_message(message),
			_file(file),
			_function(function),
			_line(line)
		{
		}
		
		WitchException::~WitchException() throw()
		{
		}
		
		const char* WitchException::what() const throw()
		{
		}
		
		String WitchException::description() const
		{
		}
			
		String WitchException::file() const
		{
			return _file;
		}
		
		String WitchException::function() const
		{
			return _function;
		}
		
		uint32 WitchException::line() const
		{
			return _line;
		}
	}
}