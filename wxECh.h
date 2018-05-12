/*-----------------------------------------------------------------
 * Name:        wxECh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *-------------------------------------------------------------- */
#ifndef _WX_EC_H
#define _WX_EC_H

#include "wx/cmdline.h"
#include "wx/filename.h"
#include "wx/textfile.h"

// ------------------------------------------------------------------

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
  { wxCMD_LINE_SWITCH, "h", "help", "show this help message",
    wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },

  { wxCMD_LINE_SWITCH, "?", "help", "show this help message",
    wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },

  { wxCMD_LINE_SWITCH, "d", "details",  "Show a more detailed usage message",
    wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL }, 

  { wxCMD_LINE_OPTION, "i", "input",   "<input path>", 
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

  { wxCMD_LINE_SWITCH, "m", "mac", "force MAC line endings" },

  { wxCMD_LINE_OPTION, "o", "output",  "<output path>", 
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

  { wxCMD_LINE_SWITCH, "p", "pause",  
    "pause at end of compile to allow user to inspect the output" },

  { wxCMD_LINE_SWITCH, "t", "timestamp",  
    "skip compile if output files are older than input", 
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

  { wxCMD_LINE_SWITCH, "u", "unix", "force Unix line endings" },

  { wxCMD_LINE_SWITCH, "v", "verbose", "be verbose" },

  { wxCMD_LINE_SWITCH, "w", "windows", "force DOS/Windows line endings" },

  { wxCMD_LINE_SWITCH, "x", "wxwidgets", "add code for wxWidgets" },

  { wxCMD_LINE_PARAM,  NULL, NULL, "input file(s)",
      wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY | wxCMD_LINE_PARAM_MULTIPLE },

  { wxCMD_LINE_NONE }
};

// ------------------------------------------------------------------
class enumData
{
public:
  wxString wsName;
  bool     bHasValue;
  int      iValue;
  wxString wsString;
};
WX_DECLARE_OBJARRAY( enumData, arrayEnumData );
// ------------------------------------------------------------------

class enumArray
{
public:
  wxString        m_wsLabel;
  arrayEnumData   m_Data;
};
WX_DECLARE_OBJARRAY( enumArray, arrayEnums );

// ------------------------------------------------------------------

bool outputHeaderFile( wxFileName wfnOutputHdrFile,
                       wxString wsInputFileName );
bool outputCodeFile( wxFileName wfnOutputCppFile,
                     wxString wsInputFileName);
void outputHeaderLines( wxString wsInputFileName,
                       wxString& wsLine );

#endif  // _WX_EC_H
// ------------------------------- eof ---------------------------
