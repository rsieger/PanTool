/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::deleteMatchedLines( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const QString &s_SearchString, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const int i_NumOfFiles )
{
    int         i                   = 0;
    int         n                   = 0;

    int         stopProgress        = 0;

    QString     s_EOL               = setEOLChar( i_EOL );

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

// **********************************************************************************************
// open output file

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

    switch ( i_CodecOutput )
    {
    case _SYSTEM_:
        break;
    case _LATIN1_:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case _APPLEROMAN_:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Deleting lines..." ), sl_Input.count() );

    while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).contains( s_SearchString ) == false )
        {
            if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
                tout << sl_Input.at( i ) << s_EOL;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doDeleteMatchedLines()
{
    int         i                               = 0;
    int         err                             = 0;
    int         stopProgress                    = 0;

    QString     s_dummy_ReplaceString           = "";
    QString     s_dummy_DatabaseName            = "";

    int         i_dummy_StartLine               = 1;
    int         i_dummy_NumberOfLines           = 0;
    int         i_dummy_SearchAndReplaceMode    = _LINE;

    bool        b_dummy_SaveFirstLine           = false;
    bool        b_dummy_SaveNoMatch             = false;
    bool        b_dummy_DeleteEmptyOutputFile   = false;

    QString     s_FilenameIn                    = "";
    QString     s_FilenameOut                   = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSearchDialog( _DELETELINES, gs_dml_DeleteLinesSearchString, s_dummy_ReplaceString, s_dummy_DatabaseName, i_dummy_StartLine, i_dummy_NumberOfLines, i_dummy_SearchAndReplaceMode, b_dummy_SaveFirstLine, b_dummy_SaveNoMatch, gb_dml_SkipEmptyLines, gb_dml_SkipCommentLines, b_dummy_DeleteEmptyOutputFile, gb_dml_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Deleting lines..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = deleteMatchedLines( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gs_dml_DeleteLinesSearchString, gb_dml_SkipEmptyLines, gb_dml_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_dml_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Deleting lines was canceled" ) );

    onError( err );
}

