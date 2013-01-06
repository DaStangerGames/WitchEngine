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

#include <WitchCore/Exception.hpp>

namespace WitchEngine
{
	namespace Core
	{
		// Declaration of the base exception class : IWitchException
		IWitchException::IWitchException(const String &message, uint32 line, const String &file, const String &function) throw() :
			_line(line),
			_file(file),
			_function(function)
		{
		}
		
		IWitchException::~IWitchException() throw()
		{
		}
		
		const char* IWitchException::what() const throw()
		{
		}
		
		uint32 IWitchException::line() const
		{
			return _line;
		}
		
		String IWitchException::file() const
		{
			return _file;
		}
		
		String IWitchException::function() const
		{
			return _function;
		}
		
		String IWitchException::description() const
		{
		}
		
		// Declaration of the specialized bad allocation exception class : BadAllocationException
		BadAllocationException::BadAllocationException(const void *pointer, bool array, uint32 line, const String &file, const String &function) throw() :
			IWitchException("", line, file, function)
		{
		}
		
		BadAllocationException::~BadAllocationException() throw()
		{
		}
		
		// Declaration of the specialized loading failed exception class : LoadingFailedException
		LoadingFailedException::LoadingFailedException(const String &message, uint32 line, const String &file, const String &function) throw() :
			IWitchException(message, line, file, function)
		{
		}
		
		LoadingFailedException::~LoadingFailedException() throw()
		{
		}
	}
}