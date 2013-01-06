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

#ifndef __WITCHENGINE_CORE_EXCEPTION_HPP__
#define __WITCHENGINE_CORE_EXCEPTION_HPP__

#include <WitchCore/WitchGlobal.hpp>
#include <WitchCore/String.hpp>
#
#include <exception>

namespace WitchEngine
{
	namespace Core
	{		
		/**
		 * \class IWitchException
		 * \author Alexandre Valentin Jamet
		 * \date 24 December 2012
		 * \brief This is the base class of all the exception that could be thrown by the engine.
		**/
		class WITCHENGINE_CORE_EXPORT IWitchException : public std::exception
		{
			public:
				IWitchException(const String &message, uint32 line, const String &file, const String &function) throw();
				virtual ~IWitchException() throw() = 0;	// Declaring a pure virtual to make the class abstract.
				
				virtual const char* what() const throw();
				
				uint32 line() const;
				String file() const;
				String function() const;
				String description() const;
				
			private:
				uint32 _line;		/*!< The line where the exception were thrown. */
				String _file;		/*!< The file where the exception were thrown. */
				String _function;	/*!< The function where the exception were thrown. */
				String _message;	/*!< The message that we want to give as description. */
		};
		
		class WITCHENGINE_CORE_EXPORT BadAllocationException : public IWitchException
		{
			public:
				BadAllocationException(const void *pointer, bool array, uint32 line, const String &file, const String &function) throw();
				virtual ~BadAllocationException() throw();
		};
		
		class WITCHENGINE_CORE_EXPORT LoadingFailedException : public IWitchException
		{
			public:
				LoadingFailedException(const String &message, uint32 line, const String &file, const String &function) throw();
				virtual ~LoadingFailedException() throw();
		};
	}
}

#endif // __WITCHENGINE_CORE_EXCEPTION_HPP__