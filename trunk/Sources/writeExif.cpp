/* 2016-10-31                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-10-31

int MainWindow::writeExif( const QString &s_ExifTool, const QString &s_FilenameIn, const int i_NumOfFiles )
{
    int         i                 = 0;
    int         n                 = 0;
    int         m                 = 0;

    int         err               = _NOERROR_;
    int         stopProgress      = 0;

    int         i_FilenameColumn  = -1;
    int         i_DateTimeColumn  = -1;
    int         i_LatitudeColumn  = -1;
    int         i_LongitudeColumn = -1;
    int         i_AltitudeColumn  = -1;
    int         i_CopyrightColumn = -1;

    double      d_Latitude        = 0;
    double      d_Longitude       = 0;
    double      d_Altitude        = 0;

    bool        b_nextDay         = false;

    QString     InputStr          = "";

    QString     s_Path            = "";
    QString     s_arg             = "";

    QString     s_GPSLatitudeRef  = "";
    QString     s_GPSLongitudeRef = "";
    QString     s_GPSAltitudeRef  = "";

    QString     s_GPSDateStamp    = "2016-10-27";
    QString     s_GPSTimeStamp    = "15:23";

    QDateTime   dt = QDateTime().toUTC();
    QDate       d( 1970, 1, 1 );
    QTime       t( 0, 0, 0, 0 );

    QStringList sl_Input;

    QProcess    process;

// **********************************************************************************************
// File name -> Date/Time -> Latitude -> Longitude -> Altitude

    if ( ( n = readFile( s_FilenameIn, sl_Input, _SYSTEM_ ) ) > 1 )
    {
        InputStr = sl_Input.at( i++ ); m = NumOfSections( InputStr );

        for ( int k=m-1; k>-1; k-- )
        {
            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "file name" ) == true )
                i_FilenameColumn = k;

            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "filename" ) == true )
                i_FilenameColumn = k;

            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "file" ) == true )
                i_FilenameColumn = k;

            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "date/time" ) == true )
                i_DateTimeColumn = k;

            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "datetime" ) == true )
                i_DateTimeColumn = k;

            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "latitude" ) == true )
                i_LatitudeColumn = k;

            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "longitude" ) == true )
                i_LongitudeColumn = k;

            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "altitude" ) == true )
                i_AltitudeColumn = k;

            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "elevation" ) == true )
                i_AltitudeColumn = k;

            if ( InputStr.section( "\t", k, k ).toLower().startsWith( "copyright" ) == true )
                i_CopyrightColumn = k;
        }

        if ( i_FilenameColumn > -1 )
        {
            QFileInfo ImportFile( sl_Input.at( 1 ).section( "\t", i_FilenameColumn, i_FilenameColumn ) );

            if ( ImportFile.isRelative() == true )
            {
                QFileInfo ImportPath( gs_Path );

                if ( ImportPath.exists() == false )
                    gs_Path = getDocumentDir();

                #if defined(Q_OS_LINUX)
                    s_Path = QFileDialog::getExistingDirectory( this, tr( "Choose the folder including images" ), gs_Path, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog );
                #endif

                #if defined(Q_OS_WIN)
                    s_Path = QFileDialog::getExistingDirectory( this, tr( "Choose the folder including images" ), gs_Path, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog );
                #endif

                #if defined(Q_OS_MAC)
                    s_Path = QFileDialog::getExistingDirectory( this, tr( "Choose the folder including images" ), gs_Path, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog );
                #endif
            }

            initProgress( i_NumOfFiles, s_FilenameIn, tr( "Writing exif record to image..." ), sl_Input.count() );

            while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
            {
                s_arg = s_ExifTool;

                s_arg.append( " -overwrite_original" );

                InputStr = sl_Input.at( i );

                if ( i_DateTimeColumn > -1 )
                {
                    if ( InputStr.section( "\t", i_DateTimeColumn, i_DateTimeColumn ).isEmpty() == false )
                    {
                        d = getDate( InputStr.section( "\t", i_DateTimeColumn, i_DateTimeColumn ) );
                        t = getTime( InputStr.section( "\t", i_DateTimeColumn, i_DateTimeColumn ), b_nextDay );

                        dt.setDate( d );
                        dt.setTime( t );
                        dt.setOffsetFromUtc( 0 );

                        if ( b_nextDay == true )
                            dt = dt.addDays( 1 );

                        s_GPSDateStamp = dt.toString( "yyyy-MM-dd" );
                        s_GPSTimeStamp = dt.toString( "hh:mm:ss" );

                        s_arg.append( " -exif:GPSDateStamp=" ).append( s_GPSDateStamp );
                        s_arg.append( " -exif:GPSTimeStamp=" ).append( s_GPSTimeStamp );
                    }
                }

                if ( i_LatitudeColumn > -1 )
                {
                    if ( InputStr.section( "\t", i_LatitudeColumn, i_LatitudeColumn ).isEmpty() == false )
                    {
                        d_Latitude = InputStr.section( "\t", i_LatitudeColumn, i_LatitudeColumn ).toDouble();

                        if ( d_Latitude < 0 )  s_GPSLatitudeRef  = "S"; else s_GPSLatitudeRef = "N";

                        s_arg.append( QString( " -exif:GPSLatitude=%1" ).arg( qAbs( d_Latitude ), 0, 'f', 6 ).append( " -exif:GPSLatitudeRef=" ).append( s_GPSLatitudeRef ) );
                    }
                }

                if ( i_LongitudeColumn > -1 )
                {
                    if ( InputStr.section( "\t", i_LongitudeColumn, i_LongitudeColumn ).isEmpty() == false )
                    {
                        d_Longitude = InputStr.section( "\t", i_LongitudeColumn, i_LongitudeColumn ).toDouble();

                        if ( d_Longitude < 0 ) s_GPSLongitudeRef = "W"; else s_GPSLongitudeRef = "O";

                        s_arg.append( QString( " -exif:GPSLongitude=%1" ).arg( qAbs( d_Longitude ), 0, 'f', 6 ).append( " -exif:GPSLongitudeRef=" ).append( s_GPSLongitudeRef ) );
                    }
                }

                if ( i_AltitudeColumn > -1 )
                {
                    if ( InputStr.section( "\t", i_AltitudeColumn, i_AltitudeColumn ).isEmpty() == false )
                    {
                        d_Altitude = InputStr.section( "\t", i_AltitudeColumn, i_AltitudeColumn ).toDouble();

                        if ( d_Altitude < 0 )  s_GPSAltitudeRef  = "below"; else s_GPSAltitudeRef = "above";

                        s_arg.append( QString( " -exif:GPSAltitude=%1" ).arg( qAbs( d_Altitude ), 0, 'f', 1 ).append( " -exif:GPSAltitudeRef=" ).append( s_GPSAltitudeRef ) );
                    }
                }

                if ( i_CopyrightColumn > -1 )
                {
                    if ( InputStr.section( "\t", i_CopyrightColumn, i_CopyrightColumn ).isEmpty() == false )
                        s_arg.append( " -exif:Copyright=" ).append( "\"" + InputStr.section( "\t", i_CopyrightColumn, i_CopyrightColumn ) + "\"" );
                }

                if ( s_arg.section( "-overwrite_original", 1, 1 ).isEmpty() == false )
                {
                    if ( InputStr.section( "\t", i_FilenameColumn, i_FilenameColumn ).isEmpty() == false )
                    {
                        QFileInfo ImportFile( InputStr.section( "\t", i_FilenameColumn, i_FilenameColumn ) );

                        if ( ImportFile.isRelative() == true )
                            ImportFile.setFile( s_Path, ImportFile.fileName() );

                        if ( ImportFile.exists() == true )
                        {
                            s_arg.append( " " ).append( "\"" + QDir::toNativeSeparators( ImportFile.absoluteFilePath() ) + "\"" );

                            process.start( s_arg );
                            process.waitForFinished( -1 );

                            wait( 500 );
                        }
                    }
                }

                stopProgress = incProgress( i_NumOfFiles, ++i );
            }

            resetProgress( i_NumOfFiles );
        }
    }

    return( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-08-17

void MainWindow::doWriteExif()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_ExifTool      = "";

// **********************************************************************************************

    s_ExifTool = findExifTool();

    if ( s_ExifTool != "ExifTool not found" )
    {
        existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );

        if ( gsl_FilenameList.count() > 0 )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Writing exif record to images..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                err = writeExif( s_ExifTool, gsl_FilenameList.at( i ), gsl_FilenameList.count() );

                stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Writing exif record was canceled" ), true );

    onError( err );
}
