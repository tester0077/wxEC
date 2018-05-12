/*-----------------------------------------------------------------
 * Name:        wxEcCppData.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *-------------------------------------------------------------- */
#ifndef _WX_EC_CPP_DATA_H
#define _WX_EC_CPP_DATA_H

// ------------------------------------------------------------------
#define BASIC       1
#define WXWIDGETS   2
// ------------------------------------------------------------------

struct ecCppData
{
  wxString  wsData;
  int       iFlags;
  wxString  wsFormat;
};
// ------------------------------------------------------------------
/**
  * Format:
  * data string                                        flags              format string
  */
ecCppData ecCpp1[] = {
  { _T("//======================================================================"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("/**** Enum Class: %s ****/"),                   BASIC | WXWIDGETS, _T("s") },
  { _T("const int %s::m_iMapSize = %d;"),               BASIC | WXWIDGETS, _T("sd") },
  { _T("%s::Map_t %s::m_Map[%s::m_iMapSize];"),         BASIC | WXWIDGETS, _T("sss") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  // ctor
  { _T("%s::%s()"),                                     BASIC | WXWIDGETS, _T("ss") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\tm_Enum = %s;"),                               BASIC | WXWIDGETS, _T("u") },
};
/* // sample data
  {Unknown, "Unknown"},
  {Solid,   "Solid Matter"},
  {Liquid,  "Liquid Matter "},
  {Gas,     "Vapour"}
 */
 ecCppData ecCpp2[] = {
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  // dtor
  { _T("%s::~%s()"),                                    BASIC | WXWIDGETS, _T("ss") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("%s::Enum %s::operator=(int i)"),                BASIC | WXWIDGETS, _T("ss") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\tm_Enum = AsEnum(i);"),                        BASIC | WXWIDGETS, _T("") },
  { _T("\treturn m_Enum;"),                             BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("%s::operator int () const"),                    BASIC | WXWIDGETS, _T("s") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\treturn m_Enum;"),                             BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("%s::Enum %s::operator=(const char* sz)"),       BASIC | WXWIDGETS, _T("ss") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\tm_Enum = AsEnum(sz);"),                       BASIC | WXWIDGETS, _T("") },
  { _T("\treturn m_Enum;"),                             BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("bool %s::operator==(const char* sz)"),          BASIC | WXWIDGETS, _T("s") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\treturn m_Enum == AsEnum(sz);"),               BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("%s::operator const char* () const"),            BASIC | WXWIDGETS, _T("s") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\treturn AsName(m_Enum);"),                     BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("%s::Enum %s::AsEnum(const char* sz)"),          BASIC | WXWIDGETS, _T("ss") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\tfor(int i=0;i<m_iMapSize;i++)"),              BASIC | WXWIDGETS, _T("") },
  { _T("\t{"),                                          BASIC | WXWIDGETS, _T("") },
  { _T("\t\tif(strcmp(sz, m_Map[i].szDescription)==0)"),BASIC | WXWIDGETS, _T("") },
  { _T("\t\t{"),                                        BASIC | WXWIDGETS, _T("") },
  { _T("\t\t\treturn (Enum)m_Map[i].iValue;"),          BASIC | WXWIDGETS, _T("") },
  { _T("\t\t}"),                                        BASIC | WXWIDGETS, _T("") },
  { _T("\t}"),                                          BASIC | WXWIDGETS, _T("") },
  { _T("\treturn (Enum)m_Map[0].iValue;"),              BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("%s::Enum %s::AsEnum(int e)"),                   BASIC | WXWIDGETS, _T("ss") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\tfor(int i=0;i<m_iMapSize;i++)"),              BASIC | WXWIDGETS, _T("") },
  { _T("\t{"),                                          BASIC | WXWIDGETS, _T("") },
  { _T("\t\tif(m_Map[i].iValue == e)"),                 BASIC | WXWIDGETS, _T("") },
  { _T("\t\t{"),                                        BASIC | WXWIDGETS, _T("") },
  { _T("\t\t\treturn (Enum)m_Map[i].iValue;"),          BASIC | WXWIDGETS, _T("") },
  { _T("\t\t}"),                                        BASIC | WXWIDGETS, _T("") },
  { _T("\t}"),                                          BASIC | WXWIDGETS, _T("") },
  { _T("\treturn (Enum)m_Map[0].iValue;"),              BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("const char* %s::AsName(int e)"),                BASIC | WXWIDGETS, _T("s") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\tfor(int i=0;i<m_iMapSize;i++)"),              BASIC | WXWIDGETS, _T("") },
  { _T("\t{"),                                          BASIC | WXWIDGETS, _T("") },
  { _T("\t\tif(m_Map[i].iValue == e)"),                 BASIC | WXWIDGETS, _T("") },
  { _T("\t\t{"),                                        BASIC | WXWIDGETS, _T("") },
  { _T("\t\t\treturn m_Map[i].szDescription;"),         BASIC | WXWIDGETS, _T("") },
  { _T("\t\t}"),                                        BASIC | WXWIDGETS, _T("") },
  { _T("\t}"),                                          BASIC | WXWIDGETS, _T("") },
  { _T("\treturn m_Map[0].szDescription;"),             BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        BASIC | WXWIDGETS, _T("") },
  { _T("bool %s::GetMap(int iIndex, int& iValue, const char*& szDescription)"),
                                                        BASIC | WXWIDGETS, _T("s") },
  { _T("{"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("\tif(iIndex>=m_iMapSize)"),                     BASIC | WXWIDGETS, _T("") },
  { _T("\t{"),                                          BASIC | WXWIDGETS, _T("") },
  { _T("\t\treturn false;"),                            BASIC | WXWIDGETS, _T("") },
  { _T("\t}"),                                          BASIC | WXWIDGETS, _T("") },
  { _T("\tiValue = m_Map[iIndex].iValue;"),             BASIC | WXWIDGETS, _T("") },
  { _T("\tszDescription = m_Map[iIndex].szDescription;"),BASIC | WXWIDGETS, _T("") },
  { _T("\treturn true;"),                               BASIC | WXWIDGETS, _T("") },
  { _T("}"),                                            BASIC | WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        WXWIDGETS, _T("") },
  { _T("bool %s::GetMap(int iIndex, int& iValue, wxString& wsDescription)"),
                                                        WXWIDGETS, _T("s") },
  { _T("{"),                                            WXWIDGETS, _T("") },
  { _T("\tif(iIndex>=m_iMapSize)"),                     WXWIDGETS, _T("") },
  { _T("\t{"),                                          WXWIDGETS, _T("") },
  { _T("\t\treturn false;"),                            WXWIDGETS, _T("") },
  { _T("\t}"),                                          WXWIDGETS, _T("") },
  { _T("\tiValue = m_Map[iIndex].iValue;"),             WXWIDGETS, _T("") },
  { _T("\twsDescription = m_Map[iIndex].wsDescription;"), WXWIDGETS, _T("") },
  { _T("\treturn true;"),                               WXWIDGETS, _T("") },
  { _T("}"),                                            WXWIDGETS, _T("") },
  { _T("//----------------------------------------------------------------------"),
                                                        WXWIDGETS, _T("") },
  { _T("const wxString& %s::AsWxName(int e)"),          WXWIDGETS, _T("s") },
  { _T("{\r\n"),                                        WXWIDGETS, _T("") },
  { _T("\tfor(int i=0;i<m_iMapSize;i++)"),              WXWIDGETS, _T("") },
  { _T("\t{"),                                          WXWIDGETS, _T("") },
  { _T("\t\tif(m_Map[i].iValue == e)"),                 WXWIDGETS, _T("") },
  { _T("\t\t{"),                                        WXWIDGETS, _T("") },
  { _T("\t\t\treturn m_Map[i].wsDescription;"),         WXWIDGETS, _T("") },
  { _T("\t\t}"),                                        WXWIDGETS, _T("") },
  { _T("\t}"),                                          WXWIDGETS, _T("") },
  { _T("\treturn m_Map[0].wsDescription;"),             WXWIDGETS, _T("") },
  { _T("}"),                                            WXWIDGETS, _T("") },
  { _T(""),                                             WXWIDGETS, _T("") },
};

#endif  // _WX_EC_CPP_DATA_H
// ------------------------------- eof ---------------------------
