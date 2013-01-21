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