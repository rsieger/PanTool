#ifndef _GLOBALS

    #define _GLOBALS

// Application constants

    const QString _PROGRAMDOI_                       = "doi:10.1594/PANGAEA.787549";
    const QString _PROGRAMREADME_                    = "https://wiki.pangaea.de/wiki/PanTool";

    const QString _VERSION_                          = "3.27";	// Version number, 2016-11-16
    const QChar   _PSEPARATOR_                       = '|';

    const int   _NOERROR_                            = 0;
    const int   _ERROR_                              = 1;
    const int   _APPBREAK_                           = 2;
    const int   _NODATAFOUND_                        = 3;
    const int   _DATAFOUND_                          = 4;
    const int   _CHOOSEABORTED_                      = 5;
    const int   _FILENOTEXISTS_                      = 6;
    const int   _WRONGDATETIMEFORMAT_                = 7;

    const int   _CONCATENATEBYLINES                  = 100;
    const int   _CONCATENATEBYCOLUMNS                = 101;
    const int   _EXTRACTLINES                        = 102;
    const int   _EXTRACTCOLUMNS                      = 103;
    const int   _EXTRACT10MINLINES                   = 104;
    const int   _DELETELINES                         = 105;
    const int   _DELETECOLUMNS                       = 106;
    const int   _DELETEDOUBLELINES                   = 107;
    const int   _DELETECOMMENTBLOCKS                 = 108;
    const int   _INSERTCHARS                         = 109;
    const int   _REPLACECHARS                        = 110;
    const int   _SEARCHONESTRING                     = 111;
    const int   _SEARCHANDREPLACEONESTRING           = 112;
    const int   _SEARCHANDREPLACEMANYSTRINGSATONCE   = 113;
    const int   _SPLITBYLINES                        = 114;
    const int   _SPLITBYCOLUMNS                      = 115;
    const int   _SPLITLARGE                          = 116;
    const int   _COLUMNSTABLE                        = 117;
    const int   _TABLECOLUMNS                        = 118;
    const int   _RECALCCOLUMNS                       = 119;

    const int   _LATITUDE                            = 0;
    const int   _LONGITUDE                           = 1;

    const int   _LINE                                = 1;
    const int   _CELL                                = 2;

    const int   _ADDLINE                             = 1;
    const int   _ADDBLOCK                            = 2;
    const int   _ADDCOLUMN                           = 3;

    const int   _BUILDDATE                           = 1;
    const int   _BUILDDATETIME                       = 2;
    const int   _BUILDISODATETIME                    = 3;

    const int   _ZIP_                                = 1;
    const int   _GZIP_                               = 2;

    const int   _FILENAMEPOS                         = 0;
    const int   _DATETIMEPOS                         = 1;
    const int   _LATITUDEPOS                         = 2;
    const int   _LONGITUDEPOS                        = 3;
    const int   _ALTITUDEPOS                         = 4;
    const int   _COPYRIGHTPOS                        = 5;

    const int   _MAX_NUM_OF_AREAS					 = 5000;	// maximum number of areas, used in find areas tool
    const int   _MAX_NUM_OF_LOCATIONS				 = 500000;	// maximum number of locations, used in find areas tool

    const int   _RED                                 = 0;
    const int   _GREEN                               = 1;
    const int   _BLUE                                = 2;
    const int   _ORANGE                              = 3;
    const int   _YELLOW                              = 4;
    const int   _WHITE                               = 5;

    const int   _CIRCLE                              = 0;
    const int   _SQUARE                              = 1;
    const int   _STAR                                = 2;
    const int   _TRIANGLE                            = 3;

    const int   _TABSTOP_                            = 0;
    const int   _COMMA_                              = 1;
    const int   _SEMICOLON_                          = 2;
    const int   _SPACE_                              = 3;
    const int   _DOT_                                = 4;

// EOL
    const int   _UNIX_                                = 0;    // LF
    const int   _MACOS_                               = 1;    // CR
    const int   _WIN_                                 = 2;    // CR+LF

// Encoding
    const int   _SYSTEM_                              = -1;   // System
    const int   _UTF8_                                = 0;    // UTF-8
    const int   _APPLEROMAN_                          = 1;    // Apple Roman
    const int   _LATIN1_                              = 2;    // Latin-1 = ISO 8859-1

// Extension
    const int   _TXT_                                 = 0;
    const int   _CSV_                                 = 1;

#endif
