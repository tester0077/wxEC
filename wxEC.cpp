/*-----------------------------------------------------------------
 * Name:        wxEC loosely based on console.cpp
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
// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/defs.h"

#include <stdio.h>
#include <conio.h>

#include "wx/string.h"
#include "wx/file.h"
#include "wx/app.h"
#include "wx/datetime.h"

#include "wx/tokenzr.h"

// without this pragma, the stupid compiler precompiles #defines below so that
// changing them doesn't "take place" later!
#ifdef __VISUALC__
    #pragma hdrstop
#endif
#include "wxECh.h"
#if defined( WANT_FILEVERSION )
#include "fileVerh.h"
#endif
#if defined( WANT_EXE_LINK_DATE )
// exeHdr code causes too many dependencies at present - omit for now
#include "exeDosHdrh.h"
#endif
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

// ============================================================================
// declarations
// ============================================================================

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(arrayEnumData);
WX_DEFINE_OBJARRAY(arrayEnums);
arrayEnums    arEnums;
wxArrayString asIncludes;
bool          g_bVerbose;
bool          g_bPause;
bool          g_bCheckTimeStamps;
bool          g_bWxWidgets;

// version info - to be displayed in 'About' dialog
// should only be defined in the file where it is used
// to avoid extraneous recompiles.
// If the values need to be used in more than one source file,
// the data should be passed aroung using variables
wxString gwsCopyrightYear;
#if defined( WANT_FILEVERSION )
int giMajorVersion = 0;
int giMinorVersion = 0;
int giBuildNumber = 0;
#else
#include "version.info"
int giMajorVersion = MAJORVERSION;
int giMinorVersion = MINORVERSION;
int giBuildNumber = BUILDNUMBER;
#endif
// ------------------------------------------------------------------
wxString gwsLogo; // This shows as part of the usage message!

wxTextFileType g_eolType = wxTextFileType_Dos;
// ============================================================================
// implementation
// ============================================================================
void cleanUp( int argc, wxChar **wxArgv )
{
    // clean up the command line args for Unicode
#if wxUSE_UNICODE
    {
        for ( int n = 0; n < argc; n++ )
            free(wxArgv[n]);
        delete [] wxArgv;
    }
#endif // wxUSE_UNICODE
}

// ------------------------------------------------------------------
void setupLogo( void )
{
  wxString ws1;
  ws1.Printf(
    _T("%s - an Enum Compiler\n")
    _T(" A. Wiegert (c)2011 - %s\n")
    _T(" Version %d.%d - Build: %d\n")
    _T(" linked with wxWidgets %d.%d.%d\n\n")
    _T(" Inspired by LLoyd Williamsen's EC\n")
    _T(" See: http://www.codeguru.com/cpp/cpp/cpp_mfc/article.php/c4001\n")
    _T("Purpose: Generate intelligent C++ enumeration classes from standard enum\n")
    _T(" declarations. This utility scans through files containing standard C++ \n")
    _T(" source code (file which are typically given the suffix .e) for enums\n")
    _T(" and outputs .cpp and .h files containing a C++ class per enum.\n\n")

    _T(" These classes extend the standard C++ enum concept by:\n")
    _T("  - allowing the enum to be read and assigned to as a string.\n")
    _T("  - ensuring only declared values are used by automatically selecting\n")
    _T("    a default/unknown value if an assigned value is not found.\n")
    _T("  - initialising the enum to a default/unknown value on creation.\n\n")

    _T(" Run with the -d switch for more details.\n\n")
    _T(" Note: Line endings are auto-adjusted to the detected OS.\n\n")

    _T(" This utility is free.\n") ,
      wxTheApp->GetAppName(), gwsCopyrightYear,
      giMajorVersion, giMinorVersion, giBuildNumber,
      wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER
      );
  gwsLogo = ws1;
}

// ------------------------------------------------------------------
wxString wsDetails =
    _T("Details: A standard enum declaration can be used for input. The declaration\n")
    _T("\t will be used without modification. The class will be given the same\n")
    _T("\t name as that of the enum. The first enum token will be used as the\n")
    _T("\t default/unknown value in the class. The comment text found just after\n")
    _T("\t each enum token is used as the string value for that token. If there\n")
    _T("\t is no comment then the token itself will be used as the text value.\n")
    _T("\tThe following enum:\n")
    _T("\t   enum EState {\n")
    _T("\t     Unknown = 0,\n")
    _T("\t     Solid = 1,  // Solid Matter\n")
    _T("\t     Liquid = 2, // \"Liquid Matter \"\n")
    _T("\t     Gas,        /* Vapour */\n")
    _T("\t   };\n\n")

    _T("\tWill become:\n")
    _T("\t     class EState {\n")
    _T("\t     public:\n")
    _T("\t       // EState::Unknown,Solid,Liquid,Gas,\n")
    _T("\t       enum Enum {\n")
    _T("\t         Unknown = 0,  // \"Unknown\"\n")
    _T("\t         Solid   = 1,  // \"Solid Matter\"\n")
    _T("\t         Liquid  = 2,  // \"Liquid Matter \"\n")
    _T("\t         Gas,          // \"Vapour\"\n")
    _T("\t       };\n")
    _T("\t      ...\n")
    _T("\t     };\n\n")
    _T("\tNow this new class can be used in the folowing ways:\n")
    _T("\t     EState ST;\n")
    _T("\t     ST = EState::Gas;      printf(\"%%d - %%s\\n\", ST, (const char*)ST);\n")
    _T("\t                            // 3 - Vapour\n")
    _T("\t     ST = \"Solid Matter\";   printf(\"%%d - %%s\\n\", ST, (const char*)ST);\n")
    _T("\t                            // 1 - Solid Matter\n")
    _T("\t     ST = 5;                printf(\"%%d - %%s\\n\", ST, (const char*)ST);\n")
    _T("\t                            // 0 - Unknown\n")
    _T("\t     ST = \"Junk\";           printf(\"%%d - %%s\\n\", ST, (const char*)ST);\n")
    _T("\t                            // 0 - Unknown");
// ------------------------------------------------------------------
/**
 * Returns the desired header line in wsLine.
 */
void outputHeaderLines( wxString wsInputFileName,
                       wxString& wsLine )
{
  wxString wsNow  = ::wxNow();
  wxDateTime now = wxDateTime::Now();
  
  wsLine.Printf( _T("// Auto-generated by wxEC %d.%d.%d - %s\r\n")
    _T("// Do not edit this file - it will be overwritten on next pass\r\n")
    _T("// Make any necessary changes to the source file:\r\n")
    _T("// %s\r\n"), giMajorVersion, giMinorVersion, giBuildNumber,
    now.Format( _T("%c"), wxDateTime::PST), wsInputFileName );
}

// ------------------------------------------------------------------
bool compileOneSet( wxString wsInputFilename, 
                   wxString wsInputPath,
                   wxString wsOutputPath )
{
  wxString wsOutputHdrFilename;
  wxString wsOutputCppFilename;
  wxFileName wfnInputFileName( wsInputFilename );
  wxFileName wfnOutputHdrFile;
  wxFileName wfnOutputCppFile;
  wxDateTime wdtInputFile;
  wxDateTime wdtOutputHdrFile;
  wxDateTime wdtOutputCppFile;

  wxTextFile wtfInputFile;
  wxTextFile wtfHdrFile;
  wxTextFile wtfCppFile;

  // first make sure the input path is terminated with a slash
  // set the default to current directory
  if( wsInputPath.IsEmpty() )
    wsInputPath = wfnInputFileName.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR  );
  if( (wsInputPath.Len() > 0 ) && 
    (wsInputPath[wsInputPath.Len() - 1] != '\\' ) && 
    (wsInputPath[wsInputPath.Len() - 1] != '/' ) )
    wsInputPath += _T("\\");
  wxFileName wfnTemp( wsInputFilename );
  wxFileName wfnInPath( wsInputPath );
  wfnInputFileName.Assign( wsInputPath );
  wfnInputFileName.SetFullName( wfnTemp.GetFullName() );
  wfnInputFileName.MakeAbsolute();
  // complain if input file does not exist on the given path
  if( !wfnInputFileName.FileExists() )
  {
    wxPrintf( _T("File %s does not exist.\nQuitting!"), 
      wfnInputFileName.GetFullPath() );
    return false;
  }
  // make up output files & paths
  // set the default to current directory
  if( wsOutputPath.IsEmpty() )
    wsOutputPath = wfnInputFileName.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR  );
  // first make sure path given is terminated with a slash
  if ( (wsOutputPath.Len() > 0 ) && 
    (wsOutputPath[wsOutputPath.Len() - 1] != '\\' ) && 
    (wsOutputPath[wsOutputPath.Len() - 1] != '/' ) )
    wsOutputPath += _T("\\");
  
  // header file
  wfnOutputHdrFile.Assign( wsOutputPath );
  wfnOutputHdrFile.SetName( wfnTemp.GetName() );
  wfnOutputHdrFile.SetExt( _T("h") );

  // CPP file
  wfnOutputCppFile.Assign( wsOutputPath );
  wfnOutputCppFile.SetName( wfnTemp.GetName() );
  wfnOutputCppFile.SetExt( _T("cpp") );
  // see if we want to compare and act on file time stamps
  if ( g_bCheckTimeStamps )
  {
    wdtInputFile = wfnInputFileName.GetModificationTime();
    if( wfnOutputHdrFile.FileExists() )
      wdtOutputHdrFile = wfnOutputHdrFile.GetModificationTime();
    else
      wdtOutputHdrFile = 0;
    if( wfnOutputCppFile.FileExists() )
      wdtOutputCppFile = wfnOutputCppFile.GetModificationTime();
    else
      wdtOutputCppFile = 0;
    if( wdtInputFile.IsEarlierThan( wdtOutputHdrFile ) ||
      wdtInputFile.IsEarlierThan( wdtOutputHdrFile ) )
    {
      if( g_bVerbose )
      {
        wxPrintf( 
          _T("Input file %s is older than output file(s)\n")
          _T("Compile not needed!\n" ), wfnInputFileName.GetFullName() );
      }
      //cleanUp( argc, wxArgv );
      if ( g_bPause )
      {
        wxPrintf( _T("\n\nPress any key to continue .... ") );
        _getch();
      }
      return true; // handle next file
    }
  }

  // make sure the output directory exists
  if( ! wfnOutputHdrFile.DirExists() )
  {
    wxPrintf( _T("%s does not exist, please create it first!\nQuitting"),
      wfnOutputHdrFile.GetFullPath() );
    //cleanUp( argc, wxArgv );
    return false;
  }
  // open the input file as 'text' files
  if( !wtfInputFile.Open( wfnInputFileName.GetFullPath() ) )
  {
    wxPrintf( _T("Problems opening %s!\nQuitting"),
      wfnInputFileName.GetFullPath() );
    //cleanUp( argc, wxArgv );
    return false;
  }
  // create the output files
  if( wtfHdrFile.Open( wfnOutputHdrFile.GetFullPath() ) )
  {
    wtfHdrFile.Clear();
    wtfHdrFile.Close(); // close until we're ready to write to file
  }
  else
  {
    if ( !wtfHdrFile.Create() )
    {
      wxPrintf( _T("Can't create %s!\nQuitting"),
        wfnOutputHdrFile.GetFullPath() );
      //cleanUp( argc, wxArgv );
      return false;
    }
  }
  if( wtfCppFile.Open( wfnOutputCppFile.GetFullPath() ) )
  {
    wtfCppFile.Clear();
  }
  else
  {
    if ( !wtfCppFile.Create() )
    {
      wxPrintf( _T("Can't create %s!\nQuitting"),
        wfnOutputCppFile.GetFullPath() );
      //cleanUp( argc, wxArgv );
      return false;
    }
  }
  // now parse the input file
  wxString wsLine;
  wxString wsRest;
  wxString wsLabel;
  wxString wsStart;
  enumArray eAr;
  
  int   iEnumDefaultVal = 0;
  int   iEnums = 0;
  bool  bFoundLeftBrace = false;
  bool  bFoundRightBrace = false;
  bool  bInComment = false;
  bool  bFoundEnum = false;
  size_t nPos;

  // clear the data & Includes array
  arEnums.Clear();
  asIncludes.Clear();
  for ( wsLine = wtfInputFile.GetFirstLine();
    !wtfInputFile.Eof(); wsLine = wtfInputFile.GetNextLine() )
  {
    if( wsLine.IsEmpty() )
      continue;
    wsLine.Trim( false ); // trim leading white space
    wsLine.Trim( true );  // trim trailing white space
    if( wsLine.IsEmpty() )
      continue;
    // check for #include lines - these are to be passed though
    nPos = wsLine.find( _T("#include"), 0 ); 
    if ( nPos == 0 )
    {
      asIncludes.Add( wsLine );
      continue;
    }
    // strip off any C++ comments - outside of braces
    if( ! bFoundLeftBrace )
    {
      int iPos;
      if( ( iPos = wsLine.Find( _T("//") ) ) != wxNOT_FOUND )
      {
        wsLine = wsLine.BeforeFirst( '/' );
        wsLine.Trim(); // trim trailing white space of what is left
        if( wsLine.IsEmpty() )
          continue;
      }
      if( ( iPos = wsLine.Find( _T("/*") ) ) != wxNOT_FOUND )
      {
        wxString wsTail;
        int iEnd;
        bInComment = true;
        wsStart = wsLine.BeforeFirst( '/' );
        wsStart.Trim(); // trim trailing white space of what is left
        wsLine = wsLine.AfterFirst( '*' );
        // see if this regular comment does not span more than this line
        if( (iEnd = wsLine.Find( _T("*/") ) ) != wxNOT_FOUND )
        {
          wsLine = wsLine.AfterFirst( '/' );
          wsLine.Trim( false );   // trim leading blanks
          if( wsLine.IsEmpty() )
            continue;
        }
        else  // this comment must be spilling over to at least the next line
        {
          // else need to find the end of the comment and splice the lines
          while ( !wtfInputFile.Eof() )
          {
            wsLine = wtfInputFile.GetNextLine();
            if( ( iPos = wsLine.Find( _T("*/") ) ) != wxNOT_FOUND )
            {
              bInComment = false;
              wsRest = wsLine.AfterFirst( '/' );
              wsLine = wsStart + wsRest;
              break;//continue;
            }
            // else get remainder of line
            if( wsRest.IsEmpty() )
              continue;   // 
            wsRest = wsLine.AfterFirst( '/' );
            wsLine = wsStart + wsRest;
            break;
          }
        }
      }
    }
    if( wsLine.IsEmpty() )
      continue;
    // at this point we need to strip out comment-only lines
    // presumes lines have been trimmed front and back
    if ( wsLine.StartsWith( _T("//" ) ) )
      continue;
    if ( wsLine.StartsWith( _T("/*") ) && wsLine.EndsWith( _T("*/") ) )
      continue;
    wxString  wsTok;

    enumData  eD;
    eD.iValue = iEnumDefaultVal;
    eD.wsString = eD.wsName = wxEmptyString;
    eD.bHasValue = false;
    wxStringTokenizer lineToken(wsLine, _T(" \t"));
    while ( lineToken.HasMoreTokens() )
    {
        wsTok = lineToken.GetNextToken();
        if( wsTok.IsSameAs( _T("};") ) )
        {
          bFoundRightBrace = true;
          bFoundLeftBrace = false;
          bFoundEnum = false;
          arEnums.Add( eAr );
          eAr.m_Data.Clear();
          eAr.m_wsLabel = wxEmptyString;
          iEnums++;
          break;
        }
        if( wsTok.IsSameAs( _T("{") ) )
        {
          bFoundRightBrace = false;
          bFoundLeftBrace = true;
          bFoundEnum = false;
          continue;
        }
        if( wsTok.IsSameAs( _T("enum") ) )
        {
          bFoundEnum = true;
          continue;
        }
        if( bFoundEnum && !bFoundLeftBrace )
        {
          eAr.m_wsLabel = wsTok;
          continue;
        }
        // if we are parsing one entry in the enum structure - 
        // all entries are restricted to a single line, with the possibility 
        // of additional pure comment lines
        if( bFoundLeftBrace && ! bFoundRightBrace )
        {
          wxString wsEl;
          long     lVal;
          int n = 0;
          wxStringTokenizer elementToken(wsLine, _T("\t ,"));

          while ( elementToken.HasMoreTokens() )
          {
            wsEl = elementToken.GetNextToken();
            if ( wsEl.IsEmpty() )
              continue;
            
            if( n == 0 )
            {
              eD.wsName = wsEl;   // first element is _always_ the name
              // set the default value now;
              // it might be overridden by the actual assigment later
              eD.iValue = iEnumDefaultVal++;
              n++;
              continue;
            }
            else
            {
              if( wsEl.IsEmpty() )
                continue;
              if( wsEl.IsSameAs( _T("=") ) )
              {
                // get the value
                wsEl = elementToken.GetNextToken();
                wsEl.ToLong( &lVal );
                eD.iValue = (int)lVal;
                iEnumDefaultVal = eD.iValue + 1;
                eD.bHasValue = true;
                n++;
                continue; // get next token
              }
              // for some reason, IsSameAs does not work all the time 
              // as expected. When parsing a file, it seems to work at
              // first, but then fails to match after some time or
              // after some input???
              // Problem resolved: caused by not handling tabs as white 
              // space in the  tokenizers
              else if ( wsEl.IsSameAs( _T("//") ) )
              //else if ( wsEl.CmpNoCase( _T("//") ) == 0 )
              {
                wsEl = elementToken.GetString();
                // will need to strip quotes if it has any
                if( wsEl[0] == '"' )
                {
                  wsEl = wsEl.Mid( 1 );
                  // should we really strip leading and
                  // trailing spaces? Original EC does NOT
                  //wsEl.Trim( false ); // trim leading blank
                  wsEl = wsEl.BeforeLast( '"' );
                  //wsEl.Trim(); // trim trailing blank
                }
                eD.wsString = wsEl;
                break;
              }
              //else if ( wsEl.IsSameAs( _T("/*") ) )
              else if ( wsEl.CmpNoCase( _T("/*") ) == 0 )
              {
                size_t pos;
                wsEl = elementToken.GetString();
                pos = wsEl.find( _T("*/") );
                wsEl = wsEl.Left( pos );
                wsEl.Trim( true ); //trim trailing spaces
                eD.wsString = wsEl;
                break;
              }
            }
          }
          eAr.m_Data.Add( eD );
          break;
        }
    }
  }

  // output the cleaned and parsed enum data for inspection
  if( g_bVerbose )
  {
    // print out arrays for checking
    for( unsigned int i = 0; i < arEnums.GetCount(); i ++ )
    {
      wxPrintf( _T("Label: %s\n"), arEnums[i].m_wsLabel );
      for ( unsigned int j = 0; j < arEnums[i].m_Data.GetCount(); j++ )
      {
        wxPrintf( _T("  Name: %s, Value: %d, String: %s\n"),
          arEnums[i].m_Data[j].wsName, arEnums[i].m_Data[j].iValue, 
          arEnums[i].m_Data[j].wsString );
      }
      wxPrintf( _T("\n" ) );
    }
  }
  // now we are ready to write the output files
  outputHeaderFile( wfnOutputHdrFile,
    wfnInputFileName.GetFullPath() );
  outputCodeFile( wfnOutputCppFile,
    wfnInputFileName.GetFullPath() );
  if ( g_bPause )
  {
    wxPrintf( _T("\n\nPress any key to continue .... ") );
    _getch();
  }
  return true;
}

// ----------------------------------------------------------------------------
// entry point
// ----------------------------------------------------------------------------

int main(int argc, char **argv)
{
  wxString wsInputPath;
  wxString wsOutputPath;
  wxString wsInputFilename;
  wxString wsOutputHdrFilename;
  wxString wsOutputCppFilename;
  wxFileName wfnInputFileName;
  wxFileName wfnOutputHdrFile;
  wxFileName wfnOutputCppFile;
  wxDateTime wdtInputFile;
  wxDateTime wdtOutputHdrFile;
  wxDateTime wdtOutputCppFile;

  wxTextFile wtfInputFile;
  wxTextFile wtfHdrFile;
  wxTextFile wtfCppFile;

  g_bVerbose = false;
  g_bWxWidgets = false;
  g_bCheckTimeStamps = false;
  g_bPause = false;

  wsInputPath = wsOutputPath = wsInputFilename = 
    wsOutputHdrFilename = wsOutputCppFilename = wxEmptyString;
#if defined( WANT_FILEVERSION )
  wxFileVersion fVer;
  // open the executable's version info
  fVer.Open();
  wxString wsFV  = fVer.GetFixedFileVersion(); 
  giMajorVersion = fVer.GetMajorVersion();
  giMinorVersion = fVer.GetMinorVersion();
  giBuildNumber  = fVer.GetBuildNumber();
  //  fVer.GetSpecialBuild();
#endif 
#if wxUSE_UNICODE
  wxChar **wxArgv = new wxChar *[argc + 1];
  {
      int n;
      for (n = 0; n < argc; n++ )
      {
          wxMB2WXbuf warg = wxConvertMB2WX(argv[n]);
          wxArgv[n] = wxStrdup(warg);
      }
      wxArgv[n] = NULL;
  }
#else // !wxUSE_UNICODE
    #define wxArgv argv
#endif // wxUSE_UNICODE/!wxUSE_UNICODE

  wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");
#if defined( WANT_EXE_LINK_DATE )
  wxString wsBuildDate;
  wxString wsYear;
  bool bRet = RetrieveLinkerTimestamp( wsBuildDate, wsYear );
#else
  wxString wsBuildDate;
  wxString wsYear;
  wxString wsMonth;
  wxString wsDay;
  wsBuildDate = wxString::FromAscii( __DATE__ );
  // extract the year from the 'build' date
  gwsCopyrightYear = wsYear = wsBuildDate.AfterLast( ' ' );
#endif
  wxInitializer initializer;
  if ( !initializer )
  {
      fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
      return -1;
  }
  wxTheApp->SetAppName(_T("wxEC"));
  // won't do much good since it only controls the EOL char for the AddLines().
  // to really change all EOLs I would have to write the proper EOL to each line
  // in the Hdr and Cpp data
  // to be done by changing all data to arrays of lines & writing them out individually

  g_eolType = wxTextFileType_None;
  if (wxPlatformIs(wxOS_MAC))
    g_eolType = wxTextFileType_Mac;
  else if (wxPlatformIs(wxOS_UNIX))
    g_eolType = wxTextFileType_Unix;
  else if (wxPlatformIs(wxOS_WINDOWS))
    g_eolType = wxTextFileType_Dos;

  wxCmdLineParser parser(g_cmdLineDesc, argc, wxArgv);
  setupLogo();
  parser.SetLogo( gwsLogo );
  wxLogNull log;
  size_t n, count = 0; // avoid a compiler error msg
  switch ( parser.Parse( false ) )  // suppress automatic Usage() msg
  {
      case -1:    // come here for -?, -h & --help
        parser.Usage();
        // Check for details switch
        if (parser.Found(_T("d")))
          wxPrintf( wsDetails );
        cleanUp( argc, wxArgv );
        return -1;
      case 0:
        // Check for details switch
        if (parser.Found(_T("d")))
        {
          wxPrintf( wsDetails );
          cleanUp( argc, wxArgv );
          return -1;
        }
        // Check for input path
        if (parser.Found(_T("i"), &wsInputPath))
        {
          if( wsInputPath.IsEmpty() )
          {
            wxPrintf( _T("No Input file given.\nQuitting!") );
            cleanUp( argc, wxArgv );
            return -1;
          }
        }
        // Check for Mac EOL switch
        // these are tested after the OS Test and thus they override
        if (parser.Found(_T("m")))
        {
          g_eolType = wxTextFileType_Mac;
        }
        // Check for output path
        if (parser.Found(_T("o"), &wsOutputPath))
        {
        }
        // Check for output path
        if (parser.Found(_T("p")))
        {
          g_bPause = true;
        }
        // Check for timestamp switch
        if (parser.Found(_T("t")))
        {
          g_bCheckTimeStamps = true;
        }
        // Check for Unix EOL switch
        if (parser.Found(_T("u")))
        {
          g_eolType = wxTextFileType_Unix;
        }
        // Check for verbose switch
        if (parser.Found(_T("v")))
        {
          g_bVerbose = true;
        }
        // Check for Windows/DOS EOL switch
        if (parser.Found(_T("w")))
        {
          g_eolType = wxTextFileType_Dos;
        }
        // Check for wxWidgets  switch
        if (parser.Found(_T("x")))
        {
          g_bWxWidgets = true;
        }
        // Check for an input filename
        if (parser.GetParamCount() > 0)
        {
          wsInputFilename = parser.GetParam(0);
        }
        //ShowCmdLine(parser);
        count = parser.GetParamCount();
        for (n = 0; n < count; n++)
        {
#if 1
          if( !compileOneSet( parser.GetParam(n),
            wsInputPath, wsOutputPath )  )
          {
            cleanUp( argc, wxArgv );
            return -1;
          }

#else
          wasFileNames.Add(parser.GetParam(n)); 
#endif
        }
        // clean up the command line args for Unicode
        cleanUp( argc, wxArgv );
        return 0;
        break;
      default:
        //wxLogMessage(_T("Syntax error detected, aborting."));
        parser.Usage();
        cleanUp( argc, wxArgv );
        return -1;
        break;
  }
}

// ------------------------------- eof ------------------------------
