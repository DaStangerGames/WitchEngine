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

#include "WitchGlobal.hpp"

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM_WIN64
#	include <Windows.h>
#	include <WindowsX.h>
#endif

#include <cstdlib>

/*!
	\mainpage WitchEngine 1.0
	
	\section intro_section Introduction
	
	WitchEngine is a complete game engine and game development tools suites.
	The main goal of the project is to reach a professional level of solutions
	and also be an open project (understand a project in which anyone can participate).
	
	This project was launched for needs of <b>The Team Entertainment</b>'s project
	and developped by them. <b>The Team Entertainment</b> is a game studio created in
	2012 by friends.
	
	\section license_section Terms of usage
	
	WitchEngine is delivered under the terms of differents license in order of the way you acquired it.
	
	WitchEngine is delivered under these licenses:
	\li The GNU GPL v3 license ;
	\li The GNU LGPL v3 license ;
	\li The Team Entertainment commercial license.
	
	Which license allows you to do differents things with this product, I'll give you an explaination
	for each license:
	
		\subsection gnu_gpl_license The GNU GPL License v3
		
		This license allow you to:
		\li freely execute the software ;
		\li freely redistribute copies.
		
		But you don't have the right to redistribute copies as commercial product or use it in a commercial product.
		
		All rights granted under this License are granted for the term of
		copyright on the Program, and are irrevocable provided the stated
		conditions are met.  This License explicitly affirms your unlimited
		permission to run the unmodified Program.  The output from running a
		covered work is covered by this License only if the output, given its
		content, constitutes a covered work.  This License acknowledges your
		rights of fair use or other equivalent, as provided by copyright law.

		  You may make, run and propagate covered works that you do not
		convey, without conditions so long as your license otherwise remains
		in force.  You may convey covered works to others for the sole purpose
		of having them make modifications exclusively for you, or provide you
		with facilities for running those works, provided that you comply with
		the terms of this License in conveying all material for which you do
		not control copyright.  Those thus making or running the covered works
		for you must do so exclusively on your behalf, under your direction
		and control, on terms that prohibit them from making any copies of
		your copyrighted material outside their relationship with you.
		
		\subsection gnu_lgpl_license The GNU LGPL License v3
		
		\subsection commercial_license The Team Entertainment Commercial License
		
	\section modules_section Modules
	
		<b>WitchEngine</b> is designed as a framework (an association of multiple library).
*/

namespace WitchEngine
{
	namespace Core
	{
		const char* WitchVersion()
		{
			return WITCH_VERSION_STR;
		}
		
		bool WitchSharedBuild()
		{
#ifdef WITCHENGINE_SHARED
			return true;
#else
			return false;
#endif
		}
		
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
		WitchSystemInfo::WinVersion WitchSystemInfo::windowsVersion()
		{
#ifndef VER_PLATFORM_WIN32s
#define VER_PLATFORM_WIN32s            0
#endif
#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS  1
#endif
#ifndef VER_PLATFORM_WIN32_NT
#define VER_PLATFORM_WIN32_NT            2
#endif
#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE            3
#endif

			static WitchSystemInfo::WinVersion winver;
			
			if(winver)
				return winver;
			winver = WitchSystemInfo::WV_NT;
			
			OSVERSIONINFO osver;
			osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			
			GetVersionEx(&osver);
			
			if(osver.dwMajorVersion < 5)
			{
				winver = WitchSystemInfo::WV_NT;
			}
			else if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
			{
				winver = WitchSystemInfo::WV_2000;
			}
			else if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
			{
				winver = WitchSystemInfo::WV_XP;
			}
			else if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
			{
				winver = WitchSystemInfo::WV_2003;
			}
			else if(osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
			{
				winver = WitchSystemInfo::WV_VISTA;
			}
			else if(osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
			{
				winver = WitchSystemInfo::WV_WINDOWS7;
			}
			else if(osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
			{
				winver = WitchSystemInfo::WV_WINDOWS8;
			}
			else
			{
				winver = WV_NT_based;
			}
			
			return winver;
		}
		
		const WitchSystemInfo::WinVersion WitchSystemInfo::WindowsVersion = WitchSystemInfo::windowsVersion();
#endif // WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_MACOS
		WitchSystemInfo::MacVersion WitchSystemInfo::macintoshVersion()
		{
		}
		
		const WitchSystemInfo::MacVersion WitchSystemInfo::MacintoshVersion = WitchSystemInfo::macintoshVersion();
#endif // WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_MACOS

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_BLACKBERRY
#endif // WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_BLACKBERRY

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_ANDROID
#endif // WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_ANDROID

		WitchSystemInfo::ProcessorArchitecture WitchSystemInfo::architecture()
		{
			static WitchSystemInfo::ProcessorArchitecture arch;
			
			if(arch)
				return arch;
				
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
			SYSTEM_INFO sysinfo;
			
			GetNativeSystemInfo(&sysinfo);

			if(sysinfo.wProcessorArchitecture == 9)
			{
				arch = WitchSystemInfo::PA_AMD64;
			}
			else if(sysinfo.wProcessorArchitecture == 6)
			{
				arch = WitchSystemInfo::PA_IA64;
			}
			else if(sysinfo.wProcessorArchitecture == 0)
			{
				arch = WitchSystemInfo::PA_INTEL;
			}
			else
			{
				arch = WitchSystemInfo::PA_UNKNOWN;
			}
#endif

			return arch;
		}
		
		const WitchSystemInfo::ProcessorArchitecture WitchSystemInfo::Architecture = WitchSystemInfo::architecture();
		
		char* getenv(const char *varName)
		{
#if defined(_MSC_VER) && _MSC_VER >= 1400
#else
			return ::getenv(varName);
#endif
		}
		
		bool putenv(const char *varName, const char *value)
		{
#if defined(_MSC_VER) && _MSC_VER >= 1400
			return _putenv_s(varName, value) == 0;
#else

#endif
		}
	}
}
