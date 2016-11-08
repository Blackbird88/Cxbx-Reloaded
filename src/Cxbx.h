// ******************************************************************
// *
// *    .,-:::::    .,::      .::::::::.    .,::      .:
// *  ,;;;'````'    `;;;,  .,;;  ;;;'';;'   `;;;,  .,;;
// *  [[[             '[[,,[['   [[[__[[\.    '[[,,[['
// *  $$$              Y$$$P     $$""""Y$$     Y$$$P
// *  `88bo,__,o,    oP"``"Yo,  _88o,,od8P   oP"``"Yo,
// *    "YUMMMMMP",m"       "Mm,""YUMMMP" ,m"       "Mm,
// *
// *   Cxbx->Cxbx.h
// *
// *  This file is part of the Cxbx project.
// *
// *  Cxbx and Cxbe are free software; you can redistribute them
// *  and/or modify them under the terms of the GNU General Public
// *  License as published by the Free Software Foundation; either
// *  version 2 of the license, or (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// *  GNU General Public License for more details.
// *
// *  You should have recieved a copy of the GNU General Public License
// *  along with this program; see the file COPYING.
// *  If not, write to the Free Software Foundation, Inc.,
// *  59 Temple Place - Suite 330, Bostom, MA 02111-1307, USA.
// *
// *  (c) 2002-2003 Aaron Robinson <caustik@caustik.com>
// *
// *  All rights reserved
// *
// ******************************************************************
#ifndef CXBX_H
#define CXBX_H

/*! CxbxKrnl exports, others import */
#ifndef _CXBXKRNL_INTERNAL
#define CXBXKRNL_API __declspec(dllimport)
#else
#define CXBXKRNL_API __declspec(dllexport)
#endif

/*! \name primitive typedefs */
/*! \{ */
typedef signed int     sint;
typedef unsigned int   uint;
typedef char           int08;
typedef short          int16;
typedef long           int32;
typedef unsigned char  uint08;
typedef unsigned short uint16;
typedef unsigned long  uint32;
typedef signed char    sint08;
typedef signed short   sint16;
typedef signed long    sint32;
/*! \} */

/*! define this to track vertex buffers */
#define _DEBUG_TRACK_VB
/*! define this to track vertex shaders */
#define _DEBUG_TRACK_VS
/*! define this to track pixel shaders */
#define _DEBUG_TRACK_PS
/*! define this to track push buffers */
#define _DEBUG_TRACK_PB
/*! define this to track memory allocations */
#ifdef _DEBUG
//#define _DEBUG_ALLOC
#endif
/*! define this to trace intercepted function calls */
#ifdef _DEBUG
#define _DEBUG_TRACE 1
#endif
/*! define this to trace warnings */
#define _DEBUG_WARNINGS
/*! define this to trace vertex shader constants */
#define _DEBUG_TRACK_VS_CONST

/*! define this to dump textures that have been set */
//#define _DEBUG_DUMP_TEXTURE_SETTEXTURE "D:\\xbox\\_textures\\"
/*! define this to dump textures that are registered */
//#define _DEBUG_DUMP_TEXTURE_REGISTER   "D:\\cxbx\\_textures\\"

#include "Version.h"

/*! version string dependent on trace flag */
#ifndef _DEBUG_TRACE
#define _CXBX_VERSION _GIT_VERSION " ("__DATE__ ")"
#else
#define _CXBX_VERSION _GIT_VERSION "-Trace ("__DATE__ ")"
#endif

/*! debug mode choices */
enum DebugMode { DM_NONE, DM_CONSOLE, DM_FILE };

/*! maximum number of threads cxbx can handle */
#define MAXIMUM_XBOX_THREADS 256

/*! runtime DbgPrintf toggle boolean */
extern volatile bool g_bPrintfOn;

#ifdef _MSC_VER
#pragma warning(disable : 4477)
#endif

/*! DbgPrintf enabled if _DEBUG_TRACE is set */
#define DbgPrintf(fmt, ...) do { if (_DEBUG_TRACE) if(g_bPrintfOn) printf(fmt, ##__VA_ARGS__); } while (0)

// From https://codecraft.co/2014/11/25/variadic-macros-tricks/
// And https://groups.google.com/d/msg/comp.std.c/d-6Mj5Lko_s/jqonQLK20HcJ

// Accept any number of args >= N, but expand to just the Nth one. In this case,
// we have settled on 10 as N. We could pick a different number by adjusting
// the count of throwaway args before N. Note that this macro is preceded by
// an underscore--it's an implementation detail, not something we expect people
// to call directly.
#define _GET_NTH_ARG( \
	_01,_02,_03,_04,_05,_06,_07,_08,_09,_10, \
	_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
	_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
	_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
	_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
	_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
	_61,_62,_63,N,...) N

#define __RSEQ_N() \
								62, 61, 60, \
	59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9,  8,  7,  6,  5,  4,  3,  2,  1,  0

// COUNT_VARARGS returns the number of arguments that have been passed to it.
// Count how many args are in a variadic macro. We now use GCC/Clang's extension to
// handle the case where ... expands to nothing. We must add a placeholder arg before
// ##__VA_ARGS__ (its value is totally irrelevant, but it's necessary to preserve
// the shifting offset we want). In addition, we must add 0 as a valid value to be in
// the N position.
#define COUNT_VARARGS(...) (_GET_NTH_ARG(_00, ##__VA_ARGS__, __RSEQ_N()))

// Define some macros to help us create overrides based on the
// arity of a for-each-style macro.
#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call(x)
#define _fe_2(_call, x, ...) _call(x) _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call(x) _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call(x) _fe_3(_call, __VA_ARGS__)
#define _fe_5(_call, x, ...) _call(x) _fe_4(_call, __VA_ARGS__)
#define _fe_6(_call, x, ...) _call(x) _fe_5(_call, __VA_ARGS__)
#define _fe_7(_call, x, ...) _call(x) _fe_6(_call, __VA_ARGS__)
#define _fe_8(_call, x, ...) _call(x) _fe_7(_call, __VA_ARGS__)
#define _fe_9(_call, x, ...) _call(x) _fe_8(_call, __VA_ARGS__)
#define _fe_10(_call, x, ...) _call(x) _fe_9(_call, __VA_ARGS__)
#define _fe_11(_call, x, ...) _call(x) _fe_10(_call, __VA_ARGS__)
#define _fe_12(_call, x, ...) _call(x) _fe_11(_call, __VA_ARGS__)

/**
 * Provide a for-each construct for variadic macros. Supports up
 * to 10 args.
 *
 * Example usage1:
 *     #define FWD_DECLARE_CLASS(cls) class cls;
 *     CALL_MACRO_X_FOR_EACH(FWD_DECLARE_CLASS, Foo, Bar)
 *
 * Example usage 2:
 *     #define START_NS(ns) namespace ns {
 *     #define END_NS(ns) }
 *     #define MY_NAMESPACES System, Net, Http
 *     CALL_MACRO_X_FOR_EACH(START_NS, MY_NAMESPACES)
 *     typedef foo int;
 *     CALL_MACRO_X_FOR_EACH(END_NS, MY_NAMESPACES)
 */
#define CALL_MACRO_X_FOR_EACH(x, ...) \
    _GET_NTH_ARG("ignored", ##__VA_ARGS__, \
    _fe_10, _fe_9, _fe_8, _fe_7, _fe_6, _fe_5, \
	_fe_4, _fe_3, _fe_2, _fe_1, _fe_0)(x, ##__VA_ARGS__)

#define DBG_ARG_WIDTH 18

#define DbgPrintHexArg(arg) printf("\n   %*s : 0x%.08X", DBG_ARG_WIDTH, #arg, arg);

 // See https://gcc.gnu.org/onlinedocs/gcc/Variadic-Macros.html
#define DbgFuncHexArgs(...) do { if (_DEBUG_TRACE) if(g_bPrintfOn) \
	printf("%s (0x%X): %s(", __FILE__, GetCurrentThreadId(), __func__); \
	if (COUNT_VARARGS(##__VA_ARGS__) > 0) { \
		CALL_MACRO_X_FOR_EACH(DbgPrintHexArg, __VA_ARGS__); \
		printf("\n"); \
	} \
	printf(");\n"); \
	} while (0)

// See https://gcc.gnu.org/onlinedocs/gcc/Variadic-Macros.html
// See http://stackoverflow.com/questions/1644868/c-define-macro-for-debug-printing
#define DbgFuncFmtArgs(fmt, ...) \
	do { if (_DEBUG_TRACE) if(g_bPrintfOn) \
	printf("%s (0x%X): %s(" fmt ");\n", __FILE__, GetCurrentThreadId(), __func__, __VA_ARGS__); \
	} while (0)

#endif
