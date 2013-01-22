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

#include "Plugin.hpp"

#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#	include "Win32/PluginImpl.hpp"
#else
#	include "Posix/PluginImpl.hpp"
#endif

namespace WitchEngine
{
	namespace Core
	{
		Plugin::Plugin() :
			_impl(nullptr),
			_pluginPath()
		{
		}
		
		Plugin::Plugin(const String &pluginName) :
			_impl(nullptr),
			_pluginPath(pluginName)
		{
		}
		
		Plugin::~Plugin()
		{
			unload();
		}
		
		bool Plugin::load()
		{
			unload();
			
			_impl = new PluginImpl;
			if(!_impl->load(_pluginPath))
			{
				delete _impl;
				_impl = nullptr;
				
				return false;
			}
			
			return true;
		}
		
		bool Plugin::load(const String &pluginPath)
		{
			unload();
			
			_impl = new PluginImpl;
			if(!_impl->load(pluginPath))
			{
				delete _impl;
				_impl = nullptr;
				
				return false;
			}
			
			_pluginPath = pluginPath;
			
			return false;
		}
		
		void Plugin::unload()
		{
			if(_impl)
			{
				_impl->unload();
				delete _impl;
				_impl = nullptr;
			}
		}
		
		bool Plugin::loaded() const
		{
			return (_impl == nullptr);
		}
		
		String Plugin::path() const
		{
			return _pluginPath;
		}
		
		void Plugin::setPath(const String &path)
		{
			if(loaded())
			{
				PluginImpl *impl = new PluginImpl;
				if(!impl->load(path))
				{
					delete impl;
				}
				
				_impl->unload();
				delete _impl;
				
				_impl = impl;
			}
			
			_pluginPath = path;
		}
		
		IModule* Plugin::instance()
		{
			// Checking if the module is loaded.
			if(!loaded())
			{
				// TODO: Throw an excpetion.
				return nullptr;
			}
			
			// Getting an unique instance of the module.
			static IModule *inst = _impl->getSymbol("GetPlugin")();
			
			return inst;
		}
	}
}