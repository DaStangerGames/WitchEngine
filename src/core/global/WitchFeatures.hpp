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

#ifndef __WITCHENGINE_CORE_WITCHFEATURES_HPP__
#define __WITCHENGINE_CORE_WITCHFEATURES_HPP__

// On the fly renderer loading
#if !defined(WITCHENGINE_NO_ON_THE_FLY_RENDERER_LOADING) && (defined(WITCHENGINE_NO_EXTERNAL_MODULES_LOADER))
#	define WITCHENGINE_NO_ON_THE_FLY_RENDERER_LOADING
#endif

// Collada model file format
#if !defined(WITCHENGINE_NO_MODELFORMAT_COLLADA) && (defined(WITCHENGINE_NO_XMLREADER) || defined(WITCHENGINE_NO_XMLWRITER))
#	define WITCHENGINE_NO_MODELFORMAT_COLLADA
#endif

// OpenGL Renderer
#if !defined(WITCHENGINE_NO_OPENGL_RENDERER) && (defined(WITCHENGINE_NO_EXTERNAL_MODULES_LOADER))
#	define WITCHENGINE_NO_OPENGL_RENDERER
#endif

// OpenGL/ES Renderer
#if !defined(WITCHENGINE_NO_OPENGLES_RENDERER) && (defined(WITCHENGINE_NO_EXTERNAL_MODULES_LOADER))
#	define WITCHENGINE_NO_OPENGLES_RENDERER
#endif

// DirectX 9 Renderer
#if !defined(WITCHENGINE_NO_DIRECTX9_RENDERER) && (defined(WITCHENGINE_NO_EXTERNAL_MODULES_LOADER))
#	define WITCHENGINE_NO_DIRECTX9_RENDERER
#endif

// DirectX 10 Renderer
#if !defined(WITCHENGINE_NO_DIRECTX10_RENDERER) && (defined(WITCHENGINE_NO_EXTERNAL_MODULES_LOADER))
#	define WITCHENGINE_NO_DIRECTX10_RENDERER
#endif

// DirectX 11 Renderer
#if !defined(WITCHENGINE_NO_DIRECTX11_RENDERER) && (defined(WITCHENGINE_NO_EXTERNAL_MODULES_LOADER))
#	define WITCHENGINE_NO_DIRECTX11_RENDERER
#endif

// Vertex Shader
#if !defined(WITCHENGINE_NO_VERTEX_SHADER) && (defined(WITCHENGINE_NO_OPENGL_RENDERER) && defined(WITCHENGINE_NO_OPENGLES_RENDERER) && defined(WITCHENGINE_NO_DIRECTX9) && defined(WITCHENGINE_NO_DIRECTX10) && defined(WITCHENGINE_NO_DIRECTX11))
#endif

// Fragment Shader
#if !defined(WITCHENGINE_NO_FRAGMENT_SHADER) && (defined(WITCHENGINE_NO_OPENGL_RENDERER) && defined(WITCHENGINE_NO_OPENGLES_RENDERER) && defined(WITCHENGINE_NO_DIRECTX9) && defined(WITCHENGINE_NO_DIRECTX10) && defined(WITCHENGINE_NO_DIRECTX11))
#endif

// Geometry Shader
#if !defined(WITCHENGINE_NO_GEOMETRY_SHADER) && (defined(WITCHENGINE_NO_OPENGL_RENDERER) && defined(WITCHENGINE_NO_DIRECTX10_RENDERER) && defined(WITCHENGINE_NO_DIRECTX11_RENDERER))
#endif

// Compute Shader
#if !defined(WITCHENGINE_NO_COMPUTE_SHADER) && (defined(WITCHENGINE_NO_OPENGL_RENDERER) && defined(WITCHENGINE_NO_DIRECTX11_RENDERER))
#endif

// Ray-tracing Renderer
#if !defined(WITCHENGINE_NO_RAYTRACING_RENDERER) && (defined(WITCHENGINE_NO_EXTERNAL_MODULES_LOADER))
#	define WITCHENGINE_NO_RAYTRACING_RENDERER
#endif

// Math models
#if !defined(WITCHENGINE_NO_MATH_MODELS) && (defined(WITCHENGINE_NO_PHYSIC) || defined(WITCHENGINE_NO_EQUATIONPARSER))
#	define WITCHENGINE_NO_MATH_MODELS
#endif

// Mechanical simulation
#if !defined(WITCHENGINE_NO_MECHANICAL_SIMULATION) && (defined(WITCHENGINE_NO_PHYSIC) || defined(WITCHENGINE_NO_MATH_MODELS))
#	define WITCHENGINE_NO_MECHANICAL_SIMULATION
#endif

// Python support
#if !defined(WITCHENGINE_NO_PYTHON_SUPPORT) && (defined(WITCHENGINE_NO_SCRIPT))
#	define WITCHENGINE_NO_PYTHON_SUPPORT
#endif

// Lua support
#if !defined(WITCHENGINE_NO_LUA_SUPPORT) && (defined(WITCHENGINE_NO_SCRIPT))
#	define WITCHENGINE_NO_LUA_SUPPORT
#endif

// WitchScript
#if !defined(WITCHENGINE_NO_WITCHSCRIPT) && (defined(WITCHENGINE_NO_SCRIPT))
#	define WITCHENGINE_NO_WITCHSCRIPT
#endif

// Scriptable shaders
#if !defined(WITCHENGINE_NO_SCRIPTABLE_SHADERS) && (defined(WITCHENGINE_NO_SCRIPT))
#	define WITCHENGINE_NO_SCRIPTABLE_SHADERS
#endif

// Scriptable environment
#if !defined(WITCHENGINE_NO_SCRIPTABLE_ENVIRONMENT) && (defined(WITCHENGINE_NO_SCRIPT))
#	define WITCHENGINE_NO_SCRIPTABLE_ENVIRONMENT
#endif

// Genetic algorithms
#if !defined(WITCHENGINE_NO_GENETIC_ALGORITHMS) && (defined(WITCHENGINE_NO_ARTIFICIAL_INTELLIGENCE))
#	define WITCHENGINE_NO_GENETIC_ALGORITHMS
#endif

#endif // __WITCHENGINE_CORE_WITCHFEATURES_HPP__