/*-----------------------------------------------------------------
 * Name:        wxEcHdrData.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *-------------------------------------------------------------- */
#ifndef _WX_EC_HDR_DATA_H
#define _WX_EC_HDR_DATA_H

// ------------------------------------------------------------------
#define BASIC       1
#define WXWIDGETS   2
// ------------------------------------------------------------------

struct ecHdrData
{
  wxString  wsData;
  int       iFlags;
  wxString  wsFormat;
};
// ------------------------------------------------------------------
/**
   * Format:
   * data string                      flags      format string
   */
ecHdrData ecHdr1[] = {
  { _T("#include \"wx/string.h\""),   WXWIDGETS, _T("") },
  { _T(""),                           WXWIDGETS, _T("") },
  { _T("#define _%s_MAP_SIZE %d"),    BASIC | WXWIDGETS, _T("sd") },
  { _T("//========================================================================"), 
                        BASIC | WXWIDGETS, _T("") },
  { _T("class %s"),     BASIC | WXWIDGETS, _T("s") },
  { _T("{"),            BASIC | WXWIDGETS, _T("") },
  { _T("public:"),      BASIC | WXWIDGETS, _T("") },
  { _T("\t// %s::%s"),  BASIC | WXWIDGETS, _T("st") },
  { _T("\tenum Enum"),  BASIC | WXWIDGETS, _T("") },
  { _T("\t{"),          BASIC | WXWIDGETS, _T("") }
};    /*  sample data is output between these two sets of data
          Unknown = 0,
          Solid   = 1,  // "Solid Matter"
          Liquid  = 2,  // "Liquid Matter "
          Gas,          // "Vapour" */
ecHdrData ecHdr2[] = {
  { _T("\t};"),                                           BASIC | WXWIDGETS, _T("") },
  { _T("\t%s();"),                                        BASIC | WXWIDGETS, _T("s")},
  { _T("\tvirtual ~%s();"),                               BASIC | WXWIDGETS, _T("s")},
  { _T("\tinline int GetSize() { return m_iMapSize; };"), BASIC | WXWIDGETS, _T("") },
  { _T("\tEnum operator=(int i);"),                       BASIC | WXWIDGETS, _T("") },
  { _T("\toperator int () const;"),                       BASIC | WXWIDGETS, _T("") },
  { _T("\tEnum operator=(const char* sz);"),              BASIC | WXWIDGETS, _T("") },
  { _T("\tbool operator==(const char* sz);"),             BASIC | WXWIDGETS, _T("") },
  { _T("\toperator const char* () const;"),               BASIC | WXWIDGETS, _T("") },
  { _T("\tstatic Enum AsEnum(const char* sz);"),          BASIC | WXWIDGETS, _T("") },
  { _T("\tstatic Enum AsEnum(int e);"),                   BASIC | WXWIDGETS, _T("") },
  { _T("\tstatic const char* AsName(int e);"),            BASIC | WXWIDGETS, _T("") },
  { _T("\tstatic bool GetMap(int iIndex, int& iValue, const char*& szDescription);"),
                                                          BASIC | WXWIDGETS, _T("") },
  { _T("\tinline operator wxString () {return AsWxName(m_Enum); };"),
                                                                  WXWIDGETS, _T("") },
  { _T("\tstatic const wxString& AsWxName(int e);"),              WXWIDGETS, _T("") },
  { _T("\tstatic bool GetMap(int iIndex, int& iValue, wxString& wsDescription);"),
                                                                  WXWIDGETS, _T("") },
  { _T("\tEnum m_Enum;"),                                 BASIC | WXWIDGETS, _T("") },
  { _T("\tclass Map_t"),                                  BASIC | WXWIDGETS, _T("") },
  { _T("\t{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("public:"),                                        BASIC | WXWIDGETS, _T("") },
  { _T("\t\tint iValue;"),                                BASIC | WXWIDGETS, _T("") },
  { _T("\t\tchar* szDescription;"),                 BASIC | WXWIDGETS, _T("") },
  { _T("\t\twxString wsDescription;"),              BASIC | WXWIDGETS, _T("") },
  { _T("\t\tMap_t( int iV = 0, char* szDesc = \"\", wxString wsDesc = _T(\"\") )"),
                                                          BASIC | WXWIDGETS, _T("") },
  { _T("\t\t  {"),                                        BASIC | WXWIDGETS, _T("") },
  { _T("\t\t    iValue = iV;"),                           BASIC | WXWIDGETS, _T("") },
  { _T("\t\t    szDescription = szDesc;"),                BASIC | WXWIDGETS, _T("") },
  { _T("\t\t    wsDescription = wsDesc;"),                BASIC | WXWIDGETS, _T("") },
  { _T("\t\t  };"),                                       BASIC | WXWIDGETS, _T("") },
  { _T("\t};"),                                           BASIC | WXWIDGETS, _T("") },
  { _T("\tstatic Map_t m_Map[];"),                        BASIC | WXWIDGETS, _T("") },
  { _T("\tstatic const int m_iMapSize;"),                 BASIC | WXWIDGETS, _T("") },
  { _T("};"),                                             BASIC | WXWIDGETS, _T("") },
};

#endif  // _WX_EC_HDR_DATA_H
// ------------------------------- eof ---------------------------
