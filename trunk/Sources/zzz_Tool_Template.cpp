/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include <QtGui>

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::NameOfTool( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const bool b_DeleteInputFile, const int i_NumOfFiles )
{
    int         i               = 0;
    int         n               = 0;

    int         stopProgress    = 0;

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

// **********************************************************************************************
// open output file

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == FALSE )
        return( -20 );

    QTextStream tout( &fout );

    switch ( i_CodecOutput )
    {
    case -1:
        break;
    case 1:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case 2:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Deleting double lines..." ), sl_Input.count() );

    tout << sl_Input.at( i++ ) << endl;

    while ( ( i<n ) && ( stopProgress != APPBREAK ) )
    {
        if ( sl_Input.at( i ) != sl_Input.at( i-1) )
        {
            if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == TRUE )
                tout << sl_Input.at( i ) << endl;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == APPBREAK )
        return( APPBREAK );

    if ( b_DeleteInputFile == TRUE )
        removeFile( s_FilenameIn );

    return( NOERROR );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doNameOfTool()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gsl_FilenameList ) == TRUE )
    {
        if ( doDialog( Mode, gb_dl_SkipEmptyLines, gb_dl_SkipCommentLines, gb_dl_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Deleting double lines..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == NOERROR ) && ( stopProgress != APPBREAK ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == TRUE )
                {
                    err = NameOfTool( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gb_dl_SkipEmptyLines, gb_dl_SkipCommentLines, gb_dl_DeleteInputFile, gsl_FilenameList.count() );

                    stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
                }
                else
                {
                    err = FILENOTEXISTS;
                }
            }

            resetFileProgress( gsl_FilenameList.count() );
        }
        else
        {
            err = CHOOSEABORTED;
        }
    }
    else
    {
        err = CHOOSEABORTED;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gsl_FilenameList, tr( "Done" ), tr( "Deleting double lines was canceled" ) );

    onError( err );
}

