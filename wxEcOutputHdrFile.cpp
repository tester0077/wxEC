/*-----------------------------------------------------------------
 * Name:        wxEcOutputHdrFile.cpp
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *---------------------------------------------------------------- */

/*----------------------------------------------------------------
 * Standard wxWidgets headers
 *---------------------------------------------------------------- */
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
#  if defined ( _DEBUG )
     // this statement NEEDS to go BEFORE all headers
#    define _CRTDBG_MAP_ALLOC
#    include <stdlib.h>
#    include <crtdbg.h>
#  endif
#endif
#include "wxEcPreProcDefsh.h"   // MUST be first
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

/* For all others, include the necessary headers (this file is
* usually all you
* need because it includes almost all "standard" wxWidgets headers) */
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wxECh.h"
#include "wxEcHdrData.h"
// ------------------------------------------------------------------
#if defined( _MSC_VER )
// only good for MSVC - see note above re __VISUALC__
// this block needs to AFTER all headers
# include <stdlib.h>
# include <crtdbg.h>
# ifdef _DEBUG
#   ifndef DBG_NEW
#     define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#     define new DBG_NEW
#   endif
# endif
#endif
// --------------------------- externals ----------------------------
extern arrayEnums     arEnums;
extern wxArrayString  asIncludes;
extern bool           g_bVerbose;
extern bool           g_bWxWidgets;
extern wxTextFileType g_eolType;

// ------------------------------------------------------------------

bool outputHeaderFile( wxFileName wfnOutputHdrFile,
                       wxString wsInputFileName )
{
  wxTextFile wtfHdrFile;
  wxString   wsLine;

  if ( g_bVerbose )
  {
    wxPrintf( _T("Writing to: %s\n"),wfnOutputHdrFile.GetFullPath() );
  }
  // open the file
  if( !wtfHdrFile.Open( wfnOutputHdrFile.GetFullPath() ) )
  {
    // some problem?
    wxPrintf( _T("Problems opening %s!\nQuitting"),
      wfnOutputHdrFile.GetFullPath() );
    return false;
  }
  // make sure it is empty
  wtfHdrFile.Clear();
  // output header
  outputHeaderLines( wsInputFileName, wsLine );
  wtfHdrFile.AddLine( wsLine, g_eolType );
  // output any header lines
  for ( size_t i = 0; i < asIncludes.GetCount(); i++ )
    wtfHdrFile.AddLine( asIncludes[i], g_eolType );
  // now output the data
  wsLine.Printf( _T("#ifndef _%s_H\r\n#define _%s_H\r\n"), 
    wfnOutputHdrFile.GetName().MakeUpper(), 
    wfnOutputHdrFile.GetName().MakeUpper() );
  wtfHdrFile.AddLine( wsLine, g_eolType );

  // output one class per enum 
  wxString wsT;
  for( unsigned int i = 0; i < arEnums.GetCount(); i++ )
  {
    wsT = wxEmptyString;

    // output the top part
    for ( int k = 0; k <  sizeof(ecHdr1)/sizeof( ecHdrData ); k++ )
    {
      if( (ecHdr1[k].iFlags & BASIC) || 
        ( g_bWxWidgets && (ecHdr1[k].iFlags & WXWIDGETS) ))
      {
        if( ecHdr1[k].wsFormat.IsEmpty() )
        {
          wtfHdrFile.AddLine( ecHdr1[k].wsData, g_eolType );
        }
        else if( ecHdr1[k].wsFormat.Matches( _T("s") ) )
        {
          wsLine.Printf( ecHdr1[k].wsData, arEnums[i].m_wsLabel );
          wtfHdrFile.AddLine( wsLine + wsT, g_eolType );
        }
        else if( ecHdr1[k].wsFormat.Matches( _T("sd") ) )
        {
          wsLine.Printf( ecHdr1[k].wsData, arEnums[i].m_wsLabel, 
            arEnums[i].m_Data.GetCount() );
          wtfHdrFile.AddLine( wsLine + wsT, g_eolType );
        }
        else if( ecHdr1[k].wsFormat.Matches( _T("st") ) )
        {
          for ( unsigned int jj = 0; jj < arEnums[i].m_Data.GetCount(); jj++ )
          {
            wsT += arEnums[i].m_Data[jj].wsName + _T(",");
          }
          wsLine.Printf( ecHdr1[k].wsData, arEnums[i].m_wsLabel, wsT );
          wtfHdrFile.AddLine( wsLine, g_eolType );
        }
        else // need to catch undefined strings
        {
          wxBell();
          wxPrintf( _T("!!! Unhandled Header string type: %s !!!!!\n" ), ecHdr1[k].wsFormat );
        }
      }
    }
    // now out put the actual enum structure
    for ( unsigned int j = 0; j < arEnums[i].m_Data.GetCount(); j++ )
    {
      if( arEnums[i].m_Data[j].bHasValue )
      {
        if( arEnums[i].m_Data[j].wsString.IsEmpty() )
        {
          wsLine.Printf( _T("\t\t%s = %3d,"),
          arEnums[i].m_Data[j].wsName, arEnums[i].m_Data[j].iValue );
        }
        else
        {
          wsLine.Printf( _T("\t\t%s = %3d, // \"%s\""),
          arEnums[i].m_Data[j].wsName, arEnums[i].m_Data[j].iValue, 
          arEnums[i].m_Data[j].wsString );
        }
      }
      else
      {
        if( arEnums[i].m_Data[j].wsString.IsEmpty() )
        {
          wsLine.Printf( _T("\t\t%s,"),
          arEnums[i].m_Data[j].wsName );
        }
        else
        {
          wsLine.Printf( _T("\t\t%s, // \"%s\""),
          arEnums[i].m_Data[j].wsName, arEnums[i].m_Data[j].wsString );
        }
      }
      wtfHdrFile.AddLine( wsLine, g_eolType );
    }
    // output the bottom part
    for ( int k = 0; k <  sizeof(ecHdr2)/sizeof( ecHdrData ); k++ )
    {
      if( (ecHdr2[k].iFlags & BASIC) || 
        ( g_bWxWidgets && (ecHdr2[k].iFlags & WXWIDGETS) ))
      {
        if( ecHdr2[k].wsFormat.IsEmpty() )
        {
          wtfHdrFile.AddLine( ecHdr2[k].wsData, g_eolType );
        }
        else if( ecHdr2[k].wsFormat.Matches( _T("s") ) )
        {
          wsLine.Printf( ecHdr2[k].wsData, arEnums[i].m_wsLabel );
          wtfHdrFile.AddLine( wsLine, g_eolType );
        }
        else // need to catch undefined strings
        {
          wxBell();
          wxPrintf( _T("!!! Unhandled Header string type: %s !!!!\n" ), ecHdr2[k].wsFormat );
        }
      }
    }
  }
  // output trailer - \r\n is added automatically
  wsLine.Printf( _T("#endif // _%s_H"),  
    wfnOutputHdrFile.GetName().MakeUpper() );
  wtfHdrFile.AddLine( wsLine, g_eolType );
  wtfHdrFile.AddLine( 
    _T("// ------------------------------- eof ------------------------------"), 
    g_eolType );

  // MUST write the data
  wtfHdrFile.Write();
  // close the header file
  wtfHdrFile.Close();
  return true; // all went well
}
// ------------------------------- eof ------------------------------
