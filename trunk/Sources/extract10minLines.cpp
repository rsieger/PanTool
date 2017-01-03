/* 2011-12-20                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-01-04

int MainWindow::extract10minLines( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const bool b_SaveHeader, const bool b_DeleteInputFile, const int i_NumOfFiles )
{
    int         i                   = 0;
    int         n                   = 0;
    int         stopProgress        = 0;

    int         i_err               = _NOERROR_;

    int         i_DateTimeColumn    = 0;
    int         i_Duration          = 600; // in seconds

    QString     s_EOL               = setEOLChar( i_EOL );

    QStringList sl_Input;

    QDateTime   DateTime1( QDate( 0001, 01, 01 ) );
    QDateTime   DateTime2( QDate( 0001, 01, 01 ) );

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

// **********************************************************************************************

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Extracting 10 min lines..." ), sl_Input.count() );

// **********************************************************************************************

    i_DateTimeColumn = findDateTimeColumn( sl_Input.at( 0 ) );

    if ( b_SaveHeader == true )  // save header
        tout << sl_Input.at( i++ ) << s_EOL;
    else
        i++;

    while ( ( i<n-1 ) && ( stopProgress != _APPBREAK_ ) )
    {
        int i_DateTimeFormat = sl_Input.at( i ).section( "\t", i_DateTimeColumn, i_DateTimeColumn ).length();

        switch ( i_DateTimeFormat )
        {
        case 16:
            DateTime2.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", i_DateTimeColumn, i_DateTimeColumn ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
            DateTime2.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", i_DateTimeColumn, i_DateTimeColumn ).section( "T", 1, 1 ), "hh:mm" ) );
            break;
        case 19:
            DateTime2.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", i_DateTimeColumn, i_DateTimeColumn ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
            DateTime2.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", i_DateTimeColumn, i_DateTimeColumn ).section( "T", 1, 1 ), "hh:mm:ss" ) );
            break;
        case 23:
            DateTime2.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", i_DateTimeColumn, i_DateTimeColumn ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
            DateTime2.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", i_DateTimeColumn, i_DateTimeColumn ).section( "T", 1, 1 ), "hh:mm:ss.zzz" ) );
            break;
        default:
            i_err = _WRONGDATETIMEFORMAT_;
            break;
        }

        if ( ( i_err == _NOERROR_ ) && ( DateTime1.secsTo( DateTime2 ) >= i_Duration ) ) // duration in seconds
        {
            DateTime1 = DateTime2;

            tout << sl_Input.at( i ) << s_EOL;
        }

        if ( i_err != _NOERROR_ )
        {
            tout << "Format of date/time is wrong. Must be yyyy-MM-ddThh:mm, yyyy-MM-ddThh:mm:ss or yyyy-MM-ddThh:mm:ss.zzz" << s_EOL;

            i_err = _NOERROR_;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

// last line
    tout << sl_Input.at( i ) << s_EOL;

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    if ( b_DeleteInputFile == true )
        removeFile( s_FilenameIn );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2014-02-24

int MainWindow::findDateTimeColumn( const QString s_InputStr )
{
    int i = 0;
    int n = NumOfSections( s_InputStr );

// **********************************************************************************************

    while ( i<n )
    {
        if ( s_InputStr.section( "\t", i, i ).toLower().startsWith( "date/time" ) == true )
            return( i );
        else
            i++;
    }

    return( -1 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-01-03

void MainWindow::doExtract10minLines()
{
    int     i                            = 0;
    int     err                          = 0;
    int     stopProgress                 = 0;

    bool    b_dummy_SkipEmptyLines       = false;
    bool    b_dummy_SkipCommentLines     = false;

    QString s_FilenameIn                 = "";
    QString s_FilenameOut                = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSkipLinesDialog( _EXTRACT10MINLINES, gb_e10minl_SaveHeader, b_dummy_SkipEmptyLines, b_dummy_SkipCommentLines, gb_e10minl_DeleteInputFile ) == QDialog::Accepted )
        {
            if ( gs_eml_ExtractLinesSearchString.isEmpty() == false )
            {
                initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Extracting 10 min lines..." ) );

                while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
                {
                    if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                    {
                        err = extract10minLines( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gb_e10minl_SaveHeader, gb_e10minl_DeleteInputFile, gsl_FilenameList.count() );

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
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Extracting 10 min lines was canceled" ) );

    onError( err );
}

