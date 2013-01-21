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

#ifndef __WITCHENGINE_CORE_WITCHGLOBAL_HPP__
#define __WITCHENGINE_CORE_WITCHGLOBAL_HPP__

// Defining versions constants.
#define WITCH_VERSION_STR	"1.0.0"		/*!< The version number as string. */

// WITCH_VERSION is (major << 16) + (minor << 8) + patch.
#define WITCH_VERSION		0x010000	/*!< Version formatted in hexadecimal. */

#define WITCH_VERSION_MAJOR	0x01		/*!< The major number of the version. */
#define WITCH_VERSION_MINOR	0x00		/*!< The minor number of the version. */
#define WITCH_VERSION_PATCH	0x00		/*!< The patch number of the version. */
#define WITCH_VERSION_NAME	""			/*!< The name of the current version. */

// Can be used like #if (WITCH_VERSION >= WITCH_VERSION_CHECK(1, 0, 0)).
#define WITCH_VERSION_CHECK(major, minor, patch)	((major<<16)|(minor<<8)|(patch))

// Creating aliases to handle the standard template library if necessary.
#ifndef WITCHENGINE_NO_STL
#	include <array>
#	include <tuple>
#	include <list>
#	include <vector>
#	include <map>
#	include <stack>
#	include <queue>
#	include <initializer_list>
#	include <set>
#	include <algorithm>

namespace WitchEngine
{
	namespace Core
	{
		// Decalaring type aliases to STL containers.
		template <typename Type, std::size_t Size> using Array = std::array<Type, Size>;
		template <typename... Types> using Tuple = std::tuple<Types...>;
		template <typename Type> using List = std::list<Type>;
		template <typename Type> using Vector = std::vector<Type>;
		template <typename Key, typename Type> using Map = std::map<Key, Type>;
		template <typename Type> using Stack = std::stack<Type>;
		template <typename Type> using Queue = std::queue<Type>;
		template <typename Type> using InitializerList = std::initializer_list<Type>;
		template <typename Type> using Set = std::set<Type>;
	}
}
#endif

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_MACOS
#	define WITCH_BEGIN_HEADER extern "C++" {
#	define WITCH_END_HEADER }
#else
#	define WITCH_BEGIN_HEADER
#	define WITCH_END_HEADER
#endif

// Defining constants to represents the operating system.
#define WITCHENGINE_PLATFORM_WIN32		0x0	/*!< Represents the Win32 operating systems. */
#define WITCHENGINE_PLATFORM_WIN64		0x1	/*!< Represents the Win64 operating systems. */
#define WITCHENGINE_PLATFORM_LINUX		0x2	/*!< Represents the Linux operating systems. */
#define WITCHENGINE_PLATFORM_MACOS		0x3 /*!< Represents the Mac OS X operating systems. */
#define WITCHENGINE_PLATFORM_BLACKBERRY	0x4	/*!< Represents the Blackberry OS. */
#define WITCHENGINE_PLATFORM_ANDROID	0x5	/*!< Represents the Android OS. */

// Defining constants to represents the operating system type.
#define WITCHENGINE_PLATFORM_KIND_POSIX	0x0	/*!< Represents system based on POSIX. */
#define WITCHENGINE_PLATFORM_KIND_DOS	0x1	/*!< Represents system based on MSDOS. */

// Defining constants to represents the compiler.
#define WITCHENGINE_COMPILER_GNUC	0x0 /*!< Represents the GNU GCC Compiler. */
#define WITCHENGINE_COMPILER_MSVC	0x1	/*!< Represents the Microsoft C++ Compiler. */

// Defining constants to represents the endianness.
#define WITCHENGINE_ENDIAN_BIG		0x0	/*!< Represents the big endian bytes order. */
#define WITCHENGINE_ENDIAN_LITTLE	0x1 /*!< Represents the little endian bytes order. */

// Defining constants to represents the target architecture.
#define WITCHENGINE_ARCH_I386		0x0	/*!< Represents the intel i386 architecture. */
#define WITCHENGINE_ARCH_X86_64		0x1 /*!< Represents the AMD x86-64 architecure. */
#define WITCHENGINE_ARCH_IA64		0x2 /*!< Represents the intel 64 architecure. */
#define WITCHENGINE_ARCH_ARM		0x3 /*!< Represents the ARM architecture. */
#define WITCHENGINE_ARCH_MIPS		0x4	/*!< Represents the MIPS architecture. */

// Defining constants to represents the windows system that you want to use.
#define WITCHENGINE_WS_WIN32		0x0	/*!< Represent the native Win32 API. */
#define WITCHENGINE_WS_X11			0x1	/*!< Represents the X11 library. */
#define WITCHENGINE_WS_COCOA		0x2	/*!< Represents the Cocoa framawork. */
#define WITCHENGINE_WS_QT			0x3	/*!< Represents the Qt framework. */

// Checking the compiler.
#if defined(_MSC_VER)
#	define WITCHENGINE_COMPILER	WITCHENGINE_COMPILER_MSVC
#	define WITCHENGINE_COMPILER_VERSION _MSC_VER

#    pragma warning(disable: 4251) /* class 'A' needs to have dll interface for to be used by clients of class 'B'. */
#    pragma warning(disable: 4244) /* 'conversion' conversion from 'type1' to 'type2', possible loss of data */
#    pragma warning(disable: 4275) /* non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier' */
#    pragma warning(disable: 4514) /* unreferenced inline/local function has been removed */
#    pragma warning(disable: 4800) /* 'type' : forcing value to bool 'true' or 'false' (performance warning) */
#    pragma warning(disable: 4097) /* typedef-name 'identifier1' used as synonym for class-name 'identifier2' */
#    pragma warning(disable: 4706) /* assignment within conditional expression */
#    pragma warning(disable: 4786) /* truncating debug info after 255 characters */
#    pragma warning(disable: 4660) /* template-class specialization 'identifier' is already instantiated */
#    pragma warning(disable: 4355) /* 'this' : used in base member initializer list */
#    pragma warning(disable: 4231) /* nonstandard extension used : 'extern' before template explicit instantiation */
#    pragma warning(disable: 4710) /* function not inlined */
#    pragma warning(disable: 4530) /* C++ exception handler used, but unwind semantics are not enabled. Specify -GX */

// Creating shortcut to attributes.
#	define FASTCALL __fastcall
#	define FUNCTION
#elif defined(__GNUC__)
#	define WITCHENGINE_COMPILER WITCHENGINE_COMPILER_GNUC
#	define WITCHENGINE_COMPILER_VERSION	__GNUC__

// Creating shortcut to attributes.
#	define FASTCALL __attribute__((fastcall))
#	define FUNCTION
#endif

// Checking the operating system.
#if defined(_WIN32) || defined(__WIN32__)
#	if defined(_WIN64)
#		define WITCHENGINE_PLATFORM WITCHENGINE_PLATFORM_WIN64
#	else
#		define WITCHENGINE_PLATFORM WITCHENGINE_PLATFORM_WIN32
#	endif
#	define WITCHENGINE_PLATFORM_KIND	WITCHENGINE_PLATFORM_KIND_DOS
	// Specials defines for Win32 API header.
	
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#	define NOMINMAX
#endif

// Checking the Windows version target.
#ifdef WITCHENGINE_WINDOWS_XP
#	define WITCHENGINE_WINDOWS_VERSION	0x0501
#elif defined(WITCHENGINE_WINDOWS_VISTA)
#	define WITCHENGINE_WINDOWS_VERSION	0x0600
#elif defined(WITCHENGINE_WINDOWS_7)
#	define WITCHENGINE_WINDOWS_VERSION	0x0601
#elif defined(WITCHENGINE_WINDOWS_8)
#	define WITCHENGINE_WINDOWS_VERSION	0x0608
#else
#	define WITCHENGINE_WINDOWS_VERSION	0x0600
#endif

#ifdef _WIN32_WINNT
#	if _WIN32_WINNT < WITCHENGINE_WINDOWS_VERSION
#		undef _WIN32_WINNT
#		define _WIN32_WINNT WITCHENGINE_WINDOWS_VERSION
#	endif
#else
#	define _WIN32_WINNT WITCHENGINE_WINDOWS_VERSION
#endif

// No DirectX10 / DirectX11 under Windows XP.
#if WITCHENGINE_WINDOWS_VERSION == 0x0501
#	define WITCHENGINE_NO_DIRECTX10_RENDERER
#	define WITCHENGINE_NO_DIRECTX11_RENDERER
#endif
	// Settings up features for this platform.
#	define WITCHENGINE_NO_OPENGLES_RENDERER
#elif defined(__linux__)
#	define WITCHENGINE_PLATFORM WITCHENGINE_PLATFORM_LINUX
#	define WITCHENGINE_PLATFORM_KIND	WITCHENGINE_PLATFORM_KIND_POSIX
	// Settings up features for this platform.
#	define WITCHENGINE_NO_DIRECTX9_RENDERER
#	define WITCHENGINE_NO_DIRECTX10_RENDERER
#	define WITCHENGINE_NO_DIRECTX11_RENDERER
#	define WITCHENGINE_NO_OPENGLES_RENDERER
#elif defined(__APPLE_CC__)
#	define WITCHENGINE_PLATFORM WITCHENGINE_PLATFORM_MACOS
#	define WITCHENGINE_PLATFORM_KIND	WITCHENGINE_PLATFORM_KIND_POSIX
	// Settings up features for this platform.
#	define WITCHENGINE_NO_DIRECTX9_RENDERER
#	define WITCHENGINE_NO_DIRECTX10_RENDERER
#	define WITCHENGINE_NO_DIRECTX11_RENDERER
#	define WITCHENGINE_NO_OPENGLES_RENDERER
#elif defined(__QNX__)
#	define WITCHENGINE_PLATFORM	WITCHENGINE_PLATFORM_BLACKBERRY
#	define WITCHENGINE_PLATFORM_KIND	WITCHENGINE_PLATFORM_KIND_POSIX
	// Settings up features for this platform.
#	define WITCHENGINE_NO_DIRECTX9_RENDERER
#	define WITCHENGINE_NO_DIRECTX10_RENDERER
#	define WITCHENGINE_NO_DIRECTX11_RENDERER
#	define WITCHENGINE_NO_OPENGL_RENDERER
#	define WITCHENGINE_NO_ON_THE_FLY_RENDERER_LOADING
#	define WITCHENGINE_NO_CONTROL_DEVICES	/* Dissabling external control devices under mobile platforms because there's no way to connect them. */
#elif defined(__ANDROID__)
#	define WITCHENGINE_PLATFORM WITCHENGINE_PLATFORM_ANDROID
#	define WITCHENGINE_PLATFORM_KIND	WITCHENGINE_PLATFORM_KIND_POSIX
	// Settings up features for this platform.
#	define WITCHENGINE_NO_DIRECTX9_RENDERER
#	define WITCHENGINE_NO_DIRECTX10_RENDERER
#	define WITCHENGINE_NO_DIRECTX11_RENDERER
#	define WITCHENGINE_NO_OPENGL_RENDERER
#	define WITCHENGINE_NO_ON_THE_FLY_RENDERER_LOADING
#	define WITCHENGINE_NO_CONTROL_DEVICES	/* Dissabling external control devices under mobile platforms because there's no way to connect them. */
#endif

// Checking the architecture.
#if defined(i386) || defined(__i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686) || defined(_M_I86) || defined(_M_IX86)
#	define WITCHENGINE_ARCH WITCHENGINE_ARCH_I386	
#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#	define WITCHENGINE_ARCH WITCHENGINE_ARCH_X86_64
#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(_M_IA64)
#	define WITCHENGINE_ARCH WITCHENGINE_ARCH_IA64
#elif defined(__arm__) || defined(__thumb__) || defined(_M_ARM) || defined(_M_ARMT)
#	define WITCHENGINE_ARCH WITCHENGINE_ARCH_ARM
#	define WITCHENGINE_NO_OPTIMIZED_MATHFUNCTIONS
#endif

// Checking the window system.
#if defined(WITCHENGINE_QT)
#	define WITCHENGINE_WS	WITCHENGINE_WS_QT
#elif WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM_WIN64
#	define WITCHENGINE_WS	WITCHENGINE_WS_WIN32
#elif WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_LINUX
#	define WITCHENGINE_WS	WITCHENGINE_WS_X11
#elif WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_MACOS
#	define WITCHENGINE_WS	WITCHENGINE_WS_COCOA
#endif

// Tools for features.
#include <WitchCore/WitchFeatures.hpp>

#define WITCHENGINE_SUPPORTS(Feature)	(!defined(WITCHENGINE_NO_##Feature))

// Creating symbols for exportations.
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_LINUX || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_MACOS
#	define WITCHENGINE_EXPORT	__attribute__((visibility("default")))
#	define WITCHENGINE_IMPORT	__attribute__((visibility("default")))
#	define WITCHENGINE_HIDDEN	__attribute__((visibility("hidden")))
#endif

#ifndef WITCHENGINE_EXPORT
#	if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#		define WITCHENGINE_EXPORT	__declspec(dllexport)
#	else
#		define WITCHENGINE_EXPORT
#	endif
#endif

#ifndef WITCHENGINE_IMPORT
#	if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#		define WITCHENGINE_IMPORT	__declspec(dllimport)
#	else
#		define WITCHENGINE_IMPORT
#	endif
#endif

#ifndef WITCHENGINE_HIDDEN
#	define WITCHENGINE_HIDDEN
#endif

// Creating WitchEngine shared library if WITCHENGINE_MAKEDLL is defined (Windows only).
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#	if defined(WITCHENGINE_NODLL)
#		undef WITCHENGINE_MAKEDLL
#		undef WITCHENGINE_DLL
#	elif defined(WITCHENGINE_MAKEDLL)
#		if defined(WITCHENGINE_DLL)
#			undef WITCHENGINE_DLL
#		endif
#		if defined(WITCHENGINE_BUILD_CORE_LIB)
#			define WITCHENGINE_CORE_EXPORT		WITCHENGINE_EXPORT
#		else
#			define WITCHENGINE_CORE_EXPORT		WITCHENGINE_IMPORT
#		endif
#		if defined(WITCHENGINE_BUILD_AUDIO_LIB)
#			define WITCHENGINE_AUDIO_EXPORT		WITCHENGINE_EXPORT
#		else
#			define WITCHENGINE_AUDIO_EXPORT		WITCHENGINE_IMPORT
#		endif
#		if defined(WITCHENGINE_BUILD_SCRIPT_LIB)
#			define WITCHENGINE_SCRIPT_EXPORT	WITCHENGINE_EXPORT
#		else
#			define WITCHENGINE_SCRIPT_EXPORT	WITCHENGINE_IMPORT
#		endif
#		if defined(WITCHENGINE_BUILD_PHYSIC_LIB)
#			define WITCHENGINE_PHYSIC_EXPORT	WITCHENGINE_EXPORT
#		else
#			define WITCHENGINE_PHYSIC_EXPORT	WITCHENGINE_IMPORT
#		endif
#		if defined(WITCHENGINE_BUILD_NETWORK_LIB)
#			define WITCHENGINE_NETWORK_EXPORT	WITCHENGINE_EXPORT
#		else
#			define WITCHENGINE_NETWORK_EXPORT	WITCHENGINE_IMPORT
#		endif
#		if defined(WITCHENGINE_BUILD_AI_LIB)
#			define WITCHENGINE_AI_EXPORT		WITCHENGINE_EXPORT
#		else
#			define WITCHENGINE_AI_EXPORT		WITCHENGINE_IMPORT
#		endif
#		if defined(WITCHENGINE_BUILD_RAYTRACER_LIB)
#			define WITCHENGINE_RAYTRACER_EXPORT	WITCHENGINE_EXPORT
#		else
#			define WITCHENGINE_RAYTRACER_EXPORT	WITCHENGINE_IMPORT
#		endif
#	elif defined(WITCHENGINE_DLL)
#		define WITCHENGINE_CORE_EXPORT 			WITCHENGINE_IMPORT
#		define WITCHENGINE_AUDIO_EXPORT			WITCHENGINE_IMPORT
#		define WITCHENGINE_SCRIPT_EXPORT		WITCHENGINE_IMPORT
#		define WITCHENGINE_PHYSIC_EXPORT		WITCHENGINE_IMPORT
#		define WITCHENGINE_NETWORK_EXPORT		WITCHENGINE_IMPORT
#		define WITCHENGINE_AI_EXPORT			WITCHENGINE_IMPORT
#		define WITCHENGINE_RAYTRACER_EXPORT		WITCHENGINE_IMPORT
#	else
#		undef WITCHENGINE_MAKEDLL
#		undef WITCHENGINE_DLL
#	endif
#endif

// Creating WitchEngine modules if WITCHENGINE_MAKEMODULE is defined.
#if defined(WITCHENGINE_MAKEMODULE)
#	define WITCHENGINE_CORE_EXPORT 			WITCHENGINE_IMPORT
#	define WITCHENGINE_AUDIO_EXPORT			WITCHENGINE_IMPORT
#	define WITCHENGINE_SCRIPT_EXPORT		WITCHENGINE_IMPORT
#	define WITCHENGINE_PHYSIC_EXPORT		WITCHENGINE_IMPORT
#	define WITCHENGINE_NETWORK_EXPORT		WITCHENGINE_IMPORT
#	define WITCHENGINE_AI_EXPORT			WITCHENGINE_IMPORT
#	define WUTCHENGINE_RAYTRACER_EXPORT		WITCHENGINE_IMPORT
#endif

#if !defined(WITCHENGINE_CORE_EXPORT)
#	if defined(WITCHENGINE_SHARED)
#		define WITCHENGINE_CORE_EXPORT 			WITCHENGINE_EXPORT
#		define WITCHENGINE_AUDIO_EXPORT			WITCHENGINE_EXPORT
#		define WITCHENGINE_SCRIPT_EXPORT		WITCHENGINE_EXPORT
#		define WITCHENGINE_PHYSIC_EXPORT		WITCHENGINE_EXPORT
#		define WITCHENGINE_NETWORK_EXPORT		WITCHENGINE_EXPORT
#		define WITCHENGINE_AI_EXPORT			WITCHENGINE_EXPORT
#		define WITCHENGINE_RAYTRACER_EXPORT		WITCHENGINE_EXPORT
#	else
#		define WITCHENGINE_CORE_EXPORT
#		define WITCHENGINE_AUDIO_EXPORT
#		define WITCHENGINE_SCRIPT_EXPORT
#		define WITCHENGINE_PHYSIC_EXPORT
#		define WITCHENGINE_NETWORK_EXPORT
#		define WITCHENGINE_AI_EXPORT
#		define WITCHENGINE_RAYTRACER_EXPORT
#	endif
#endif

/*
	This will gives the possibility to check which modules
	the user can uses.
*/
#define WITCH_MODULE_CORE		0x0001
#define WITCH_MODULE_AUDIO		0x0002
#define WITCH_MODULE_SCRIPT		0x0004
#define WITCH_MODULE_PHYSIC		0x0008
#define WITCH_MODULE_NETWORK	0x0010
#define WITCH_MODULE_AI			0x0020
#define WITCH_MODULE_RAYTRACER	0x0040

/* WithcEngine editions */
#define WITCH_EDITION_DEVELOPPER	(WITCH_MODULE_CORE \
									| WITCH_MODULE_AUDIO \
									| WITCH_MODULE_SCRIPT \
									| WITCH_MODULE_PHYSIC \
									| WITCH_MODULE_NETWORK \
									| WITCH_MODULE_AI \
									| WITCH_MODULE_RAYTRACER)
									
#define WITCH_EDITION_GAME			(WITCH_MODULE_CORE \
									| WITCH_MODULE_AUDIO \
									| WITCH_MODULE_SCRIPT \
									| WITCH_MODULE_PHYSIC \
									| WITCH_MODULE_NETWORK \
									| WITCH_MODULE_AI)
									
#define WITCH_EDITION_DESKTOPGAME	WITCH_EDITION_GAME

#define WITCH_EDITION_MOBILEGAME	WITCH_EDITION_GAME

/* Determine which modules can be used */
#ifndef WITCH_EDITION
#	error "WitchEngine not configured"
#endif

#define WITCH_LICENSED_MODULE(x) \
	enum WitchValidLicenseFor##x##Module { Licensed##x = true };
	
#if (WITCH_EDITION & WITCH_MODULE_CORE)
WITCH_LICENSED_MODULE(Core)
#endif

#if (WITCH_EDITION & WITCH_MODULE_AUDIO)
WITCH_LICENSED_MODULE(Audio)
#endif

#if (WITCH_EDITION & WITCH_MODULE_SCRIPT)
WITCH_LICENSED_MODULE(Script)
#endif

#if (WITCH_EDITION & WITCH_MODULE_PHYSIC)
WITCH_LICENSED_MODULE(Physic)
#endif

#if (WITCH_EDITION & WITCH_MODULE_NETWORK)
WITCH_LICENSED_MODULE(Network)
#endif

#if (WITCH_EDITION & WITCH_MODULE_AI)
WITCH_LICENSED_MODULE(AI)
#endif

#define WITCH_MODULE(x) \
	typedef WitchValidLicenseFor##x##Module Witch##x##Module;

// Checking the endianness.
#if WITCHENGINE_COMPILER == WITCHENGINE_COMPILER_GNUC
#	include <sys/param.h>
#	if !defined(BYTE_ORDER)
#		error "Byte order isn't defined."
#	elif BYTE_ORDER == LITTLE_ENDIAN
#		define WITCHENGINE_ENDIAN WITCHENGINE_ENDIAN_LITTLE
#	elif BYTE_ORDER == BIG_ENDIAN
#		define WITCHENGINE_ENDIAN WITCHENGINE_ENDIAN_BIG
#	else
#		error "The byte order isn't defined."
#	endif
#else // WITCHENGINE_COMPILER == WITCHENGINE_COMPILER_MSVC
#endif

// Checking the C++ standard version.
#if __cplusplus != 201103L
#	error You should use a compiler that support the C++11 standard.
#endif

// Declaring fixed width integer types.
namespace WitchEngine
{
	namespace Core
	{
		typedef unsigned char 		uint8;		/*!< Defines a signed 8-bytes width integer. */
		typedef char 				int8;		/*!< Defines an unsigned 8-bytes width integer. */
		typedef unsigned short 		uint16;		/*!< Defines a signed 16-bytes width integer. */
		typedef short 				int16;		/*!< Defines an unsigned 16-bytes width integer. */
		typedef unsigned int 		uint32;		/*!< Defines a signed 32-bytes width integer. */
		typedef int 				int32;		/*!< Defines an unsigned 32-bytes width integer. */
	
#if WITCHENGINE_COMPILER == WITCHENGINE_COMPILER_MSVC
#  define WITCH_INT64(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#  define WITCH_UINT64(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */
		typedef unsigned __int64	uint64;		/*!< Defines a signed 32-bytes width integer. */
		typedef __int64				int64;		/*!< Defines an unsigned 32-bytes width integer. */
#else
#  define WITCH_INT64(c) c ## i64    /* signed 64 bit constant */
#  define WITCH_UINT64(c) c ## ui64   /* unsigned 64 bit constant */
		typedef unsigned long long 	uint64;		/*!< Defines a signed 32-bytes width integer. */
		typedef long long 			int64;		/*!< Defines an unsigned 32-bytes width integer. */
#endif

		template <unsigned int>	struct WitchIntegerForSize;
		template <>				struct WitchIntegerForSize<1> { typedef uint8 Unsigned; typedef int8 Signed; };
		template <>				struct WitchIntegerForSize<2> { typedef uint16 Unsigned; typedef int16 Signed; };
		template <>				struct WitchIntegerForSize<4> { typedef uint32 Unsigned; typedef int32 Signed; };
		template <>				struct WitchIntegerForSize<8> { typedef uint64 Unsigned; typedef int64 Signed; };

		class WITCHENGINE_CORE_EXPORT WitchSystemInfo
		{
			public:
				enum Sizes
				{
					WordSize = (sizeof(void *) << 3)
				};
				
				enum Endian
				{
					LittleEndian,
					
					BigEndian
				
#if WITCHENGINE_ENDIAN == WITCHENGINE_ENDIAN_LITTLE
					, ByteOrder = BigEndian
#elif WITCHENGINE_ENDIAN == WITCHENGINE_ENDIAN_BIG
					, ByteOrder = LittleEndian
#endif
				};

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
				enum WinVersion
				{
					/* codenames */
					WV_NT       = 0x0010,
					WV_2000     = 0x0020,
					WV_XP       = 0x0030,
					WV_2003     = 0x0040,
					WV_VISTA    = 0x0080,
					WV_WINDOWS7 = 0x0090,
					WV_WINDOWS8 = 0x00a0,
					WV_NT_based = 0x00b0,

					/* version numbers */
					WV_4_0      = WV_NT,
					WV_5_0      = WV_2000,
					WV_5_1      = WV_XP,
					WV_5_2      = WV_2003,
					WV_6_0      = WV_VISTA,
					WV_6_1      = WV_WINDOWS7,
					WV_6_2		= WV_WINDOWS8
				};
				
				static const WinVersion WindowsVersion;
				static WinVersion windowsVersion();
#endif // WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_MACOS
				enum MacVersion
				{
					/* version */
					MV_9 = 0x0001,
					MV_10_0 = 0x0002,
					MV_10_1 = 0x0003,
					MV_10_2 = 0x0004,
					MV_10_3 = 0x0005,
					MV_10_4 = 0x0006,
					MV_10_5 = 0x0007,
					MV_10_6 = 0x0008,
					MV_10_7 = 0x0009,

					/* codenames */
					MV_CHEETAH = MV_10_0,
					MV_PUMA = MV_10_1,
					MV_JAGUAR = MV_10_2,
					MV_PANTHER = MV_10_3,
					MV_TIGER = MV_10_4,
					MV_LEOPARD = MV_10_5,
					MV_SNOWLEOPARD = MV_10_6,
					MV_LION = MV_10_7
				};
			
				static const MacVersion MacintoshVersion;
				static MacVersion macintoshVersion();
#endif // WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_MACOS
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_BLACKBERRY
				enum BlackBerryOSVersion
				{
				};
				
				static const BlackBerryOSVersion BlackBerryVersion;
				static BlackBerryOSVersion blackberryVersion();
#endif // WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_BLACKBERRY
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_ANDROID
				enum AndroidOSVersion
				{
				};
				
				static const AndroidOSVersion AndroidVersion;
				static AndroidOSVersion androidVersion;
#endif // WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_ANDROID

				enum ProcessorArchitecture
				{
					PA_AMD64 = 0x0010,
					PA_ARM = 0x0020,
					PA_IA64 = 0x0030,
					PA_INTEL = 0x0040,
					PA_UNKNOWN = 0x0050
				};
				
				static const ProcessorArchitecture Architecture;
				static ProcessorArchitecture architecture();
			};
			
			WITCHENGINE_CORE_EXPORT const char* WitchVerstion();
			WITCHENGINE_CORE_EXPORT bool WitchSharedBuild();
			
			/*
			 * Utility to avoid the "unused parameters" warning
			*/
			template <typename T>
			inline void WitchUnused(T &x) { (void)x; }
#define WITCHENGINE_UNUSED(x)	WitchUnused(x)

			/*
			 * Actually all platforms we are supporting
			 * provides a way to know if we are compiling
			 * in debug or release mode.
			 * No need to make a per-platform test.
			*/
#if defined(NDEBUG) && !defined(WITCHENGINE_NO_DEBUG)
#	define WITCHENGINE_NO_DEBUG
#endif

#if !defined(WITCHENGINE_NO_DEBUG) && !defined(WITCHENGINE_DEBUG)
#	define WITCHENGINE_DEBUG
#endif

			/*
			 * WitchTypeInfo - type traits functionnality.
			*/
			template <typename T>
			class WitchTypeInfo
			{
				public:
					enum 
					{
						isPointer = false,
						isComplex = true,
						isStatic = true,
						isLarge = (sizeof(T)>sizeof(void*)),
						isDummy = false
					};
			};
			
			template <typename T>
			class WitchTypeInfo<T *>
			{
				public:
					enum 
					{
						isPointer = true,
						isComplex = false,
						isStatic = false,
						isLarge = false,
						isDummy = false
					};
			};
			
			/*
			 * Specialize a specific type with : 
			 *		WITCHENGINE_DECLARE_TYPEINFO(type, flags)
			*/
			
			/* Type info flags. */
			enum 
			{
				WITCH_COMPLEX_TYPE = 0,
				WITCH_PRIMITIVE_TYPE = 0x1,
				WITCH_STATIC_TYPE = 0,
				WITCH_MOVABLE_TYPE = 0x2,
				WITCH_DUMMY_TYPE = 0x4
			};
			
#define WITCHENGINE_DECLARE_TYPEINFO_BODY(TYPE, FLAGS) \
			class WitchTypeInfo<TYPE> \
			{ \
				public : \
					enum \
					{ \
						isComplex = (((FLAGS) & WITCH_PRIMITIVE_TYPE) == 0), \
						isStatic = (((FLAGS) & (WITCH_MOVABLE_TYPE | WITCH_PRIMITIVE_TYPE)) == 0), \
						isLarge = (sizeof(TYPE)>sizeof(void*)), \
						isPointer = false, \
						isDummy = (((FLAGS) & WITCH_DUMMY_TYPE) != 0) \
					}; \
					static inline const char *name() { return #TYPE; } \
			}

#define WITCHENGINE_DECLARE_TYPEINFO(TYPE, FLAGS) \
			template <> \
			WITCHENGINE_DECLARE_TYPEINFO_BODY(TYPE, FLAGS)
			
			/*
			 * Type info primitive specializations.
			*/
			WITCHENGINE_DECLARE_TYPEINFO(bool, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(char, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(signed char, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(unsigned char, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(short, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(unsigned short, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(int, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(unsigned int, WITCH_PRIMITIVE_TYPE);
#if WITCHENGINE_COMPILER == WITCHENGINE_COMPILER_MSVC
			WITCHENGINE_DECLARE_TYPEINFO(__int64, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(signed __int64, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(unsigned __int64, WITCH_PRIMITIVE_TYPE);
#else
			WITCHENGINE_DECLARE_TYPEINFO(long long, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(unsigned long long, WITCH_PRIMITIVE_TYPE);
#endif
			WITCHENGINE_DECLARE_TYPEINFO(float, WITCH_PRIMITIVE_TYPE);
			WITCHENGINE_DECLARE_TYPEINFO(double, WITCH_PRIMITIVE_TYPE);
			
			/*
			 * Defining tools to get oriented object flags.
			*/
			class WITCHENGINE_CORE_EXPORT WitchFlag
			{
				int i;
				
				public:
					inline WitchFlag(int i);
					inline operator int() const { return i; }
			};
			
			inline WitchFlag::WitchFlag(int ai) : i(ai) {}
			
			class WITCHENGINE_CORE_EXPORT WitchIncompatibleFlag
			{
				int i;
			public:
				inline explicit WitchIncompatibleFlag(int i);
				inline operator int() const { return i; }
			};
			
			inline WitchIncompatibleFlag::WitchIncompatibleFlag(int ai) : i(ai) {}
			
			template <typename Enum>
			class WitchFlags
			{
				typedef void **Zero;
				int i;
				
				public:
					typedef Enum enum_type;
					inline WitchFlags(const WitchFlags &f) : i(f.i) { }
					inline WitchFlags(Enum f) : i(f) { }
					inline WitchFlags(Zero = 0) : i(0) { }
					inline WitchFlags(WitchFlag f) : i(f) { }
					
					inline WitchFlags &operator=(const WitchFlags &f) { i = f.i; return *this; }
					inline WitchFlags &operator&=(int mask) { i &= mask; return *this; }
					inline WitchFlags &operator&=(uint32 mask) { i &= mask; return *this; }
					inline WitchFlags &operator|=(WitchFlags f) { i |= f.i; return *this; }
					inline WitchFlags &operator|=(Enum f) { i |= f; return *this; }
					inline WitchFlags &operator^=(WitchFlags f) { i ^= f.i; return *this; }
					inline WitchFlags &operator^=(Enum f) { i ^= f; return *this; }

					inline operator int() const { return i; }

					inline WitchFlags operator|(WitchFlags f) const { return WitchFlags(Enum(i | f.i)); }
					inline WitchFlags operator|(Enum f) const { return WitchFlags(Enum(i | f)); }
					inline WitchFlags operator^(WitchFlags f) const { return WitchFlags(Enum(i ^ f.i)); }
					inline WitchFlags operator^(Enum f) const { return WitchFlags(Enum(i ^ f)); }
					inline WitchFlags operator&(int mask) const { return WitchFlags(Enum(i & mask)); }
					inline WitchFlags operator&(uint32 mask) const { return WitchFlags(Enum(i & mask)); }
					inline WitchFlags operator&(Enum f) const { return WitchFlags(Enum(i & f)); }
					inline WitchFlags operator~() const { return WitchFlags(Enum(~i)); }

					inline bool operator!() const { return !i; }
			};
#define WITCHENGINE_DECLARE_FLAGS(Flags, Enum) \
	typedef WitchFlags<Enum> Flags;
	
#define WITCHENGINE_DECLARE_INCOMPATIBLE_FLAGS(Flags) \
inline WitchIncompatibleFlag operator|(Flags::enum_type f1, int f2) \
{ return WitchIncompatibleFlag(int(f1) | f2); }

#define WITCHENGINE_DECLARE_OPERATORS_FOR_FLAGS(Flags) \
inline WitchFlags<Flags::enum_type> operator|(Flags::enum_type f1, Flags::enum_type f2) \
{ return WitchFlags<Flags::enum_type>(f1) | f2; } \
inline WitchFlags<Flags::enum_type> operator|(Flags::enum_type f1, WitchFlags<Flags::enum_type> f2) \
{ return f2 | f1; } WITCHENGINE_DECLARE_INCOMPATIBLE_FLAGS(Flags)

/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/
#define WITCH_DISABLE_COPY(Class) \
	private : \
		Class(const Class &) = delete; \
		Class(Class &&) = delete; \
		void operator= (const Class &) = delete; \
		void operator= (Class &&) = delete;
		
			WITCHENGINE_CORE_EXPORT char* getenv(const char *varName);
			WITCHENGINE_CORE_EXPORT bool putenv(const char *varName, const char* value);
			
			/*
			 * Defining tools to create type list.
			*/
			/*!
			 \brief This structure represents a null type. Useful to represents the end of a TypeList.
			*/
			struct NullType { };	// Defining an empty type to represent the end of a list.
			
			/**
				\struct TypeList
				\author Alexandre Valentin Jamet
				\date 07 Junuary 2013
				\brief Provides a way to create a list of type. 
				
				\remarks Defines has been created to simplify creation of list :
				\code{.cpp}
					typedef TypeList<char, TypeList<short, TypeList<int, NullType> > > IntegerTypes;
					// Previous line equivalent to the next.
					typedef TYPELIST_3(char, short, int> IntegerTypes;
				\endcode
				
				The define specifies how much types you can put. TYPELIST_5 will create a type list of 5 types.
				Algorithms are also defined to act on TypeList, these algorithms take form of external template structures.
			**/
			template <typename T, typename S>
			struct TypeList
			{
				typedef T Head;	/*!< Represents the type at the current level. */
				typedef S Tail;	/*!< If NullType represents the end of the list otherwise represents the next level of the list. */
			};
			
#define TYPELIST_1(t1) \
						TypeList<t1, NullType>
#define TYPELIST_2(t1, t2) \
						TypeList<t1, TYPELIST_1(t1)>
#define TYPELIST_3(t1, t2, t3) \
						TypeList<t1, TYPELIST_2(t2, t3)>
#define TYPELIST_4(t1, t2, t3, t4) \
						TypeList<t1, TYPELIST_3(t2, t3, t4)>
#define TYPELIST_5(t1, t2, t3, t4, t5) \
						TypeList<t1, TYPELIST_4(t2, t3, t4, t5)>
#define TYPELIST_6(t1, t2, t3, t4, t5, t6) \
						TypeList<t1, TPYELIST_5(t2, t3, t4, t5, t6)>
#define TYPELIST_7(t1, t2, t3, t4, t5, t6, t7) \
						TypeList<t1, TYPELIST_6(t2, t3, t4, t5, t6, t7)>
#define TYPELIST_8(t1, t2, t3, t4, t5, t6, t7, t8) \
						TypeList<t1, TYPELIST_7(t2, t3, t4, t5, t6, t7, t8)>
#define TYPELIST_9(t1, t2, t3, t4, t5, t6, t7, t8, t9) \
						TypeList<t1, TYPELIST_8(t2, t3, t4, t5, t6, t7, t8, t9)>
#define TYPELIST_10(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10) \
						TypeList<t1, TYPELIST_9(t2, t3, t4, t5, t6, t7, t8, t9, t10)>
#define TYPELIST_11(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11) \
						TypeList<t1, TYPELIST_10(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11)>
#define TYPELIST_12(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12) \
						TypeList<t1, TYPELIST_11(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12)>
#define TYPELIST_13(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13) \
						TypeList<t1, TYPELIST_12(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13)>
#define TYPELIST_14(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14) \
						TypeList<t1, TYPELIST_13(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14)>
#define TYPELIST_15(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15) \
						TypeList<t1, TYPELIST_14(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15)>
#define TYPELIST_16(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16) \
						TypeList<t1, TYPELIST_15(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16)>
#define TYPELIST_17(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17) \
						TypeList<t1, TYPELIST_16(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17)>
#define TYPELIST_18(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18) \
						TypeList<t1, TYPELIST_17(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18)>
#define TYPELIST_19(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19) \
						TypeList<t1, TYPELIST_18(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19)>
#define TYPELIST_20(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20) \
						TypeList<t1, TYPELIST_19(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20)>
			
			// TypeList algorithm: SizeOf
			//	- Param 	: a type list
			//	- Return 	: the size of the list.
			template <typename List> struct SizeOf;
			
			template <typename Head, typename Tail>
			struct SizeOf<TypeList<Head, Tail> >
			{
				enum
				{
					Value = 1 + SizeOf<Tail>::Value
				};
			};
			
			template <>
			struct SizeOf<NullType>
			{
				enum
				{
					Value = 0
				};
			};
			
			// TypeList algorithm: Contains
			//	- Param		: a type list.
			//	- Param		: a type.
			//	- Return	: Does the list contains the type.
			template <typename List, typename Type> struct Contains;
			
			template <typename Head, typename Tail, typename Type>
			struct Contains<TypeList<Head, Tail>, Type>
			{
				enum
				{
					Value = Contains<Tail, Type>::Value
				};
			};
			
			template <typename Type, typename Tail>
			struct Contains<TypeList<Type, Tail>, Type>
			{
				enum
				{
					Value = true
				};
			};
			
			template <typename Type>
			struct Contains<NullType, Type>
			{
				enum
				{
					Value = false
				};
			};
			
			/*
			 * Defining design patterns.
			*/
			template <typename Class>
			class Singleton
			{
				public:
					static Class* instance()
					{
						// It assume that "Class" contains a default constructor.
						if(!_singleton)
							_singleton = new Class;
							
						return _singleton;
					}
					
					static void destroy()
					{
						if(_singleton)
							delete _singleton;
							
						_singleton = nullptr;
					}
					
				protected:
					Singleton() { }
					~Singleton() { }
					
				private:
					Singleton(const Singleton &) = delete;
					Singleton(Singleton &&) = delete;
					void operator =(const Singleton &) = delete;
					void operator =(Singleton &&) = delete;
				
					static Class *_singleton;
			};
			
			template <typename Class>
			Class* Singleton<Class>::_singleton = nullptr;
	}
}

#endif // __WITCHENGINE_CORE_WITCHGLOBAL_HPP__