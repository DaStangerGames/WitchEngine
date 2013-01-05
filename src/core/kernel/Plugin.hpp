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

#ifndef __WITCHENGINE_CORE_PLUGIN_HPP__
#define __WITCHENGINE_CORE_PLUGIN_HPP__

#include <WitchCore/WitchGlobal.hpp>
#include <WitchCore/Exception.hpp>
#include <WitchCore/String.hpp>

namespace WitchEngine
{
	namespace Core
	{
#if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
		typedef HMODULE	ModuleHandle;
#	define OpenLibrary(path)			LoadLibrary(path)
#	define GetSymbol(handle, symbol)	GetProcAddress(handle, symbol)
#	define CloseLibrary(handle)			FreeLibrary(handle)
#else
		typedef void*	ModuleHandle;
#	define OpenLibrary(path)			dlopen(path, RTLD_LAZY)
#	define GetSymbol(handle, symbol)	dlsym(handle, symbol)
#	define CloseLibrary(handle)			dlclose(handle)
#endif

		/**
		 * \class IModule
		 * \author Alexandre Valentin Jamet
		 * \date 26 December 2012
		 * \brief This is the class from which the plug-ins should be derived.
		**/
		class WITCHENGINE_CORE_EXPORT IModule
		{
			public:
				IModule();
				virtual ~IModule();
				
				virtual const char* name() const = 0;
				virtual const char* author() const = 0;
				virtual const char* version() const = 0;
		};
		
		/**
		 * \class Plugin
		 * \author Alexandre Valentin Jamet
		 * \date 25 December 2012
		 * \brief This class provides a way to load at run-time library to extend functionnalities.
		 * \todo The class should be thread-safe. We should add mutex.
		**/
		template <typename... Arguments>
		class Plugin
		{
			public:
				Plugin();
				Plugin(const String &filename);
				~Plugin();
				
				Plugin(const Plugin &other) = delete;
				Plugin(Plugin &&other) = delete;
				void operator =(const Plugin &other) = delete;
				void operator =(Plugin &&other) = delete;
				
				void load();
				void load(const String &filename);
				
				void unload();
				
				String filename() const;
				void setFilename(const String &filename);
				
				IModule* instance(Arguments... args);
				
				bool loaded() const;
				
			private:
				typedef IModule* (*PtrFunc)(Arguments...);
				
				ModuleHandle _plugin;	/*!< The plugin handle. */
				PtrFunc	_function;		/*!< The function do load the instance of the plugin. */
				String _file;			/*!< The file from which should be loaded. */
				bool _loaded;			/*!< Does the plugin is loaded. */
		};
		
		/**
		 * \fn Plugin::Plugin()
		 * \brief  It's the default constructor of the Plugin class.
		**/
		template <typename... Arguments>
		Plugin<Arguments...>::Plugin() :
			_plugin(nullptr),
			_function(nullptr),
			_file(),
			_loaded(false)
		{
		}
		
		/**
		 * \fn Plugin::Plugin(const String &filename)
		 * \param filename The name of the file from which should be loaded the plugin.
		 * \brief This plugin is constructed from filename.
		**/
		template <typename... Arguments>
		Plugin<Arguments...>::Plugin(const String &filename) :
			_plugin(nullptr),
			_function(nullptr),
			_file(filename),
			_loaded(false)
		{
		}
		
		/**
		 * \fn Plugin::~Plugin
		 * \brief The destructor of the Plugin class.
		**/
		template <typename... Arguments>
		Plugin<Arguments...>::~Plugin()
		{
			// Closing the library.
			if(_plugin)
				CloseLibrary(_plugin);
		}
		
		/**
		 * \fn void Plugin::load()
		 * \brief This load the plugin form the registered filename.
		**/
		template <typename... Arguments>
		void Plugin<Arguments...>::load()
		{
			// Checking if the plugin is already loaded.
			if(_loaded)
				return;
			
			// Loading the plugin.
			_plugin = OpenLibrary(_file.constBuffer());
			
			// Loading failed, throwing an exception and exiting the function.
			if(!_plugin)
			{
				throw LoadingFailed("", __LINE__, __FILE__, __FUNCTION__);
				return;
			}
			
			// Loading the requested symbol.
			_function = reinterpret_cast<PtrFunc>(GetSymbol(_plugin, "GetPlugin"));
			
			// Loading failed, throwing an exception and exiting the function.
			if(!_function)
			{
				throw LoadingFailed("", __LINE__, __FILE__, __FUNCTION__);
				return;
			}
			
			// Telling that the plugin is loaded.
			_loaded = true;
		}
		
		/**
		 * \fn void Plugin::load(const String &filename)
		 * \param filename The file from which should be loaded the plugin.
		 * \brief The function load the plugin from the specified filename.
		**/
		template <typename... Arguments>
		void Plugin<Arguments...>::load(const String &filename)
		{
			// Checking if the plugin is already loaded.
			if(_loaded)
				return;
			
			// Loading the plugin.			
			_plugin = OpenLibrary(filename.constBuffer());
			
			// Loading failed, throwing an exception and exiting the function.
			if(!_plugin)
			{
				throw LoadingFailed("", __LINE__, __FILE__, __FUNCTION__);
				return;
			}
			
			// Loading the requested symbol.
			_function = reinterpret_cast<PtrFunc>(GetSymbol(_plugin, "GetPlugin"));
			
			// Loading failed, throwing an exception and exiting the function.
			if(!_function)
			{
				throw LoadingFailed("", __LINE__, __FILE__, __FUNCTION__);
				return;
			}
			
			// Telling that the plugin is loaded.
			_loaded = true;
		}
		
		/**
		 * \fn void Plugin::unload()
		 * \brief THe function unload the plugin and set to null all values.
		**/
		template <typename... Arguments>
		void Plugin<Arguments...>::unload()
		{
			// Checking is the plugin isn't loaded.
			if(!_loaded)
				return;
				
			// Unloading plugin.
			CloseLibrary(_plugin);
			
			_plugin = nullptr;
			_function = nullptr;
			
			_loaded = false;
		}
		
		/**
		 * \fn String Plugin::filename() const
		 * \brief The function returns the name of the file should be loaded the plugin.
		**/
		template <typename... Arguments>
		String Plugin<Arguments...>::filename() const
		{
			return _file;
		}
		
		/**
		 * \fn void Plugin::setFilename(const String &filename)
		 * \param filename The name of the file from which should be loaded the plugin.
		 * \brief The function set the filename to the new one that is specified as parameters.
		**/
		template <typename... Arguments>
		void Plugin<Arguments...>::setFilename(const String &filename)
		{
			// Unloading the plugin if loaded.
			if(_loaded)	
				unload();
				
			_file = filename;
		}
		
		/**
		 * \fn IModule* Plugin::instance(Arguments... args)
		 * \param The list of parameters that the plugin loader need.
		 * \return An instance of the plugin.
		 * \brief The function return the an instance of the plugin.
		**/
		template <typename... Arguments>
		IModule* Plugin<Arguments...>::instance(Arguments... args)
		{
			return _function(args...);
		}
		
		/**
		 * \fn bool Plugin::loaded() const
		 * \return Is the plugin loaded or not ?
		 * \brief The function return a boolean that tell if the plugin is loaded or not.
		**/
		template <typename... Arguments>
		bool Plugin<Arguments...>::loaded() const
		{
			return _loaded;
		}
			
		// Declaring helper defines for module and plugin.
#	if WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN32 || WITCHENGINE_PLATFORM == WITCHENGINE_PLATFORM_WIN64
#		define WITCHENGINE_EXPORT_PLUGIN(PluginClass, ...) \
			static_assert(std::is_base_of<WitchEngine::Core::IModule, PluginClass>, "The plugin class should be inherited of IModule."); \
			export "C" __declspec(dllexport) WitchEngine::Core::IModule* GetPlugin( ## __VA_ARGS__ )
#else
#		define WITCHENGINE_EXPORT_PLUGIN(PluginClass, ...) \
			static_assert(std::is_base_of<WitchEngine::Core::IModule, PluginClass>, "The plugin class should be inherited of IModule."); \
			export "C" __attribute__((visibility("default"))) WitchEngine::Core::IModule* GetPlugin( ## __VA_ARGS__)
#endif
	}
}

#endif // __WITCHENGINE_CORE_PLUGIN_HPP__