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

#ifndef __WITCHENGINE_CORE_WITCHENGINEENUMS_HPP__
#define __WITCHENGINE_CORE_WITCHENGINEENUMS_HPP__

namespace WitchEngine
{
	namespace Core
	{
		enum Key
		{
			Key_A,
			
			Key_B,
			
			Key_C,
			
			Key_D,
			
			Key_E,
			
			Key_F,
			
			Key_G,
			
			Key_H,
			
			Key_I,
			
			Key_J,
			
			Key_K,
			
			Key_L,
			
			Key_M,
			
			Key_N,
			
			Key_O,
			
			Key_P,
			
			Key_Q,
			
			Key_R,
			
			Key_S,
			
			Key_T,
			
			Key_U,
			
			Key_V,
			
			Key_W,
			
			Key_X,
		
			Key_Y,
			
			Key_Z
		};
		
		enum LoggingLevel
		{
			Information,
			
#if WITCHENGINE_DEBUG_MODE == 0x1
			Debug,
#endif
			
			Warning,
			
			Error
		};
		
		enum OpenModeFlag
		{
			ReadOnly = 0x0001,					/*!< The device is open for reading. */
			
			WriteOnly = 0x0002,					/*!< The device is open for writing. */
			
			ReadWrite = ReadOnly | WriteOnly,	/*!< The device is open for reading and writing. */
			
			Append = 0x0004,					/*!< The device is opened in append mode, so that all data is written to the end of the file. */
			
			Truncate = 0x0008,					/*!< If possible, the device is truncated before it is opened. All earlier contents of the device are lost. */
			
			Text = 0x0010						/*!< When reading, the end-of-line terminators are translated to '\n'. When writing, the end-of-line terminators are translated to the local encoding, for example '\r\n' for Win32. */
		};
		WITCHENGINE_DECLARE_FLAGS(OpenMode, OpenModeFlag)
		
		enum KeyboardModifier
		{
			NoModifier = 0x00,
			ControlModifier = 0x01,
			ShiftModifier = 0x02,
			AltModifier = 0x4
		};
		WITCHENGINE_DECLARE_FLAGS(KeyboardModifiers, KeyboardModifier)
		
		enum MouseButton
		{
			NoButton = 0x00,
			LeftButton = 0x01,
			RightButton = 0x02,
			MiddleButton = 0x04
		};
		WITCHENGINE_DECLARE_FLAGS(MouseButtons, MouseButton)
		
		enum ArrowType
		{
			NoArrow = 0x00,
			UpArrow = 0x01,
			DownArrow = 0x02,
			LeftArrow = 0x04,
			RightArrow = 0x08
		};
		
		enum Axis
		{
			XAxis,
			YAxis,
			ZAxis
		};
		
		enum ShaderType
		{
			VertexShader,
			FragmentShader,
			GeometryShader,
			ComputeShader
		};
		
		enum GraphicalDriver
		{
			DirectX = 0x0001,	/*!< */
			OpenGL = 0x0002,	/*!< */
			OpenGL_ES = 0x0004	/*!< */
		};
		
		/**
		 * \enum GraphicalDriverVersion
		 * \brief Represents which version of the graphical API we are using.
		**/
		enum GraphicalDriverVersion
		{
			DirectX_9		= 0x0010 | DirectX,		/*!< */
			DirectX_10 		= 0x0020 | DirectX,		/*!< */
			DirectX_11		= 0x0040 | DirectX,		/*!< */
			OpenGL_3_0		= 0x0010 | OpenGL,		/*!< */
			OpenGL_3_1		= 0x0020 | OpenGL,		/*!< */
			OpenGL_3_2		= 0x0040 | OpenGL,		/*!< */
			OpenGL_3_3		= 0x0080 | OpenGL,		/*!< */
			OpenGL_3_4		= 0x0100 | OpenGL,		/*!< */
			OpenGL_4_0		= 0x0200 | OpenGL,		/*!< */
			OpenGL_4_1		= 0x0400 | OpenGL,		/*!< */
			OpenGL_4_2		= 0x0800 | OpenGL,		/*!< */
			OpenGL_4_3		= 0x1000 | OpenGL,		/*!< */
			OpenGL_ES_1_0	= 0x0010 | OpenGL_ES,	/*!< */
			OpenGL_ES_1_1	= 0x0020 | OpenGL_ES,	/*!< */
			OpenGL_ES_2_0	= 0x0040 | OpenGL_ES,	/*!< */
			OpenGL_ES_3_0	= 0x0080 | OpenGL_ES	/*!< */
		};
	}
}

#endif // __WITCHENGINE_CORE_WITCHENGINEENUMS_HPP__