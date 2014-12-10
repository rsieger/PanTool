/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::checkTimeSeries( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i                 = 0;
    int         n                 = 0;

    int         i_Result          = 0;
    int         i_NumberOfColumns = 0;
    int         i_DateTimeColumn  = -1;

    int         stopProgress      = 0;

    QString     s_EOL             = setEOLChar( i_EOL );
    QString     s_Header          = "";

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Checking time series..." ), n );

// **********************************************************************************************

    if ( sl_Input.at( i ).contains( "/*" ) == true )
    {
        while ( ( i < n ) && ( sl_Input.at( i ).contains( "*/" ) == false ) )
            i++;

        i++;
    }

// **********************************************************************************************
// find date/time column

    s_Header = sl_Input.at( i++ );

    i_NumberOfColumns = NumOfSections( s_Header );

    for ( int i=0; i<i_NumberOfColumns; i++ )
    {
        if ( s_Header.section( "\t", i, i).toLower() == "date/time" )
            i_DateTimeColumn = i;
    }

// **********************************************************************************************

    QFileInfo fi( s_FilenameIn );

// **********************************************************************************************

    i++; //start with data line 2

    tout << "Filename\ttime(i-1) > time(i)"<< s_EOL;

    if ( i_DateTimeColumn < 0 )
    {
        tout << fi.baseName() << "\t" << tr( "date/time not found" ) << s_EOL;

        i_Result = 1;

        stopProgress = incProgress( i_NumOfFiles, n );
    }
    else
    {
        while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( sl_Input.at( i-1 ).section( "\t", i_DateTimeColumn, i_DateTimeColumn ) > sl_Input.at( i ).section( "\t", i_DateTimeColumn, i_DateTimeColumn ) )
            {
                tout << fi.baseName() << "\t" << QString( "%1" ).arg( i ) << s_EOL;

                i_Result = i;
            }

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    if ( i_NumOfFiles == 1 )
    {
        switch ( i_Result )
        {
        case 0:
            fout.remove();
            QMessageBox::information( this, getApplicationName( true ), QString( "No problems in file\n%1" ).arg( fi.baseName() ) );
            break;
        case 1:
            fout.remove();
            QMessageBox::information( this, getApplicationName( true ), QString( "Date/time not found in file\n%1" ).arg( fi.baseName() ) );
            break;
        default:
            QMessageBox::information( this, getApplicationName( true ), QString( "Something is wrong in file\n%1" ).arg( fi.baseName() ) );
            break;
        }
    }

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-27

void MainWindow::doCheckTimeSeries()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Checking time series..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = checkTimeSeries( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

// **********************************************************************************************

    if ( ( gsl_FilenameList.count() > 1 ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
    {
        QFileInfo fi( gsl_FilenameList.at( 0 ) );

        s_FilenameOut = fi.absolutePath() + "/" + fi.absolutePath().section( "/", -1, -1 ) + "_time_series_check_results" + setExtension( gi_Extension );
        stopProgress  = concatenateFilesByLines( gi_ActionNumber+1, gs_FilenameFormat, gi_Extension, gsl_FilenameList, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, 1, false, 0, 0, true );
    }

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Checking time series lines was canceled" ) );

    onError( err );
}


