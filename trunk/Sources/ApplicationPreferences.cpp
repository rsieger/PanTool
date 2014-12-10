/****************************************************************
**
** Preferences
** 2008-12-01, Dr. Rainer Sieger
** last change: 2009-12-01
**
****************************************************************/

#include "Application.h"

const QString PrefDate = "PanTool, 2012-03-30";

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 11.3.2004

/*! @brief Sichern der Programmeinstellungen */

void MainWindow::savePreferences()
{
    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );

    settings.beginGroup( "Window" );
    settings.setValue( "WindowX", x() );
    settings.setValue( "WindowY", y() );
    settings.setValue( "WindowHeight", height() );
    settings.setValue( "WindowWidth", width() );
    settings.endGroup();

    settings.beginGroup( "Dialog" );
    settings.setValue( "DialogX", posDialog.x() );
    settings.setValue( "DialogY", posDialog.y() );
    settings.endGroup();

    settings.beginGroup( "ParameterDialog" );
    settings.setValue( "DialogHeight", sizeParameterDialog.height() );
    settings.setValue( "DialogWidth", sizeParameterDialog.width() );
    settings.endGroup();

    // General settings
    settings.beginGroup( "GeneralSettings" );
    settings.setValue( "NumOfProgramStarts", gi_NumOfProgramStarts );
    settings.setValue( "Path", gs_Path );
    settings.setValue( "FileFormat", gs_FilenameFormat );
    settings.setValue( "CodecInput", gi_CodecInput );
    settings.setValue( "CodecOutput", gi_CodecOutput );
    settings.setValue( "CodecDownload", gi_CodecDownload );
    settings.setValue( "EOL", gi_EOL );
    settings.setValue( "Extension", gi_Extension );
    settings.endGroup();

    // PanGet
    settings.beginGroup( "PanGet" );
    settings.setValue( "IDListFile", gs_IDListFile );
    settings.setValue( "DownloadDirectory", gs_DownloadDirectory );
    settings.setValue( "DownloadXML", gb_DownloadXML );
    settings.endGroup();

    // extract columns
    settings.beginGroup( "ExtractColumns" );
    settings.setValue( "List", gs_ec_ExtractColumnsList );
    settings.setValue( "SkipEmptyLines", gb_ec_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_ec_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_ec_DeleteInputFile );
    settings.endGroup();

    // extract matched columns
    settings.beginGroup( "ExtractMatchedColumns" );
    settings.setValue( "SearchString", gs_emc_ExtractColumnsSearchString );
    settings.setValue( "SaveNoMatch", gb_emc_SaveNoMatch );
    settings.setValue( "SkipEmptyLines", gb_emc_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_emc_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_emc_DeleteInputFile );
    settings.setValue( "DeleteEmptyOutputFile", gb_emc_DeleteEmptyOutputFile );
    settings.endGroup();

    // extract lines
    settings.beginGroup( "ExtractLines" );
    settings.setValue( "StartLine", gi_el_StartLine );
    settings.setValue( "Increment", gi_el_Increment );
    settings.setValue( "NumberOfLines", gi_el_NumberOfLines );
    settings.setValue( "FirstLineLastLine", gb_el_ExtractFristLineLastLine );
    settings.setValue( "SaveNoMatch", gb_el_SaveNoMatch );
    settings.setValue( "SkipEmptyLines", gb_el_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_el_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_el_DeleteInputFile );
    settings.setValue( "DeleteEmptyOutputFile", gb_el_DeleteEmptyOutputFile );
    settings.endGroup();

    // extract matched lines
    settings.beginGroup( "ExtractMatchedLines" );
    settings.setValue( "SearchString", gs_eml_ExtractLinesSearchString );
    settings.setValue( "StartLine", gi_eml_StartLine );
    settings.setValue( "NumberOfLines", gi_eml_NumberOfLines );
    settings.setValue( "SaveFirstLine", gb_eml_SaveFirstLine );
    settings.setValue( "SaveNoMatch", gb_eml_SaveNoMatch );
    settings.setValue( "SkipEmptyLines", gb_eml_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_eml_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_eml_DeleteInputFile );
    settings.setValue( "DeleteEmptyOutputFile", gb_eml_DeleteEmptyOutputFile );
    settings.endGroup();

    settings.beginGroup( "Extract10minLines" );
    settings.setValue( "SaveHeader", gb_e10minl_SaveHeader );
    settings.setValue( "DeleteInputFile", gb_e10minl_DeleteInputFile );
    settings.endGroup();

    // delete columns
    settings.beginGroup( "DeleteColumns" );
    settings.setValue( "List", gs_dc_DeleteColumnsList );
    settings.setValue( "SkipEmptyLines", gb_dc_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_dc_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_dc_DeleteInputFile );
    settings.endGroup();

    // delete matched columns
    settings.beginGroup( "DeleteMatchedColumns" );
    settings.setValue( "SearchString", gs_dmc_DeleteColumnsSearchString );
    settings.setValue( "SkipEmptyLines", gb_dmc_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_dmc_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_dmc_DeleteInputFile );
    settings.endGroup();

    // delete lines
    settings.beginGroup( "DeleteLines" );
    settings.setValue( "StartLine", gi_dl_StartLine );
    settings.setValue( "NumberOfLines", gi_dl_NumberOfLines );
    settings.setValue( "SkipEmptyLines", gb_dl_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_dl_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_dl_DeleteInputFile );
    settings.endGroup();

    // delete matched lines
    settings.beginGroup( "DeleteMatchedLines" );
    settings.setValue( "SearchString", gs_dml_DeleteLinesSearchString );
    settings.setValue( "SkipEmptyLines", gb_dml_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_dml_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_dml_DeleteInputFile );
    settings.endGroup();

    // delete double lines
    settings.beginGroup( "DeleteDoubleLines" );
    settings.setValue( "SkipEmptyLines", gb_ddl_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_ddl_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_ddl_DeleteInputFile );
    settings.endGroup();

    // delete comment blocks
    settings.beginGroup( "DeleteCommentBlocks" );
    settings.setValue( "SkipEmptyLines", gb_dcl_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_dcl_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_dcl_DeleteInputFile );
    settings.endGroup();

    // insert characters at position
    settings.beginGroup( "InsertChar" );
    settings.setValue( "List", gs_ic_InsertPositionList );
    settings.setValue( "InsertString", gs_ic_InsertStr );
    settings.setValue( "SkipEmptyLines", gb_ic_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_ic_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_ic_DeleteInputFile );
    settings.endGroup();

    // replace characters at position
    settings.beginGroup( "ReplaceChar" );
    settings.setValue( "List", gs_rc_ReplacePositionList );
    settings.setValue( "ReplaceString", gs_rc_ReplaceStr );
    settings.setValue( "SkipEmptyLines", gb_rc_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_rc_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_rc_DeleteInputFile );
    settings.endGroup();

    // search one string
    settings.beginGroup( "SearchString" );
    settings.setValue( "SearchString", gs_sos_SearchString );
    settings.setValue( "StartLine", gi_sos_StartLine );
    settings.setValue( "NumberOfLines", gi_sos_NumberOfLines );
    settings.setValue( "SkipEmptyLines", gb_sos_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_sos_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_sos_DeleteInputFile );
    settings.setValue( "DeleteEmptyOutputFile", gb_sos_DeleteEmptyOutputFile );
    settings.endGroup();

    // search and replace one string
    settings.beginGroup( "SearchReplaceString" );
    settings.setValue( "SearchString", gs_srs_SearchString );
    settings.setValue( "ReplaceString", gs_srs_ReplaceString );
    settings.setValue( "StartLine", gi_srs_StartLine );
    settings.setValue( "NumberOfLines", gi_srs_NumberOfLines );
    settings.setValue( "SkipEmptyLines", gb_srs_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_srs_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_srs_DeleteInputFile );
    settings.endGroup();

    // search and replace many strings at once
    settings.beginGroup( "SearchReplaceManyStrings" );
    settings.setValue( "DatabaseName", gs_srm_DatabaseName );
    settings.setValue( "Mode", gi_srm_SearchAndReplaceMode );
    settings.setValue( "StartLine", gi_srm_StartLine );
    settings.setValue( "NumberOfLines", gi_srm_NumberOfLines );
    settings.setValue( "SkipEmptyLines", gb_srm_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_srm_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_srm_DeleteInputFile );
    settings.endGroup();

    // concatenate files by columns
    settings.beginGroup( "ConcatenateColumns" );
    settings.setValue( "SkipNStartLines", gi_cfc_SkipNStartLines );
    settings.setValue( "IncludeFilename", gb_cfc_IncludeFilename );
    settings.setValue( "SkipCommentLines", gb_cfc_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_cfc_DeleteInputFile );
    settings.endGroup();

    // concatenate files by lines
    settings.beginGroup( "ConcatenateLines" );
    settings.setValue( "SkipNStartLines", gi_cfl_SkipNStartLines );
    settings.setValue( "IncludeFilename", gb_cfl_IncludeFilename );
    settings.setValue( "SkipEmptyLines", gb_cfl_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_cfl_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_cfl_DeleteInputFile );
    settings.endGroup();

    // split files by columns
    settings.beginGroup( "SplitColumns" );
    settings.setValue( "NumberOfColumns", gi_sfc_NumberOfColumns );
    settings.setValue( "NumberOfMeatadataColumns", gi_sfc_NumberOfMetadataColumns );
    settings.setValue( "DeleteInputFile", gb_sfc_DeleteInputFile );
    settings.endGroup();

    // concatenate files by lines
    settings.beginGroup( "SplitLines" );
    settings.setValue( "NumberOfLines", gi_sfl_NumberOfLines );
    settings.setValue( "NumberOfHeaderLines", gi_sfl_NumberOfHeaderLines );
    settings.setValue( "SkipEmptyLines", gb_sfl_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_sfl_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_sfl_DeleteInputFile );
    settings.endGroup();

    // add line
    settings.beginGroup( "AddLine" );
    settings.setValue( "Text", gs_al_Text );
    settings.setValue( "LineNo", gi_al_LineNo );
    settings.setValue( "AddFilename", gb_al_AddFilename );
    settings.setValue( "AddFullPath", gb_al_AddFullPath );
    settings.setValue( "AddOrdinalNumber", gb_al_AddOrdinalNumber );
    settings.setValue( "SkipEmptyLines", gb_al_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_al_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_al_DeleteInputFile );
    settings.endGroup();

    // add block
    settings.beginGroup( "AddBlock" );
    settings.setValue( "Text", gs_ab_Text );
    settings.setValue( "LineNo", gi_ab_LineNo );
    settings.setValue( "AddFilename", gb_ab_AddFilename );
    settings.setValue( "AddFullPath", gb_ab_AddFullPath );
    settings.setValue( "AddOrdinalNumber", gb_ab_AddOrdinalNumber );
    settings.setValue( "SkipEmptyLines", gb_ab_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_ab_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_ab_DeleteInputFile );
    settings.endGroup();

    // add column
    settings.beginGroup( "AddColumn" );
    settings.setValue( "HeaderText", gs_ac_HeaderText );
    settings.setValue( "ColumnText", gs_ac_ColumnText );
    settings.setValue( "AddFilename", gb_ac_AddFilename );
    settings.setValue( "AddFullPath", gb_ac_AddFullPath );
    settings.setValue( "AddOrdinalNumber", gb_ac_AddOrdinalNumber );
    settings.setValue( "SkipEmptyLines", gb_ac_SkipEmptyLines );
    settings.setValue( "SkipCommentLines", gb_ac_SkipCommentLines );
    settings.setValue( "DeleteInputFile", gb_ac_DeleteInputFile );
    settings.endGroup();

    // recalc columns
    settings.beginGroup( "RecalcColumns" );
    settings.setValue( "List", gs_rec_RecalcColumnsList );
    settings.setValue( "MulX", gf_rec_MulX );
    settings.setValue( "AddX", gf_rec_AddX );
    settings.setValue( "NumberOfDigits", gi_rec_NumOfDigits );
    settings.setValue( "DeleteInputFile", gb_rec_DeleteInputFile );
    settings.endGroup();

    // find area
    settings.beginGroup( "FindArea" );
    settings.setValue( "AreaDatabaseFilename", gs_AreaDatabaseFilename );
    settings.setValue( "DeleteInputFile", gb_fa_DeleteInputFile );
    settings.endGroup();

    // table to columns
    settings.beginGroup( "TableToColumns" );
    settings.setValue( "NumberOfGeocodes", gi_tcf_NumOfGeocodes );
    settings.setValue( "DeleteInputFile", gb_tcf_DeleteInputFile );
    settings.endGroup();

    // columns to table
    settings.beginGroup( "ColumnsToTable" );
    settings.setValue( "MissingValue", gs_cft_MissingValue );
    settings.setValue( "FieldDelimiter", gi_cft_FieldDelimiter );
    settings.setValue( "DeleteInputFile", gb_cft_DeleteInputFile );
    settings.endGroup();

    // date/time
    settings.beginGroup( "DateTimeConverter" );
    settings.setValue( "DateColumn", gi_dt_DateColumn );
    settings.setValue( "YearColumn", gi_dt_YearColumn );
    settings.setValue( "MonthColumn", gi_dt_MonthColumn );
    settings.setValue( "DayColumn", gi_dt_DayColumn );
    settings.setValue( "TimeColumn", gi_dt_TimeColumn );
    settings.setValue( "HourColumn", gi_dt_HourColumn );
    settings.setValue( "MinuteColumn", gi_dt_MinuteColumn );
    settings.setValue( "SecondColumn", gi_dt_SecondColumn );
    settings.setValue( "DateTimeColumn", gi_dt_DateTimeColumn );
    settings.setValue( "DayOfYearColumn", gi_dt_DayOfYearColumn );
    settings.setValue( "JulianDayColumn", gi_dt_JulianDayColumn );
    settings.setValue( "WriteDateTimeOnly", gb_dt_WriteDateTimeOnly );
    settings.endGroup();

    // Pressure to depth
    settings.beginGroup( "PressureToDepth" );
    settings.setValue( "LatitudeColumn", gi_pd_LatitudeColumn );
    settings.setValue( "LongitudeColumn", gi_pd_LongitudeColumn );
    settings.setValue( "PressureColumn", gi_pd_PressureColumn );
    settings.setValue( "NumberOfDigits", gi_pd_NumOfDigits );
    settings.setValue( "DeleteInputFile", gb_pd_DeleteInputFile );
    settings.endGroup();

    // ODP Sample Label
    settings.beginGroup( "ODPSampleLabel" );
    settings.setValue( "LegColumn", gi_odp_LegColumn );
    settings.setValue( "SiteColumn", gi_odp_SiteColumn );
    settings.setValue( "HoleColumn", gi_odp_HoleColumn );
    settings.setValue( "CoreColumn", gi_odp_CoreColumn );
    settings.setValue( "TypeColumn", gi_odp_CoreTypeColumn );
    settings.setValue( "SectionColumn", gi_odp_SectionColumn );
    settings.setValue( "TopColumn", gi_odp_TopColumn );
    settings.setValue( "BottomColumn", gi_odp_BottomColumn );
    settings.setValue( "DeleteInputFile", gb_odp_DeleteInputFile );
    settings.endGroup();

    // Translate character encoding
    settings.beginGroup( "TranslateCharcterEncoding" );
    settings.setValue( "CodecInput", gi_tce_CodecInput );
    settings.setValue( "CodecOutput", gi_tce_CodecOutput );
    settings.setValue( "EOL", gi_tce_EOL );
    settings.setValue( "DeleteInputFile", gb_tce_DeleteInputFile );
    settings.endGroup();

    // Rename files
    settings.beginGroup( "RenameFile" );
    settings.setValue( "SearchString", gs_rf_SearchString );
    settings.setValue( "ReplaceString;", gs_rf_ReplaceString );
    settings.endGroup();

    // Transpose table
    settings.beginGroup( "TransposeTable" );
    settings.setValue( "CodecInput", gi_tt_CodecInput );
    settings.setValue( "CodecOutput", gi_tt_CodecOutput );
    settings.setValue( "EOL", gi_tt_EOL );
    settings.setValue( "DeleteInputFile", gb_tt_DeleteInputFile );
    settings.endGroup();

    settings.endGroup();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 21.11.2007

/*! @brief Laden der Programmeinstellungen */

void MainWindow::loadPreferences()
{
    #if defined(Q_OS_LINUX)
        gi_CodecInput                       = _UTF8_;               //!< UTF-8
        gi_CodecOutput                      = _UTF8_;
        gi_CodecDownload                    = _UTF8_;
        gi_EOL                              = _UNIX_;
    #endif

    #if defined(Q_OS_MAC)
        gi_CodecInput                       = _APPLEROMAN_;        //!< Apple Roman
        gi_CodecOutput                      = _APPLEROMAN_;
        gi_CodecDownload                    = _APPLEROMAN_;
        gi_EOL                              = _UNIX_;
    #endif

    #if defined(Q_OS_WIN)
        gi_CodecInput                       = _LATIN1_;            //!< Latin-1 = ISO 8859-1
        gi_CodecOutput                      = _LATIN1_;
        gi_CodecDownload                    = _LATIN1_;
        gi_EOL                              = _WIN_;
    #endif

    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );

    settings.beginGroup( "Window" );
    posWindow.setX( settings.value( "WindowX", 100 ).toInt() );
    posWindow.setY( settings.value( "WindowY", 100 ).toInt() );
    sizeWindow.setHeight( settings.value( "WindowHeight", 250 ).toInt() );
    sizeWindow.setWidth( settings.value( "WindowWidth", 600 ).toInt() );
    settings.endGroup();

    settings.beginGroup( "Dialog" );
    posDialog.setX( settings.value( "DialogX", 110 ).toInt() );
    posDialog.setY( settings.value( "DialogY", 110 ).toInt() );
    settings.endGroup();

    settings.beginGroup( "ParameterDialog" );
    sizeParameterDialog.setHeight( settings.value( "DialogHeight", 100 ).toInt() );
    sizeParameterDialog.setWidth( settings.value( "DialogWidth", 100 ).toInt() );
    settings.endGroup();

    // General settings
    settings.beginGroup( "GeneralSettings" );
    gi_NumOfProgramStarts = settings.value( "NumOfProgramStarts", 0 ).toInt();
    gs_Path               = settings.value( "Path", "" ).toString();
    gs_FilenameFormat     = settings.value( "FileFormat", "zz%a_%N" ).toString();
    gi_CodecInput         = settings.value( "CodecInput", gi_CodecInput ).toInt();
    gi_CodecOutput        = settings.value( "CodecOutput", gi_CodecOutput ).toInt();
    gi_CodecDownload      = settings.value( "CodecDownload", gi_CodecDownload ).toInt();
    gi_EOL                = settings.value( "EOL", gi_EOL ).toInt();
    gi_Extension          = settings.value( "Extension", _TXT_ ).toInt();
    settings.endGroup();

    // PanGet
    settings.beginGroup( "PanGet" );
    gs_IDListFile        = settings.value( "IDListFile", "" ).toString();
    gs_DownloadDirectory = settings.value( "DownloadDirectory", "" ).toString();
    gb_DownloadXML       = settings.value( "DownloadXML", false ).toBool();
    settings.endGroup();

    // extract columns
    settings.beginGroup( "ExtractColumns" );
    gs_ec_ExtractColumnsList    = settings.value( "List", "1,4-7,10-end" ).toString();
    gb_ec_SkipEmptyLines        = settings.value( "SkipEmptyLines", false ).toBool();
    gb_ec_SkipCommentLines      = settings.value( "SkipCommentLines", false ).toBool();
    gb_ec_DeleteInputFile       = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // extract matched columns
    settings.beginGroup( "ExtractMatchedColumns" );
    gs_emc_ExtractColumnsSearchString   = settings.value( "SearchString", "PANGAEA" ).toString();
    gb_emc_SaveNoMatch                  = settings.value( "SaveNoMatch", false ).toBool();
    gb_emc_SkipEmptyLines               = settings.value( "SkipEmptyLines", false ).toBool();
    gb_emc_SkipCommentLines             = settings.value( "SkipCommentLines", false ).toBool();
    gb_emc_DeleteInputFile              = settings.value( "DeleteInputFile", false ).toBool();
    gb_emc_DeleteEmptyOutputFile        = settings.value( "DeleteEmptyOutputFile", true ).toBool();
    settings.endGroup();

    // extract lines
    settings.beginGroup( "ExtractLines" );
    gi_el_StartLine                 = settings.value( "StartLine", 1 ).toInt();
    gi_el_Increment                 = settings.value( "Increment", 1 ).toInt();
    gi_el_NumberOfLines             = settings.value( "NumberOfLines", 0 ).toInt();
    gb_el_ExtractFristLineLastLine  = settings.value( "FirstLineLastLine", false ).toBool();
    gb_el_SaveNoMatch               = settings.value( "SaveNoMatch", false ).toBool();
    gb_el_SkipEmptyLines            = settings.value( "SkipEmptyLines", false ).toBool();
    gb_el_SkipCommentLines          = settings.value( "SkipCommentLines", false ).toBool();
    gb_el_DeleteInputFile           = settings.value( "DeleteInputFile", false ).toBool();
    gb_el_DeleteEmptyOutputFile     = settings.value( "DeleteEmptyOutputFile", true ).toBool();
    settings.endGroup();

    // extract matched lines
    settings.beginGroup( "ExtractMatchedLines" );
    gs_eml_ExtractLinesSearchString = settings.value( "SearchString", "PANGAEA" ).toString();
    gi_eml_StartLine                = settings.value( "StartLine", 1 ).toInt();
    gi_eml_NumberOfLines            = settings.value( "NumberOfLines", 0 ).toInt();
    gb_eml_SaveFirstLine            = settings.value( "SaveFirstLine", true ).toBool();
    gb_eml_SaveNoMatch              = settings.value( "SaveNoMatch", false ).toBool();
    gb_eml_SkipEmptyLines           = settings.value( "SkipEmptyLines", false ).toBool();
    gb_eml_SkipCommentLines         = settings.value( "SkipCommentLines", false ).toBool();
    gb_eml_DeleteInputFile          = settings.value( "DeleteInputFile", false ).toBool();
    gb_eml_DeleteEmptyOutputFile    = settings.value( "DeleteEmptyOutputFile", true ).toBool();
    settings.endGroup();

    settings.beginGroup( "Extract10minLines" );
    gb_e10minl_SaveHeader       = settings.value( "SaveHeader", true ).toBool();
    gb_e10minl_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // delete columns
    settings.beginGroup( "DeleteColumns" );
    gs_dc_DeleteColumnsList = settings.value( "List", "1,4-7,10-end" ).toString();
    gb_dc_SkipEmptyLines    = settings.value( "SkipEmptyLines", false ).toBool();
    gb_dc_SkipCommentLines  = settings.value( "SkipCommentLines", false ).toBool();
    gb_dc_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // delete matched columns
    settings.beginGroup( "DeleteMatchedColumns" );
    gs_dmc_DeleteColumnsSearchString = settings.value( "SearchString", "PANGAEA" ).toString();
    gb_dmc_SkipEmptyLines            = settings.value( "SkipEmptyLines", false ).toBool();
    gb_dmc_SkipCommentLines          = settings.value( "SkipCommentLines", false ).toBool();
    gb_dmc_DeleteInputFile           = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // delete lines
    settings.beginGroup( "DeleteLines" );
    gi_dl_StartLine         = settings.value( "StartLine", 1 ).toInt();
    gi_dl_NumberOfLines     = settings.value( "NumberOfLines", 0 ).toInt();
    gb_dl_SkipEmptyLines    = settings.value( "SkipEmptyLines", false ).toBool();
    gb_dl_SkipCommentLines  = settings.value( "SkipCommentLines", false ).toBool();
    gb_dl_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // delete matched lines
    settings.beginGroup( "DeleteMatchedLines" );
    gs_dml_DeleteLinesSearchString = settings.value( "SearchString", "PANGAEA" ).toString();
    gb_dml_SkipEmptyLines          = settings.value( "SkipEmptyLines", false ).toBool();
    gb_dml_SkipCommentLines        = settings.value( "SkipCommentLines", false ).toBool();
    gb_dml_DeleteInputFile         = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // delete double lines
    settings.beginGroup( "DeleteDoubleLines" );
    gb_ddl_SkipEmptyLines   = settings.value( "SkipEmptyLines", false ).toBool();
    gb_ddl_SkipCommentLines = settings.value( "SkipCommentLines", false ).toBool();
    gb_ddl_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // delete comment blocks
    settings.beginGroup( "DeleteCommentBlocks" );
    gb_dcl_SkipEmptyLines   = settings.value( "SkipEmptyLines", false ).toBool();
    gb_dcl_SkipCommentLines = settings.value( "SkipCommentLines", false ).toBool();
    gb_dcl_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // insert characters at position
    settings.beginGroup( "InsertChar" );
    gs_ic_InsertPositionList = settings.value( "List", "10,15,43" ).toString();
    gs_ic_InsertStr          = settings.value( "InsertString", "^t" ).toString();
    gb_ic_SkipEmptyLines     = settings.value( "SkipEmptyLines", false ).toBool();
    gb_ic_SkipCommentLines   = settings.value( "SkipCommentLines", false ).toBool();
    gb_ic_DeleteInputFile    = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // replace characters at position
    settings.beginGroup( "ReplaceChar" );
    gs_rc_ReplacePositionList   = settings.value( "List", "10,15,43" ).toString();
    gs_rc_ReplaceStr            = settings.value( "ReplaceString", "^t" ).toString();
    gb_rc_SkipEmptyLines        = settings.value( "SkipEmptyLines", false ).toBool();
    gb_rc_SkipCommentLines      = settings.value( "SkipCommentLines", false ).toBool();
    gb_rc_DeleteInputFile       = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // search one string
    settings.beginGroup( "SearchString" );
    gs_sos_SearchString          = settings.value( "SearchString", "PANGAEA" ).toString();
    gi_sos_StartLine             = settings.value( "StartLine", 1 ).toInt();
    gi_sos_NumberOfLines         = settings.value( "NumberOfLines", 0 ).toInt();
    gb_sos_SkipEmptyLines        = settings.value( "SkipEmptyLines", false ).toBool();
    gb_sos_SkipCommentLines      = settings.value( "SkipCommentLines", false ).toBool();
    gb_sos_DeleteInputFile       = settings.value( "DeleteInputFile", false ).toBool();
    gb_sos_DeleteEmptyOutputFile = settings.value( "DeleteEmptyOutputFile", false ).toBool();
    settings.endGroup();

    // search and replace one string
    settings.beginGroup( "SearchReplaceString" );
    gs_srs_SearchString     = settings.value( "SearchString", "Search for" ).toString();
    gs_srs_ReplaceString    = settings.value( "ReplaceString", "Replace with" ).toString();
    gi_srs_StartLine        = settings.value( "StartLine", 1 ).toInt();
    gi_srs_NumberOfLines    = settings.value( "NumberOfLines", 0 ).toInt();
    gb_srs_SkipEmptyLines   = settings.value( "SkipEmptyLines", false ).toBool();
    gb_srs_SkipCommentLines = settings.value( "SkipCommentLines", false ).toBool();
    gb_srs_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // search and replace many strings at once
    settings.beginGroup( "SearchReplaceManyStrings" );
    gs_srm_DatabaseName         = settings.value( "DatabaseName", "" ).toString();
    gi_srm_SearchAndReplaceMode = settings.value( "Mode", _LINE ).toInt();
    gi_srm_StartLine            = settings.value( "StartLine", 1 ).toInt();
    gi_srm_NumberOfLines        = settings.value( "NumberOfLines", 0 ).toInt();
    gb_srm_SkipEmptyLines       = settings.value( "SkipEmptyLines", false ).toBool();
    gb_srm_SkipCommentLines     = settings.value( "SkipCommentLines", false ).toBool();
    gb_srm_DeleteInputFile      = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // concatenate files by columns
    settings.beginGroup( "ConcatenateColumns" );
    gi_cfc_SkipNStartLines  = settings.value( "SkipNStartLines", 0 ).toInt();
    gb_cfc_IncludeFilename  = settings.value( "IncludeFilename", false ).toBool();
    gb_cfc_SkipCommentLines = settings.value( "SkipCommentLines", false ).toBool();
    gb_cfc_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // concatenate files by lines
    settings.beginGroup( "ConcatenateLines" );
    gi_cfl_SkipNStartLines  = settings.value( "SkipNStartLines", 0 ).toInt();
    gb_cfl_IncludeFilename  = settings.value( "IncludeFilename", false ).toBool();
    gb_cfl_SkipEmptyLines   = settings.value( "SkipEmptyLines", false ).toBool();
    gb_cfl_SkipCommentLines = settings.value( "SkipCommentLines", false ).toBool();
    gb_cfl_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // split files by columns
    settings.beginGroup( "SplitColumns" );
    gi_sfc_NumberOfColumns         = settings.value( "NumberOfColumns", 0 ).toInt();
    gi_sfc_NumberOfMetadataColumns = settings.value( "NumberOfMeatadataColumns", 0 ).toInt();
    gb_sfc_DeleteInputFile         = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // concatenate files by lines
    settings.beginGroup( "SplitLines" );
    gi_sfl_NumberOfLines        = settings.value( "NumberOfLines", 0 ).toInt();
    gi_sfl_NumberOfHeaderLines  = settings.value( "NumberOfHeaderLines", 0 ).toInt();
    gb_sfl_SkipEmptyLines       = settings.value( "SkipEmptyLines", false ).toBool();
    gb_sfl_SkipCommentLines     = settings.value( "SkipCommentLines", false ).toBool();
    gb_sfl_DeleteInputFile      = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // add line
    settings.beginGroup( "AddLine" );
    gs_al_Text              = settings.value( "Text", "PANGAEA" ).toString();
    gi_al_LineNo            = settings.value( "LineNo", 1 ).toInt();
    gb_al_AddFilename       = settings.value( "AddFilename", false ).toBool();
    gb_al_AddFullPath       = settings.value( "AddFullPath", false ).toBool();
    gb_al_AddOrdinalNumber  = settings.value( "AddOrdinalNumber", false ).toBool();
    gb_al_SkipEmptyLines    = settings.value( "SkipEmptyLines", false ).toBool();
    gb_al_SkipCommentLines  = settings.value( "SkipCommentLines", false ).toBool();
    gb_al_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // add block
    settings.beginGroup( "AddBlock" );
    gs_ab_Text              = settings.value( "Text", "PANGAEA" ).toString();
    gi_ab_LineNo            = settings.value( "LineNo", 1 ).toInt();
    gb_ab_AddFilename       = settings.value( "AddFilename", false ).toBool();
    gb_ab_AddFullPath       = settings.value( "AddFullPath", false ).toBool();
    gb_ab_AddOrdinalNumber  = settings.value( "AddOrdinalNumber", false ).toBool();
    gb_ab_SkipEmptyLines    = settings.value( "SkipEmptyLines", false ).toBool();
    gb_ab_SkipCommentLines  = settings.value( "SkipCommentLines", false ).toBool();
    gb_ab_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // add column
    settings.beginGroup( "AddColumn" );
    gs_ac_HeaderText        = settings.value( "HeaderText", "Event label" ).toString();
    gs_ac_ColumnText        = settings.value( "ColumnText", "PS77/123-2" ).toString();
    gb_ac_AddFilename       = settings.value( "AddFilename", false ).toBool();
    gb_ac_AddFullPath       = settings.value( "AddFullPath", false ).toBool();
    gb_ac_AddOrdinalNumber  = settings.value( "AddOrdinalNumber", false ).toBool();
    gb_ac_SkipEmptyLines    = settings.value( "SkipEmptyLines", false ).toBool();
    gb_ac_SkipCommentLines  = settings.value( "SkipCommentLines", false ).toBool();
    gb_ac_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // recalc columns
    settings.beginGroup( "RecalcColumns" );
    gs_rec_RecalcColumnsList = settings.value( "List", "1,4-7,10-end" ).toString();
    gf_rec_MulX              = settings.value( "MulX", 1. ).toFloat();
    gf_rec_AddX              = settings.value( "AddX", 0. ).toFloat();
    gi_rec_NumOfDigits       = settings.value( "NumberOfDigits", 5 ).toInt();
    gb_rec_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // find area
    settings.beginGroup( "FindArea" );
    gs_AreaDatabaseFilename = settings.value( "AreaDatabaseFilename", "Browse through the area database with \"Browse ...\"" ).toString();
    gb_fa_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // table to columns
    settings.beginGroup( "TableToColumns" );
    gi_tcf_NumOfGeocodes   = settings.value( "NumberOfGeocodes", 1 ).toInt();
    gb_tcf_DeleteInputFile = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // columns to table
    settings.beginGroup( "ColumnsToTable" );
    gs_cft_MissingValue    = settings.value( "MissingValue", "" ).toString();
    gi_cft_FieldDelimiter  = settings.value( "FieldDelimiter", _TABSTOP_ ).toInt();
    gb_cft_DeleteInputFile = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // date/time
    settings.beginGroup( "DateTimeConverter" );
    gi_dt_DateColumn        = settings.value( "DateColumn", 0 ).toInt();
    gi_dt_YearColumn        = settings.value( "YearColumn", 0 ).toInt();
    gi_dt_MonthColumn       = settings.value( "MonthColumn", 0 ).toInt();
    gi_dt_DayColumn         = settings.value( "DayColumn", 0 ).toInt();
    gi_dt_TimeColumn        = settings.value( "TimeColumn", 0 ).toInt();
    gi_dt_HourColumn        = settings.value( "HourColumn", 0 ).toInt();
    gi_dt_MinuteColumn      = settings.value( "MinuteColumn", 0 ).toInt();
    gi_dt_SecondColumn      = settings.value( "SecondColumn", 0 ).toInt();
    gi_dt_DateTimeColumn    = settings.value( "DateTimeColumn", 0 ).toInt();
    gi_dt_DayOfYearColumn   = settings.value( "DayOfYearColumn", 0 ).toInt();
    gi_dt_JulianDayColumn   = settings.value( "JulianDayColumn", 0 ).toInt();
    gb_dt_WriteDateTimeOnly = settings.value( "WriteDateTimeOnly", true ).toBool();
    settings.endGroup();

    // Pressure to depth
    settings.beginGroup( "PressureToDepth" );
    gi_pd_LatitudeColumn    = settings.value( "LatitudeColumn", 1 ).toInt();
    gi_pd_LongitudeColumn   = settings.value( "LongitudeColumn", 2 ).toInt();
    gi_pd_PressureColumn    = settings.value( "PressureColumn", 3 ).toInt();
    gi_pd_NumOfDigits       = settings.value( "NumberOfDigits", 2 ).toInt();
    gb_pd_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // ODP Sample Label
    settings.beginGroup( "ODPSampleLabel" );
    gi_odp_LegColumn        = settings.value( "LegColumn", 1 ).toInt();
    gi_odp_SiteColumn       = settings.value( "SiteColumn", 2 ).toInt();
    gi_odp_HoleColumn       = settings.value( "HoleColumn", 3 ).toInt();
    gi_odp_CoreColumn       = settings.value( "CoreColumn", 4 ).toInt();
    gi_odp_CoreTypeColumn   = settings.value( "TypeColumn", 5 ).toInt();
    gi_odp_SectionColumn    = settings.value( "SectionColumn", 6 ).toInt();
    gi_odp_TopColumn        = settings.value( "TopColumn", 7 ).toInt();
    gi_odp_BottomColumn     = settings.value( "BottomColumn", 8 ).toInt();
    gb_odp_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // Translate character encoding
    settings.beginGroup( "TranslateCharcterEncoding" );
    gi_tce_CodecInput       = settings.value( "CodecInput", gi_CodecInput ).toInt();
    gi_tce_CodecOutput      = settings.value( "CodecOutput", gi_CodecOutput ).toInt();
    gi_tce_EOL              = settings.value( "EOL", gi_EOL ).toInt();
    gb_tce_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // Rename files
    settings.beginGroup( "RenameFile" );
    gs_rf_SearchString      = settings.value( "SearchString", "zz1_" ).toString();
    gs_rf_ReplaceString     = settings.value( "ReplaceString;", "" ).toString();
    settings.endGroup();

    // Transpose table
    settings.beginGroup( "TransposeTable" );
    gi_tt_CodecInput        = settings.value( "CodecInput", gi_CodecInput ).toInt();
    gi_tt_CodecOutput       = settings.value( "CodecOutput", gi_CodecOutput ).toInt();
    gi_tt_EOL               = settings.value( "EOL", gi_EOL ).toInt();
    gb_tt_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    settings.endGroup();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermitteln des Dateinamens fuer die Programmeinstellungen.
*   @return Name der Datei fuer die Programmeinstellungen. Dateiname ist gleich "_ERROR_",
*   wenn Betriebssystem ungleich Windows, Mac OS oder Unix ist.
*/

QString MainWindow::getPreferenceFilename()
{
    QString s_PrefFilename = "";

    #if defined(Q_OS_LINUX)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/.config/" ) + QCoreApplication::organizationName() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".conf" );
    #endif

    #if defined(Q_OS_MAC)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/Library/Preferences/" ) + QCoreApplication::organizationDomain().section( ".", 1, 1 ) + "." + QCoreApplication::organizationDomain().section( ".", 0, 0 ) + "." + QCoreApplication::applicationName() + QLatin1String( ".plist" );
    #endif

    #if defined(Q_OS_WIN)
        if ( QCoreApplication::applicationName().toLower().endsWith( "portable" ) )
        {
            s_PrefFilename = QCoreApplication::applicationDirPath() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
        else
        {
            QSettings cfg( QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName() );
            s_PrefFilename = QFileInfo( cfg.fileName() ).absolutePath() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
    #endif

    return( s_PrefFilename );
}
