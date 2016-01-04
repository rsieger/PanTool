/* Application.h              */
/* 2013-05-15                 */
/* Dr. Rainer Sieger          */

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtXml/QtXml>

#include <QTextEdit>
#include <QTextCodec>
#include <QTextStream>
#include <QStringListModel>
#include <QListView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QTimer>
#include <QMimeData>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QStatusBar>
#include <QSettings>

#include <qmath.h>

#include "Globals.h"
#include "Webfile.h"
#include "findArea.h"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QAction;
class QLabel;
class QProgressDialog;
class QProgressBar;
class QTimer;
class QDateTime;
class QUrl;
class QTextEdit;
class QStringListModel;
class QListView;
class QTableWidget;
class QXml;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO( "author", "Rainer Sieger" )
    Q_CLASSINFO( "url", "http://www.pangaea.de" )

    struct structLocation
    {
        double	Latitude;
        double	Longitude;
    };

    struct structArea
    {
        QString	AreaName;       // Name of area
        int     NumOfPoints;    // Number of points in closed polygon
        float   minLat;         // Minimum latitude
        float   maxLat;         // Maximum latitude
        float   minLong;        // Minimum longitude
        float   maxLong;        // Maximum longitude
    };

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

    QStringList gsl_FilenameList;                   //!< Liste aller Dateinamen incl. Pfad.

    // General settings
    QString     gs_Path;                            //!< Der zuletzt verwendete Pfad.
    QString     gs_FilenameFormat;                  //!< Bestimmt in die Form des automatisch erzeugten neuen Dateinamens. \%a = aktuelle Actionnumber, \%N = Dateiname.
    QString     gs_Version;                         //!< Aktuelle Version des Programms.

    int         gi_ActionNumber;                    //!< Die ActionNumber wird bei jedem angewandenten Tool um 1 erhoeht.
    int         gi_NumOfProgramStarts;              //!< Anzahl der Programmstarts
    int         gi_CodecInput;                      //!< Encoding der Inputdateien
    int         gi_CodecOutput;                     //!< Encoding der Outputdateien
    int         gi_CodecDownload;                   //!< Encoding der Downloaddateien
    int         gi_EOL;                             //!< End-of-Line Zeichen fuer die Outputdateien
    int         gi_Extension;                       //!< Dateierweiterung fuer Ausgabedateien

    bool        gb_showProgressBar;                 //!< Soll der ProgressBar im Fenster unten rechts angezeigt werden?

    // extract columns
    QString     gs_ec_ExtractColumnsList;           //!< Liste der zu extrahierenden Spalten
    bool        gb_ec_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_ec_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_ec_DeleteInputFile;              //!< loesche Inputdatei.

    // extract matched columns
    QString     gs_emc_ExtractColumnsSearchString;   //!< Suchbegriff fuer das Extrahieren von Spalten
    bool        gb_emc_SaveNoMatch;                  //!< Speicher die nicht extrahierten Zeilen
    bool        gb_emc_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_emc_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_emc_DeleteInputFile;              //!< loesche Inputdatei.
    bool        gb_emc_DeleteEmptyOutputFile;        //!< Erstelle keine leere Outputdatei.

    // extract lines
    int         gi_el_StartLine;                    //!< Bearbeitung startet in Zeile
    int         gi_el_Increment;                    //!< Zeilen Inkrement
    int         gi_el_NumberOfLines;                //!< Anzahl von Zeilen
    bool        gb_el_ExtractFristLineLastLine;     //!< Lese nur die erste und letzte Zeile
    bool        gb_el_SaveNoMatch;                  //!< Speicher die nicht extrahierten Zeilen
    bool        gb_el_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_el_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_el_DeleteInputFile;              //!< loesche Inputdatei.
    bool        gb_el_DeleteEmptyOutputFile;        //!< Erstelle keine leere Outputdatei.

    // extract matched lines
    QString     gs_eml_ExtractLinesSearchString;    //!< Suchbegriff fuer das Extrahieren von Zeilen
    int         gi_eml_StartLine;                   //!< Bearbeitung startet in Zeile
    int         gi_eml_NumberOfLines;               //!< Anzahl von Zeilen
    bool        gb_eml_SaveFirstLine;               //!< Speicher auf jeden Fall die erste Zeile der Datei
    bool        gb_eml_SaveNoMatch;                 //!< Speicher die nicht extrahierten Zeilen
    bool        gb_eml_SkipEmptyLines;              //!< Ueberspringe leere Zeilen
    bool        gb_eml_SkipCommentLines;            //!< Ueberspringe Comment Zeilen
    bool        gb_eml_DeleteInputFile;             //!< loesche Inputdatei.
    bool        gb_eml_DeleteEmptyOutputFile;       //!< Erstelle keine leere Outputdatei.

    // extract 10 min lines
    bool        gb_e10minl_SaveHeader;              //!< ueberspringe die erste Zeile.
    bool        gb_e10minl_DeleteInputFile;         //!< loesche Inputdatei.

    // delete columns
    QString     gs_dc_DeleteColumnsList;            //!< Liste der zu loeschenden Spalten
    bool        gb_dc_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_dc_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_dc_DeleteInputFile;              //!< loesche Inputdatei.

    // delete matched columns
    QString     gs_dmc_DeleteColumnsSearchString;    //!< Suchbegriff fuer das Loeschen von Spalten
    bool        gb_dmc_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_dmc_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_dmc_DeleteInputFile;              //!< loesche Inputdatei.

    // delete lines
    int         gi_dl_StartLine;                    //!< Bearbeitung startet in Zeile
    int         gi_dl_NumberOfLines;                //!< Anzahl von Zeilen
    bool        gb_dl_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_dl_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_dl_DeleteInputFile;              //!< loesche Inputdatei.

    // delete matched lines
    QString     gs_dml_DeleteLinesSearchString;      //!< Suchbegriff fuer das Loeschen von Zeilen
    bool        gb_dml_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_dml_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_dml_DeleteInputFile;              //!< loesche Inputdatei.

    // delete double lines
    bool        gb_ddl_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_ddl_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_ddl_DeleteInputFile;              //!< loesche Inputdatei.

    // delete comment blocks
    bool        gb_dcl_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_dcl_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_dcl_DeleteInputFile;              //!< loesche Inputdatei.

    // insert characters at position
    QString     gs_ic_InsertPositionList;            //!< Positionsliste fuer das Einfuegen von Zeichenketten
    QString     gs_ic_InsertStr;                     //!< Einzusetzende Zeichenkette
    bool        gb_ic_SkipEmptyLines;                //!< Ueberspringe leere Zeilen
    bool        gb_ic_SkipCommentLines;              //!< Ueberspringe Comment Zeilen
    bool        gb_ic_DeleteInputFile;               //!< loesche Inputdatei.

    // replace characters at position
    QString     gs_rc_ReplacePositionList;           //!< Positionsliste fuer das Ersetzen von Zeichenketten
    QString     gs_rc_ReplaceStr;                    //!< Einzusetzende Zeichenkette
    bool        gb_rc_SkipEmptyLines;                //!< Ueberspringe leere Zeilen
    bool        gb_rc_SkipCommentLines;              //!< Ueberspringe Comment Zeilen
    bool        gb_rc_DeleteInputFile;               //!< loesche Inputdatei.

    // search one string
    QString     gs_sos_SearchString;                 //!< Suchbegriff
    int         gi_sos_StartLine;                    //!< Bearbeitung startet in Zeile
    int         gi_sos_NumberOfLines;                //!< Anzahl von Zeilen
    bool        gb_sos_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_sos_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_sos_DeleteInputFile;              //!< loesche Inputdatei.
    bool        gb_sos_DeleteEmptyOutputFile;        //!< Erstelle keine leere Outputdatei.

    // search and replace one string
    QString     gs_srs_SearchString;                 //!< Suchbegriff
    QString     gs_srs_ReplaceString;                //!< Ersetzbegriff
    int         gi_srs_StartLine;                    //!< Bearbeitung startet in Zeile
    int         gi_srs_NumberOfLines;                //!< Anzahl von Zeilen
    bool        gb_srs_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_srs_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_srs_DeleteInputFile;              //!< loesche Inputdatei.

    // search and replace many strings at once
    QString     gs_srm_DatabaseName;                 //!< Pfad und Name der Search and Replace Database
    int         gi_srm_StartLine;                    //!< Bearbeitung startet in Zeile
    int         gi_srm_NumberOfLines;                //!< Anzahl von Zeilen
    int         gi_srm_SearchAndReplaceMode;         //!< Search and Replace Mode (_LINE oder _CELL)
    bool        gb_srm_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_srm_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_srm_DeleteInputFile;              //!< loesche Inputdatei.

    // concatenate files by columns
    int         gi_cfc_SkipNStartLines;              //!< Ueberspringe n Zeilen am Anfang der 2. bis n. Datei
    bool        gb_cfc_IncludeFilename;              //!< Fuege Dateinamen ein
    bool        gb_cfc_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_cfc_DeleteInputFile;              //!< loesche Inputdatei.

    // concatenate files by line
    int         gi_cfl_SkipNStartLines;              //!< Ueberspringe n Zeilen am Anfang der 2. bis n. Datei
    bool        gb_cfl_IncludeFilename;              //!< Fuege Dateinamen ein
    bool        gb_cfl_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_cfl_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_cfl_DeleteInputFile;              //!< loesche Inputdatei.

    // split files by columns
    int         gi_sfc_NumberOfColumns;              //!< max. Anzahl von Spalten
    int         gi_sfc_NumberOfMetadataColumns;      //!< Anzahl der Spalten, die in jeder Datei auftauchen sollen
    bool        gb_sfc_DeleteInputFile;              //!< loesche Inputdatei.

    // split files by line
    int         gi_sfl_NumberOfLines;                //!< max. Anzahl von Zeilen in jeder Datei
    int         gi_sfl_NumberOfHeaderLines;          //!< Anzahl der Zeilen, die in jeder Datei auftauchen sollen
    bool        gb_sfl_SkipEmptyLines;               //!< Ueberspringe leere Zeilen
    bool        gb_sfl_SkipCommentLines;             //!< Ueberspringe Comment Zeilen
    bool        gb_sfl_DeleteInputFile;              //!< loesche Inputdatei.

    // add line
    QString     gs_al_Text;                          //!< dieser Text wird in Zeile eingesetzt
    int         gi_al_LineNo;                        //!< Zeilennummer
    bool        gb_al_AddFilename;                   //!< fuege Dateinamen ein
    bool        gb_al_AddFullPath;                   //!< fuege Pfad und Dateinamen ein
    bool        gb_al_AddOrdinalNumber;              //!< fuege fortlaufende Nummer ein
    bool        gb_al_SkipEmptyLines;                //!< Ueberspringe leere Zeilen
    bool        gb_al_SkipCommentLines;              //!< Ueberspringe Comment Zeilen
    bool        gb_al_DeleteInputFile;               //!< loesche Inputdatei.

    // add block
    QString     gs_ab_Text;                          //!< dieser Text wird in Zeile eingesetzt
    int         gi_ab_LineNo;                        //!< Zeilennummer
    bool        gb_ab_AddFilename;                   //!< fuege Dateinamen ein
    bool        gb_ab_AddFullPath;                   //!< fuege Pfad und Dateinamen ein
    bool        gb_ab_AddOrdinalNumber;              //!< fuege fortlaufende Nummer ein
    bool        gb_ab_SkipEmptyLines;                //!< Ueberspringe leere Zeilen
    bool        gb_ab_SkipCommentLines;              //!< Ueberspringe Comment Zeilen
    bool        gb_ab_DeleteInputFile;               //!< loesche Inputdatei.

    // add column
    QString     gs_ac_HeaderText;                    //!< dieser Text wird in Spalte in der ersten Zeile eingesetzt
    QString     gs_ac_ColumnText;                    //!< dieser Text wird in Splate in jeder Zeile eingesetzt
    bool        gb_ac_AddFilename;                   //!< fuege Dateinamen ein
    bool        gb_ac_AddFullPath;                   //!< fuege Pfad und Dateinamen ein
    bool        gb_ac_AddOrdinalNumber;              //!< fuege fortlaufende Nummer ein
    bool        gb_ac_SkipEmptyLines;                //!< Ueberspringe leere Zeilen
    bool        gb_ac_SkipCommentLines;              //!< Ueberspringe Comment Zeilen
    bool        gb_ac_DeleteInputFile;               //!< loesche Inputdatei.

    // recalc columns
    QString     gs_rec_RecalcColumnsList;            //!< Liste der zu rekalkurienden Spalten
    float       gf_rec_MulX;                         //!< Column x X
    float       gf_rec_AddX;                         //!< Column + X
    int         gi_rec_NumOfDigits;                  //!< Ausgabegenauigkeit
    bool        gb_rec_OnlyIfEmpty;                  //!< Addition nur machen, wenn Zelle leer ist
    bool        gb_rec_DeleteInputFile;              //!< loesche Inputdatei.

    // find area
    QString     gs_AreaDatabaseFilename;             //!< Name der Area database
    bool        gb_fa_DeleteInputFile;               //!< loesche Inputdatei

    // table to columns
    int         gi_tcf_NumOfGeocodes;                //!< Anzahl der Geocodes
    bool        gb_tcf_DeleteInputFile;              //!< loesche Inputdatei.

    // columns to table
    QString     gs_cft_MissingValue;                 //!< Missing value String
    int         gi_cft_FieldDelimiter;               //!< Feldtrenner
    bool        gb_cft_DeleteInputFile;              //!< loesche Inputdatei.

    // date/time
    int         gi_dt_DateColumn;                    //!< Spalte fuer das Datum
    int         gi_dt_YearColumn;                    //!< Spalte fuer das Jahr
    int         gi_dt_MonthColumn;                   //!< Spalte fuer den Monat
    int         gi_dt_DayColumn;                     //!< Spalte fuer den Tag
    int         gi_dt_TimeColumn;                    //!< Spalte fuer die Uhrzeit
    int         gi_dt_HourColumn;                    //!< Spalte fuer die Stunde
    int         gi_dt_MinuteColumn;                  //!< Spalte fuer die Minute
    int         gi_dt_SecondColumn;                  //!< Spalte fuer die Sekunde
    int         gi_dt_DateTimeColumn;                //!< Spalte fuer Datum/Uhrzeit
    int         gi_dt_DayOfYearColumn;               //!< Spalte fuer Tag im Jahr
    int         gi_dt_JulianDayColumn;               //!< Spalte fuer den Julianischen Tag
    int         gi_dt_MatLabDateColumn;              //!< Spalte fuer das MatLab Date
    bool        gb_dt_WriteDateTimeOnly;             //!< schreibe nur die Spalte Date/Time

    // Pressure to depth
    int         gi_pd_LatitudeColumn;                //!< Spalte fuer Latitude
    int         gi_pd_LongitudeColumn;               //!< Spalte fuer Longitude
    int         gi_pd_PressureColumn;                //!< Spalte fuer Pressure
    int         gi_pd_NumOfDigits;                   //!< Ausgabegenauigkeit
    bool        gb_pd_DeleteInputFile;               //!< loesche Inputdatei.

    // PanGet
    QString     gs_IDListFile;                       //!< PanGet, Name der Datei, die die ID-Liste enthaelt
    QString     gs_DownloadDirectory;                //!< PanGet, Download-Verzeichnis
    bool        gb_DownloadData;                     //!< PanGet, hole Dataset als Text
    bool        gb_DownloadCitation;                 //!< PanGet, hole Citation als Text
    bool        gb_DownloadMetadata;                 //!< PanGet, hole nur die Metadaten als XML

    // ODP Sample Label
    int         gi_odp_LegColumn;                    //!< Spalte fuer Leg number
    int         gi_odp_SiteColumn;                   //!< Spalte fuer Site number
    int         gi_odp_HoleColumn;                   //!< Spalte fuer Hole
    int         gi_odp_CoreColumn;                   //!< Spalte fuer Core number
    int         gi_odp_CoreTypeColumn;               //!< Spalte fuer Core-Type
    int         gi_odp_SectionColumn;                //!< Spalte fuer Section number
    int         gi_odp_TopColumn;                    //!< Spalte fuer Section Top
    int         gi_odp_BottomColumn;                 //!< Spalte fuer Section Bottom
    bool        gb_odp_DeleteInputFile;              //!< loesche Inputdatei.

    // Translate character encoding
    int         gi_tce_CodecInput;                   //!< Encoding der Inputdateien
    int         gi_tce_CodecOutput;                  //!< Encoding der Outputdateien
    int         gi_tce_EOL;                          //!< End-of-Line Zeichen fuer die Outputdateien
    bool        gb_tce_DeleteInputFile;              //!< loesche Inputdatei.

    // move files
    QString     gs_FilenameListFile;                 //!< Liste der zu verschiebenen Dateien
    QString     gs_TargetDirectory;                  //!< Zielverzeichnis

    // rename files
    QString     gs_rf_SearchString;                  //!< Search string in filename
    QString     gs_rf_ReplaceString;                 //!< Replace string in filename

    // tranpose table
    int         gi_tt_CodecInput;                    //!< Encoding der Inputdateien
    int         gi_tt_CodecOutput;                   //!< Encoding der Outputdateien
    int         gi_tt_EOL;                           //!< End-of-Line Zeichen fuer die Outputdateien
    bool        gb_tt_DeleteInputFile;               //!< loesche Inputdatei.

    // External web path
    QString     gs_ExternalWebPath;                  //!< External web path

    bool LineCanBeWritten( const QString &InputStr, const bool SkipEmptyLines, const bool SkipCommentLines );
    bool isInColumnList( const QList<int> ColumnList, const int ColumnNo );
    QList<int> scanList( const int mode, const int maxNumOfPositions, const QString &List );

    void compressFile( const QString &FilenameIn );
    int compressFolder( const QString &Folder );

    void downloadDatasets( const QString &IDListFile, const QString &DownloadDirectory, const bool DownloadData, const bool DownloadCitation, const bool DownloadMetadata, const int CodecDownload, const int EOL, const int Extension );

    int extractColumns( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &ColumnList = "", const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int extractMatchedColumns( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &SearchString = "", const bool SaveNoMatch = false, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const bool DeleteEmptyOutputFile = false, const int NumOfFiles = 0 );
    int extractLines( const QString &s_FilenameIn, const QString &s_FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int StartLine = 1, const int Increment = 1, const int NumberOfLines = 1, const bool ExtractFristLineLastLine = false, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int extractMatchedLines( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &SearchString = "", const bool SaveFirstLine = true, const bool SaveNoMatch = false, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteEmptyOutputFile = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int extract10minLines( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const bool SkiptFirstLine = true, const bool DeleteInputFile = false, const int NumOfFiles = 0 );

    int deleteColumns( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &ColumnList = "", const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int deleteMatchedColumns( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &SearchString = "", const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int deleteLines( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int StartLine = 1, const int NumberOfLines = 1, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int deleteMatchedLines( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &SearchString = "", const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );

    int deleteDoubleLines( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int deleteCommentBlocks( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );

    int insertCharactersAtPosition( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &CharacterPositionList = "", const QString &InsertStr = "", const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int replaceCharactersAtPosition( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &CharacterPositionList = "", const QString &ReplaceStr = "", const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );

    int searchOneString( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList &FilenameList, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &SearchStr = "", const int StartLine = 1, const int NumberOfLines = 0, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const bool DeleteEmptyOutputFile = false );
    int searchAndReplaceOneString( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &SearchStr = "", const QString &ReplaceStr = "", const int StartLine = 1, const int NumberOfLines = 0, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int searchAndReplaceManyStringsAtOnce( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QStringList &SearchStrings, const QStringList &ReplaceStrings, const int StartLine = 1, const int NumberOfLines = 0, const int SearchAndReplaceMode = _LINE, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int buildSearchAndReplaceDatabase( const QString &FilenameIn, const int CodecInput, const int CodecOutput, const int EOL, const int NumOfFiles );
    int concatenateFilesByColumns( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList &FilenameList, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int SkipNStartLines = 1, const bool IncludeFilename = false, const bool DeleteInputFile = false );
    int concatenateFilesByLines( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList &FilenameList, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int SkipNStartLines = 1, const bool IncludeFilename = false, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false );

    int createODPSampleLabel( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int Leg, const int Site, const int Hole, const int Core, const int CoreType, const int Section, const int Top, const int Bottom, const bool DeleteInputFile, const int NumOfFiles );

    int splitFileByColumns( const QString &FilenameIn, const int CodecInput, const int CodecOutput, const int EOL, const int Extension, const int NumberOfColumns = 1, const int NumberOfMetadataColumns = 0, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int splitFileByLines( const QString &FilenameIn, const int CodecInput, const int CodecOutput, const int EOL, const int Extension, const int NumberOfLines = 1, const int NumberOfHeaderLines = 0, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int splitLargeFile( const QString &FilenameIn, const int CodecInput, const int CodecOutput, const int EOL, const int Extension, const int NumberOfHeaderLines = 0, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );

    int addColumn( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &HeaderText, const QString &ColumnText, const bool AddFilename = false, const bool AddFullPath = false, const bool AddOrdinalNumber = false, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int addLine( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QStringList &Text, const int LineNo = 1, const bool AddFilename = false, const bool AddFullPath = false, const bool AddOrdinalNumber = false, const bool SkipEmptyLines = false, const bool SkipCommentLines = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );

    int recalcColumns( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const QString &ColumnsList = "", const float MulX = 1., const float AddX = 0., const int NumOfDigits = 5, const bool OnlyIfEmpty = false, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int transposeTable( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int createScript(const QString &FilenameIn, const int CodecInput , const int NumOfFiles);
    int saveFilelist( const QString &FilenameOut, const QStringList FilenameList, const int CodecOutput, const QString &ExternalWebPath, const int EOL );

    int calcDepthFromPressure( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int LatitudeColumn, const int PressureColumn, const int NumOfDigits, const bool DeleteInputFile, const int NumOfFiles );
    float gravity( const float lat );
    float pzcon( const int cnvrsn, const float grav, const float dynz, const float prsdb, const float depth );

    int findArea( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, QVector<AreaItem> &Area, QVector<PositionItem> &Position, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int readAreaDataBaseFile(const QString &FilenameADB, QVector<AreaItem> &Area, QVector<PositionItem> &Position );
    int buildAreaDatabase( const QString& FilenameIn, const int CodecInput, const int NumOfFiles );
    int PtInPolygon( double d_Latitude, double d_Longitude, int StartPosition, int NumOfPoints, QVector<PositionItem> &v_Position );
    int polyCentroid( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const bool DeleteInputFile, const int NumOfFiles );

    int findDateTimeColumn( const QString InputStr );
    int createDateTime( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int DateColumn, const int YearColumn, const int MonthColumn, const int DayColumn, const int TimeColumn, const int HourColumn, const int MinuteColumn, const int SecondColumn, const int DateTimeColumn, const int DayOfYearColumn, const int JulianDayColumn,  const int MatLabDateColumn, const bool WriteDateTimeOnly, const int NumOfFiles );

    int getYear( const QString YearIn );
    int getMonth( const QString MonthIn );
    int getTimeClass3h( const int Hour );

    QDate getDate( const QString DateIn );
    QTime getTime( const QString TimeIn );

    int checkTimeSeries( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int NumOfFiles );
    int calcSalinityFromCondTempPress( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int NumOfFiles );

    double Cond2Sal78 ( const double Cond, const double Temp, const double Press );
    double SAL( const double XR, const double XT);
    double A( const double XT );
    double B( const double XT );
    double C( const double XP );
    double RT35( const double XT );

    int convertColumns2TableFormat( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int FieldDelimiter, const QString &MissingValue, const bool DeleteInputFile, const int NumOfFiles );
    int convertTable2ColumnsFormat( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int NumOfGeocodes, const bool DeleteInputFile, const int NumOfFiles );

    int doAddDialog( const int mode, QString &HeaderText, QString &ColumnText, int &LineNo, bool &AddFilename, bool &AddFullPath, bool &AddOrdinalNumber, bool &SkipEmptyLines, bool &SkipCommentLines, bool &DeleteInputFile );
    int doColumnListDialog( const int mode, QString &ColumnList, bool &SkipEmptyLines, bool &SkipCommentLines, bool &DeleteInputFile );
    int doConcatenateFilesOptionsDialog( const int mode, int &SkipNStartLines, bool &IncludeFilename, bool &SkipEmptyLines, bool &SkipCommentLines, bool &DeleteInputFile );
    int doCharactersAtPositionDialog( const int mode, QString &PositionList, QString &Text, bool &SkipEmptyLines, bool &SkipCommentLines, bool &DeleteInputFile );
    int doDateTimeDialog( int &DateColumn, int &YearColumn, int &MonthColumn, int &DayColumn, int &TimeColumn, int &HourColumn, int &MinuteColumn, int &SecondColumn, int &DateTimeColumn,  int &DayOfYearColumn, int &JulianDayColumn, int &MatLabDateColumn, bool &WriteDateTimeOnly );
    int doDeleteLinesDialog( int &StartLine, int &NumberOfLines, bool &SkipEmptyLines, bool &SkipCommentLines, bool &DeleteInputFile );
    int doFileListOptionsDialog( QString &ExternalWebPath );
    int doExtractLinesDialog( int &StartLine, int &Increment, int &NumberOfLines, bool &ExtractFristLineLastLine, bool &SkipEmptyLines, bool &SkipCommentLines, bool &DeleteInputFile );
    int doFindAreaDialog( QString &AreaDatabaseFilename, bool &DeleteInputFile );
    int doFourColumnsDialog( const int mode, int &NumOfGeocodes, int &FieldDelimiter, QString &MissingValue, bool &DeleteInputFile );
    int doLatLongOptionsDialog( int &ColumnLat, int &ColumnLong, int &ColumnPressure, int &NumOfDigits, bool &DeleteInputFile );
    int doRecalcDialog( QString &ColumnsList, float &MulX, float &AddX, int &NumOfDigits, bool &OnlyIfEmpty, bool &DeleteInputFile );
    int doRenameFilesDialog( QString &SearchString, QString &ReplaceString );
    int doSearchDialog( const int mode, QString &SearchString, QString &ReplaceString, QString &s_DatabaseName, int &StartLine, int &BreakAfterNLines, int &SearchAndReplaceMode, bool &SaveFirstLine, bool &SaveNoMatch, bool &SkipEmptyLines, bool &SkipCommentLines,  bool &NoEmptyOutputFile, bool &DeleteInputFile );
    int doSkipLinesDialog( const int mode, bool &SaveHeader, bool &SkipEmptyLines, bool &SkipCommentLines, bool &DeleteInputFile );
    int doSplitFileDialog( const int mode, int &NumberOfLines, int &NumberOfHeaderLines, int &NumberOfColumns, int &NumberOfMetadataColumns, bool &SkipEmptyLines, bool &SkipCommentLines, bool &DeleteInputFile );
    int doODPSampleLabelDialog( int &LegColumn, int &SiteColumn, int &HoleColumn, int &CoreColumn, int &CoreTypeColumn, int &SectionColumn, int &TopColumn, int &BottomColumn, bool &DeleteInputFile );
    int doTranslateCharacterEncodingDialog( int &CodecInput, int &CodecOutput, int &EOL, bool &DeleteInputFile );
    int doTransposeTableOptionsDialog( int &CodecInput, int &CodecOutput, int &EOL, bool &DeleteInputFile );

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );
    void closeEvent( QCloseEvent *event );
    void onError( const int err );

signals:
    void finishedBuildFilelist( bool );

private slots:
    int  chooseFile();
    int  chooseFiles();
    int  chooseFolder( const QString &Folder = "" );
    int  saveFile();
    int  saveFileAs();
    void hideWindow();
    void newWindow();
    void displayHelp();
    void about();
    void clearStatusMessage();

    void doDeleteCommentBlocks();
    void doDeleteDoubleLines();
    void doExtractColumns();
    void doExtractMatchedColumns();
    void doExtractLines();
    void doExtractMatchedLines();
    void doExtract10minLines();
    void doDecompressFiles();
    void doDeleteColumns();
    void doDeleteMatchedColumns();
    void doDeleteLines();
    void doDeleteMatchedLines();

    void doInsertCharactersAtPosition();
    void doReplaceCharactersAtPosition();
    void doSearchOneString();
    void doSearchAndReplaceOneString();
    void doSearchAndReplaceManyStringsAtOnce();
    void doBuildSearchAndReplaceDatabase();
    void doCompressFiles();
    void doCompressFolder();
    void doConcatenateFilesByColumns();
    void doConcatenateFilesByLines();
    void doRecalcColumns();
    void doCreateScript();
    void doRenameFiles();
    void doSplitFilesByColumns();
    void doSplitFilesByLines();
    void doSplitLargeFiles();
    void doAddColumn();
    void doAddLine();
    void doAddBlock();
    void doSaveFilelist();
    void doTranslateCharacterEncoding();
    void doTransposeTable();

    void doCalcDepthFromPressure();
    void doCalcSalinityFromCondTempPress();
    void doCreateDateTime();
    void doCreateODPSampleLabel();
    void doTable2ColumnsFormat();
    void doColumns2TableFormat();
    void doFindArea();
    void doFindPolygonCentroid();
    void doBuildAreaDatabase();
    void doCheckTimeSeries();

// Dialogs
    void doGetDatasets();
    int  doGeneralOptionsDialog();

private:
    QStringList expandCommandline();
    QStringList renameFiles( const QStringList Filename, const QString &searchStr, const QString &replaceStr );
    bool buildFilename( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QString &Filename, QString &FilenameIn, QString &FilenameOut );
    bool containsBinaryFile( const QStringList FilenameList );
    bool existsFirstFile( const int ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList );
    bool isEmptyLine( const QString &String );
    int NumOfSections( const QString &String );
    int addToFilenameList( QStringList &FilenameList, const QString &Filename, const bool showAllFiles = true, const bool decompressFile = false );
    int calcFileSizeClass( const QString &FilenameIn, const int NumOfFiles, const int lowerLimit = 100, const int upperLimit = 1000 );
    int copyStringList( const QStringList InStringList, QStringList &OutStringList, const QString &s_Pattern = "*" );
    int emptyDir( const QString &Dir );
    int decompressFile( const QString &Filename, const bool createNewDir = false, const bool delZipFile = false );
    int incFileProgress( const int NumOfFiles, const int FileNumber );
    int incProgress( const int NumOfFiles, const int Step );
    int readFile( const QString &FilenameIn, QStringList &Input, const int Codec = -1, const int NumOfFiles = 0, const qint64 Bytes = 0 );
    int removeFile( const QString &Filename );
    int downloadFile( const QString &Url, const QString &absoluteFilePath );
    unsigned int incProgress( const int NumOfFiles, const unsigned int filesize, const unsigned int length, const QString &InputStr );
    void appendItem( QStringList &List, const QString &Item, const QString &SS = "", const QString &RS = "" );
    void clearFilenameList( int &ActionNumber, QStringList &FilenameList );
    void clearList( QStringList& List );
    void clearMessage();
    void createActions();
    void createMenus();
    void createStatusBar( const bool showProgressBar );
    void enableMenuItems( const QStringList FilenameList );
    void endTool( const int err, const int stopProgress, int &ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList, const QString &doneMessage = "Done", const QString &canceledMessage = "Converter was canceled", const bool clearList = false, const bool incActionNumber = true );
    void initFileProgress( const int NumOfFiles, const QString &FilenameIn, const QString &MessageText );
    void initProgress( const int NumOfFiles, const QString &Filename, const QString &MessageText, const int totalNumberOfSteps );
    void initProgress( const int totalNumberOfSteps );
    void listDir( QStringList &Filename, const QString &Directory, const bool showAllFiles = true, const bool decompressFile = false );
    void loadPreferences();
    void resetFileProgress( const int NumOfFiles );
    void resetProgress( const int NumOfFiles = 0 );
    void savePreferences();
    void setNormalCursor();
    void setStatusBar( const QString &Message = "", const int seconds = 0 );
    void setStatusBarFileInProgress( const QString &Filename, const QString &Message = "", const int i_seconds = 0 );
    void setWTitle( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList FilenameList );
    void setWaitCursor();
    void showFilenameList( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList FilenameList );
    void showList( const QStringList List );
    void showMessage( const QString &Message, QStringList &MessageList );
    void wait( const int msec );

    QString getApplicationName( const bool replaceUnderline = false );
    QString getDataLocation();
    QString getDocumentDir();
    QString getPreferenceFilename();
    QString getVersion();
    QString setEOLChar( const int EOL );
    QString setExtension( const int Extension );

    QMenu   *fileMenu;
    QMenu   *basicToolsMenu;
    QMenu   *specialToolsMenu;
    QMenu   *helpMenu;

    QAction *newWindowAction;
    QAction *openFileAction;
    QAction *openFolderAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *hideWindowAction;
    QAction *setOptionsAction;
    QAction *exitAction;
    QAction *getFilesAction;

    QAction *addColumnAction;
    QAction *addLineAction;
    QAction *addBlockAction;
    QAction *deleteCommentBlocksAction;
    QAction *deleteDoubleLinesAction;
    QAction *extractColumnsAction;
    QAction *extractMatchedColumnsAction;
    QAction *extractLinesAction;
    QAction *extractMatchedLinesAction;
    QAction *extract10minLinesAction;
    QAction *findAreaAction;
    QAction *deleteColumnsAction;
    QAction *deleteMatchedColumnsAction;
    QAction *deleteLinesAction;
    QAction *deleteMatchedLinesAction;
    QAction *insertCharactersAtPositionAction;
    QAction *replaceCharactersAtPositionAction;
    QAction *compressFilesAction;
    QAction *compressFolderAction;
    QAction *decompressFilesAction;
    QAction *createScriptAction;
    QAction *concatenateFilesByLinesAction;
    QAction *concatenateFilesByColumnsAction;
    QAction *recalcColumnsAction;
    QAction *renameFilesAction;
    QAction *splitFilesByColumnsAction;
    QAction *splitFilesByLinesAction;
    QAction *splitLargeFilesAction;
    QAction *saveFilelistAction;
    QAction *transposeTableAction;
    QAction *searchOneStringAction;
    QAction *searchAndReplaceOneStringAction;
    QAction *searchAndReplaceManyStringsAtOnceAction;
    QAction *buildSearchAndReplaceDatabaseAction;
    QAction *buildAreaDatabaseAction;
    QAction *calcDepthAction;
    QAction *calcSalinityAction;
    QAction *checkTimeSeriesAction;
    QAction *createDateTimeAction;
    QAction *createODPSampleLabelAction;
    QAction *findPolygonCentroidAction;
    QAction *Table2ColumnsFormatAction;
    QAction *Columns2TableFormatAction;
    QAction *translateCharacterEncodingAction;

    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *showHelpAction;

    QProgressDialog *ProgressDialog;
    QProgressDialog *FileProgressDialog;
    QProgressBar    *ProgressBar;
    QLabel          *StatusMessage;

    QStringListModel    *Model;
    QListView           *ListView;
    QTextEdit           *textViewer;

    QPoint	posWindow;              //!< Position des Programmfensters.
    QSize	sizeWindow;             //!< Groesse des Programmfensters.

    QPoint	posDialog;              //!< Position der Dialoge.
    QSize	sizeParameterDialog;    //!< Groesse der Parameterdialogs.
};

#endif
