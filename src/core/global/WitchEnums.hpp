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

#ifndef __WITCHENGINE_CORE_WITCHENGINEENUMS_HPP__
#define __WITCHENGINE_CORE_WITCHENGINEENUMS_HPP__

WITCH_BEGIN_HEADER

WITCH_MODULE(Core)

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
			VertexShader,		/*!< */
			FragmentShader,		/*!< */ 
			GeometryShader,		/*!< */
			ComputeShader,		/*!< */
			RayTracingShader
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
#ifndef WITCHENGINE_NO_DIRECTX9_RENDERER // Only defined when available
			DirectX_9		= 0x0010 | DirectX,		/*!< */
#endif
#ifndef WITCHENGINE_NO_DIRECTX10_RENDERER // Only defined when available
			DirectX_10 		= 0x0020 | DirectX,		/*!< */
#endif
#ifndef WITCHENGINE_NO_DIRECTX11_RENDERER // Only defined when available
			DirectX_11		= 0x0040 | DirectX,		/*!< */
#endif
#ifndef WITCHENGINE_NO_OPENGL_RENDERER	// Only defined when available
			OpenGL_3_0		= 0x0010 | OpenGL,		/*!< */
			OpenGL_3_1		= 0x0020 | OpenGL,		/*!< */
			OpenGL_3_2		= 0x0040 | OpenGL,		/*!< */
			OpenGL_3_3		= 0x0080 | OpenGL,		/*!< */
			OpenGL_3_4		= 0x0100 | OpenGL,		/*!< */
			OpenGL_4_0		= 0x0200 | OpenGL,		/*!< */
			OpenGL_4_1		= 0x0400 | OpenGL,		/*!< */
			OpenGL_4_2		= 0x0800 | OpenGL,		/*!< */
			OpenGL_4_3		= 0x1000 | OpenGL,		/*!< */
#endif
#ifndef WITCHENGINE_NO_OPENGLES_RENDERER // Only defined when available
			OpenGL_ES_1_0	= 0x0010 | OpenGL_ES,	/*!< */
			OpenGL_ES_1_1	= 0x0020 | OpenGL_ES,	/*!< */
			OpenGL_ES_2_0	= 0x0040 | OpenGL_ES,	/*!< */
			OpenGL_ES_3_0	= 0x0080 | OpenGL_ES	/*!< */
#endif
		};
		
		enum AlignmentFlag
		{
			AlignLeft = 0x0001,
			AlignLeading = AlignLeft,
			AlignRight = 0x0002,
			AlignTrailing = AlignRight,
			AlignHCenter = 0x0004,
			AlignJustify = 0x0008,
			AlignAbsolute = 0x0010,
			AlignHorizontal_Mask = AlignLeft | AlignRight | AlignHCenter | AlignJustify | AlignAbsolute,
			
			AlignTop = 0x0020,
			AlignBottom = 0x0040,
			AlignVCenter = 0x0080,
			AlignVerticalMask = AlignTop | AlignBottom | AlignVCenter
			
			AlignCenter = AlignHCenter | AlignVCenter
		};
		WITCH_DECLARE_FLAGS(Alignment, AlignmentsFlag)
		
		enum WindowType
		{
			Widget = 0x00000000,
			Window = 0x00000001,
			RendererWindow = 0x00000002 | Window,
			Dialog = 0x00000004 | Window,
			Tool = 0x00000008 | Window,
			Popup = 0x00000010 | Window
			ToolTip = 0x00000020 | Window
		};
		WITCH_DECLARE_FLAGS(WindowFlags, WindowType)
		
		enum WindowState
		{
			WindowNoState = 0x00000000,
			WindowMinized = 0x00000001,
			WindowMaximized = 0x00000002,
			WindowFullScreen = 0x00000004,
			WindowActive = 0x00000008
		};
		WITCH_DECLARE_FLAGS(WindowStates, WindowState)
		
		enum RendererType
		{
			RealTimeRenderer = 0x0001,
			RayTracingRenderer = 0x0002,
			MovieRenderer = 0x0004,
			FrameRenderer = 0x0008,
			
			CinematicRenderer = RayTracingRenderer | MovieRenderer,
			ImageRenderer = FrameRenderer | RayTracingRenderer,
			GameRenderer = RealTimeRenderer
		};
		WITCH_DECLARE_FLAGS(RendererType, RendererFlags)
		
		enum RendererAttribute
		{
			RA_MovableMesh = 0,
			RA_ShowAxis = 1,
			RA_ShowFrameByFrame = 2,
			RA_NoShader = 3,
			RA_ShowableMenus = 4,
			RA_MovableCamera = 5,
			RA_UseMouseToMoveCamera = 6,
			RA_UseApiShader = 7,
			RA_UseRayTracingShader = 8,
			RA_WireFrameMode = 9,
			RA_TexturedMode = 10,
			RA_MaterialMode = 11,
			RA_ObjectMode = 12,
		};
		WITCH_DECLARE_FLAGS(RendererAttributes, RendererAttribute)
	}
}

WITCH_END_HEADER

#endif // __WITCHENGINE_CORE_WITCHENGINEENUMS_HPP__