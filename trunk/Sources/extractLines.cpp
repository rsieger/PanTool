/* extractLines.cpp           */
/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::extractLines( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_StartLine, const int i_Increment, const int i_NumberOfLines, const bool b_ExtractFristLineLastLine, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const int i_NumOfFiles )
{
    int         i               = i_StartLine + i_Increment - 1;
    int         n               = 0;
    int         e               = 0;

    int         stopProgress    = 0;

    QString     s_EOL           = setEOLChar( i_EOL );
    QString     s_Output        = "";

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

// **********************************************************************************************

    if ( n < i_StartLine )
        return( _NOERROR_ );

    if ( i_NumberOfLines == 0 )
        e = n;
    else
        e = qMin( n, i_StartLine + i_NumberOfLines * i_Increment - 1 );

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

    if ( b_ExtractFristLineLastLine == false )
    {
        initProgress( i_NumOfFiles, s_FilenameIn, tr( "Extracting lines..." ), e );

        tout << sl_Input.at( i_StartLine - 1 ) << s_EOL;

        while ( ( i<e ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
                tout << sl_Input.at( i ) << s_EOL;

            i += i_Increment;

            stopProgress = incProgress( i_NumOfFiles, i );
        }

        resetProgress( i_NumOfFiles );
    }
    else
    {
        setStatusBarFileInProgress( s_FilenameIn );

        tout << sl_Input.at( i_StartLine - 1 ) << s_EOL;
        tout << sl_Input.last() << s_EOL;
    }

// **********************************************************************************************
// Join lines (Bracher, PDI-15573)
/*
    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Join lines..." ), e );

    i = 0;

    while ( ( i<e ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).section( ",", 1, 1 ).contains( ":" ) == false )
            s_Output.append( sl_Input.at( i ) + "\t" );
        else
        {
            s_Output.replace( ",", "\t" );
            tout << s_Output << s_EOL;
            s_Output = sl_Input.at( i ) + "\t";
            s_Output.replace( "/", "." );
        }

        s_Output.replace( ",", "\t" );
        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    tout << s_Output << s_EOL;

    resetProgress( i_NumOfFiles );
*/
// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-27

void MainWindow::doExtractLines()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doExtractLinesDialog( gi_el_StartLine, gi_el_Increment, gi_el_NumberOfLines, gb_el_ExtractFristLineLastLine, gb_el_SkipEmptyLines, gb_el_SkipCommentLines, gb_el_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Extracting lines..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = extractLines( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_el_StartLine, gi_el_Increment, gi_el_NumberOfLines, gb_el_ExtractFristLineLastLine, gb_el_SkipEmptyLines, gb_el_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_el_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Extracting lines was canceled" ) );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-10-24

void MainWindow::doTranslateCharacterEncoding()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doTranslateCharacterEncodingDialog( gi_tce_CodecInput, gi_tce_CodecOutput, gi_tce_EOL, gb_tce_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Translating character encoding..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = extractLines( s_FilenameIn, s_FilenameOut, gi_tce_CodecInput, gi_tce_CodecOutput, gi_tce_EOL, 1, 1, 0, false, false, false, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_tce_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Translating character encoding was canceled" ) );

    onError( err );
}
