/* 2011-10-06                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::splitFileByColumns( const QString &s_FilenameIn, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_Extension, const int i_NumberOfColumns, const QString &s_FixedColumnsList, const int i_NumOfFiles )
{
    int         s                    = 0;
    int         e                    = 0;
    int         n                    = 0;

    int         err                  = _NOERROR_;

    QString     s_FilenameOut        = "";
    QString     s_ExtractColumnsList = "";

    QStringList sl_Input;

    QFileInfo   fi( s_FilenameIn );

    QList<int>  il_ColumnList;

// **********************************************************************************************

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

    n = NumOfSections( sl_Input.at( 0 ) );

// **********************************************************************************************

    il_ColumnList = scanList( _EXTRACTCOLUMNS, n, s_FixedColumnsList );

    s = il_ColumnList.last() + 2;

    for ( int i=1; i<n; i++ )
    {
        if ( ( s < n ) && ( e < n ) )
        {
            s_ExtractColumnsList = s_FixedColumnsList;

            s_FilenameOut = fi.absolutePath() + "/" + fi.completeBaseName() + QString( "_%1" ).arg( i, 4, 10, QLatin1Char( '0' ) ) + setExtension( i_Extension );

            e = s + i_NumberOfColumns - 1;

            if ( e >= n )
                e = n;

            s_ExtractColumnsList.append( QString( ",%1-%2").arg( s ).arg( e ) );

            err = extractColumns( s_FilenameIn, s_FilenameOut, i_CodecInput, i_CodecOutput, i_EOL, s_ExtractColumnsList, false, false, i_NumOfFiles );

            s = e + 1;
        }
    }

// **********************************************************************************************

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doSplitFilesByColumns()
{
    int     i                           = 0;
    int     err                         = 0;
    int     stopProgress                = 0;

    int     i_dummy_NumberOfLines       = 0;
    int     i_dummy_NumberOfHeaderLines = 0;

    bool    b_dummy_SkipEmptyLines      = 0;
    bool    b_dummy_SkipCommentLines    = 0;

    QString s_FilenameIn                = "";
    QString s_FilenameOut               = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSplitFileDialog( _SPLITBYCOLUMNS, i_dummy_NumberOfLines, i_dummy_NumberOfHeaderLines, gi_sfc_NumberOfColumns, gs_sfc_FixedColumnsList, b_dummy_SkipEmptyLines, b_dummy_SkipCommentLines, gb_sfc_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Split files..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = splitFileByColumns( s_FilenameIn, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_Extension, gi_sfc_NumberOfColumns, gs_sfc_FixedColumnsList, gsl_FilenameList.count() );

                    stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
                }
                else
                {
                    err = _FILENOTEXISTS_;
                }
            }

            resetFileProgress( gsl_FilenameList.count() );
        }
        else
        {
            err = _CHOOSEABORTED_;
        }
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_sfc_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Split files was canceled" ), true, false );

    onError( err );
}



