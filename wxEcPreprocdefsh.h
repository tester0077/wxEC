/*-----------------------------------------------------------------
 * Name:        wxEcPreprocdefsh.h
 * Purpose:		  by including this file in the main project app header
 *              it is hoped that all sub-projects will be compiled with
 *              the same #defines
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_EC_PRE_PROC_H
#define _WX_EC_PRE_PROC_H
/*
 * Move project #define value to a separate header file
 * remove them from the Project Properties -> Preproc
 * and add them to separate header file for inclusion with all other
 * source files for all sub-projects
 *
 * This, hopefully, prevents the problem of different #defines because
 * some project preprocessor values are not updated consistently
 */
#define WS_LOG_FILE_NAME _("wxMsLog.txt")
#define COPYRIGHT_YEAR_1 2018;
// ------------------------------------------------------------------
#if !defined( wxUSE_UNICODE )   // defining it here seesm too late
#define wxUSE_UNICODE           // for some header
#endif
#if !defined( wxUNICODE )
#define wxUNICODE
#endif

// ------------------------------------------------------------------
// common #define s
#define WIN32
#define _WINDOWS
#define NOPCH
#define __WXMSW__
#define wxUSE_GUI 0
#define __WXMSW__
#define _CONSOLE

#define WANT_FILEVERSION
#define WANT_EXE_LINK_DATE_NO


// ------------------------------------------------------------------
// version specific #defines
#if defined (_DEBUG )

#else

#endif  // _DEBUG

#endif  // _WX_EC_PRE_PROC_H
// ------------------------------- eof ---------------------------

