/*-----------------------------------------------------------------
 * Name:        wxEcOutputCppFile.cpp
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
#include "wxEcCppData.h"
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
extern arrayEnums    arEnums;
extern bool          g_bVerbose;
extern bool          g_bWxWidgets;
extern wxTextFileType g_eolType;
// ------------------------------------------------------------------

bool outputCodeFile( wxFileName wfnOutputCppFile,
                     wxString wsInputFileName )
{
  wxTextFile wtfCppFile;
  wxString   wsLine;

  if ( g_bVerbose )
  {
    wxPrintf( _T("Writing to: %s\n"),wfnOutputCppFile.GetFullPath() );
  }
  // open the file
  if( !wtfCppFile.Open( wfnOutputCppFile.GetFullPath() ) )
  {
    // some problem?
    wxPrintf( _T("Problems opening %s!\nQuitting"),
      wfnOutputCppFile.GetFullPath() );
    return false;
  }
  // make sure it is empty
  wtfCppFile.Clear();

  outputHeaderLines( wsInputFileName, wsLine );
  wtfCppFile.AddLine( wsLine, g_eolType );

  wsLine.Printf( _T("#include \"%s.h\"\r\n#include <string.h>"), 
    wfnOutputCppFile.GetName() );
  wtfCppFile.AddLine( wsLine, g_eolType );
  // output one class per enum 
  wxString wsT;
  
  for( unsigned int i = 0; i < arEnums.GetCount(); i ++ )
  {
    wxString wsLabel = arEnums[i].m_wsLabel;

    // output top part
    for ( int k = 0; k <  sizeof(ecCpp1)/sizeof( ecCppData ); k++ )
    {
      if( (ecCpp1[k].iFlags & BASIC) || 
        ( g_bWxWidgets && (ecCpp1[k].iFlags & WXWIDGETS) ))
      {
        if( ecCpp1[k].wsFormat.IsEmpty() )
        {
          wtfCppFile.AddLine( ecCpp1[k].wsData, g_eolType );
        }
        else if( ecCpp1[k].wsFormat.Matches( _T("s") ) )
        {
          wsLine.Printf( ecCpp1[k].wsData, arEnums[i].m_wsLabel );
          wtfCppFile.AddLine( wsLine, g_eolType );
        }
        else if( ecCpp1[k].wsFormat.Matches( _T("ss") ) )
        {
          wsLine.Printf( ecCpp1[k].wsData, arEnums[i].m_wsLabel, 
            arEnums[i].m_wsLabel );
          wtfCppFile.AddLine( wsLine, g_eolType );
        }
        else if( ecCpp1[k].wsFormat.Matches( _T("sss") ) )
        {
          wsLine.Printf( ecCpp1[k].wsData, arEnums[i].m_wsLabel, 
            arEnums[i].m_wsLabel, arEnums[i].m_wsLabel );
          wtfCppFile.AddLine( wsLine, g_eolType );
        }
        else if( ecCpp1[k].wsFormat.Matches( _T("sd") ) )
        {
          wsLine.Printf( ecCpp1[k].wsData, arEnums[i].m_wsLabel, 
            arEnums[i].m_Data.GetCount() );
          wtfCppFile.AddLine( wsLine, g_eolType );
        }
        else if( ecCpp1[k].wsFormat.Matches( _T("u") ) )
        {
          wsLine.Printf( ecCpp1[k].wsData, arEnums[i].m_Data[0].wsName );
          wtfCppFile.AddLine( wsLine, g_eolType );
        }
        else // need to catch undefined strings
        {
          wxBell();
          wxPrintf( _T("!!! Unhandled CPP string type: %s !!!!!\n" ), ecCpp1[k].wsFormat );
        }
      }
    }
    // now output the enum Map initializers
    for ( unsigned int j = 0; j < arEnums[i].m_Data.GetCount(); j++ )
    {
#if 1
      wsLine.Printf( _T("\t%s::m_Map[%d] = Map_t( %s, \"%s\", _T(\"%s\") );"),
        arEnums[i].m_wsLabel, j, arEnums[i].m_Data[j].wsName, 
        arEnums[i].m_Data[j].wsString.IsEmpty() ?
          arEnums[i].m_Data[j].wsName : arEnums[i].m_Data[j].wsString,
        arEnums[i].m_Data[j].wsString.IsEmpty() ?
          arEnums[i].m_Data[j].wsName : arEnums[i].m_Data[j].wsString );
#else
      wsLine.Printf( _T("\t{%s, \"%s\", _T(\"%s\")}%c"),
        arEnums[i].m_Data[j].wsName, 
        arEnums[i].m_Data[j].wsString.IsEmpty() ?
          arEnums[i].m_Data[j].wsName : arEnums[i].m_Data[j].wsString,
        arEnums[i].m_Data[j].wsString.IsEmpty() ?
          arEnums[i].m_Data[j].wsName : arEnums[i].m_Data[j].wsString,
        (j == (arEnums[i].m_Data.GetCount() - 1)) ? ' ' : ',' );
#endif
      wtfCppFile.AddLine( wsLine, g_eolType ); 
    }
    // output bottom part
    for ( int k = 0; k <  sizeof(ecCpp2)/sizeof( ecCppData ); k++ )
    {
      if( (ecCpp2[k].iFlags & BASIC) || 
        ( g_bWxWidgets && (ecCpp2[k].iFlags & WXWIDGETS) ))
      {
        if( ecCpp2[k].wsFormat.IsEmpty() )
        {
          wtfCppFile.AddLine( ecCpp2[k].wsData, g_eolType );
        }
        else if( ecCpp2[k].wsFormat.Matches( _T("s") ) )
        {
          wsLine.Printf( ecCpp2[k].wsData, arEnums[i].m_wsLabel );
          wtfCppFile.AddLine( wsLine, g_eolType );
        }
        else if( ecCpp2[k].wsFormat.Matches( _T("u") ) )
        {
          wsLine.Printf( ecCpp2[k].wsData, arEnums[i].m_Data[0].wsName );
          wtfCppFile.AddLine( wsLine, g_eolType );
        }
        else if( ecCpp2[k].wsFormat.Matches( _T("ss") ) )
        {
          wsLine.Printf( ecCpp2[k].wsData, arEnums[i].m_wsLabel, 
            arEnums[i].m_wsLabel );
          wtfCppFile.AddLine( wsLine, g_eolType );
        }
        else // need to catch undefined strings
        {
          wxPrintf( _T("!!! Unhandled CPP string type: %s !!!!!\n" ), ecCpp2[k].wsFormat );
          wxBell();
        }
      }
    }
  }
  // output trailer - \r\n is added automatically
  wsLine.Printf( 
  _T("// ------------------------------- eof ------------------------------") );
  wtfCppFile.AddLine( wsLine, g_eolType );

  // MUST write the data
  wtfCppFile.Write();
  // close the header file
  wtfCppFile.Close();
  return true;    // all went well
}
// ------------------------------- eof ------------------------------
