//
// Created by Joran on 22/08/2023.
//

#pragma once

// https://www.flipcode.com/archives/FIXME_TODO_Notes_As_Warnings_In_Compiler_Output.shtml
//
// 	FIXME & TODO Notes As Warnings In Compiler Output
//  Submitted by Alberto Garc�­a-Baquero Vega
//
//---------------------------------------------------------------------------------------------
// FIXMEs / TODOs / NOTE macros
//---------------------------------------------------------------------------------------------
#define _QUOTE( x ) #x
#define QUOTE( x ) _QUOTE( x )
#define __FILE__LINE__ __FILE__ "(" QUOTE( __LINE__ ) ") : "

#define NOTE( x ) message( x )
#define FILE_LINE message( __FILE__LINE__ )

#define TODO( x )                                                                                                                                                                                                                                              \
    message( __FILE__LINE__ "\n"                                                                                                                                                                                                                               \
                            " ------------------------------------------------\n"                                                                                                                                                                              \
                            "|  TODO :   " #x "\n"                                                                                                                                                                                                             \
                            " -------------------------------------------------\n" )

#define FIXME( x )                                                                                                                                                                                                                                             \
    message( __FILE__LINE__ "\n"                                                                                                                                                                                                                               \
                            " ------------------------------------------------\n"                                                                                                                                                                              \
                            "|  FIXME :  " #x "\n"                                                                                                                                                                                                             \
                            " -------------------------------------------------\n" )

#define todo( x ) message( __FILE__LINE__ "Todo: " #x "\n" )
#define fixme( x ) message( __FILE__LINE__ "Fix Me: " #x "\n" )
