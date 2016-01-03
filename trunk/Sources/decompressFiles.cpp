/* 2016-11-03                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-01-03

/*! @brief Dekomprimieren von Dateien mit unzip. unzip
*   muss sich im gleichen Verzeichnis befinden wie das Programm "PanTool".
*/

void MainWindow::decompressFile( const QString &s_FilenameIn )
{
    QFileInfo fi( s_FilenameIn );

    QProcess process;

// **********************************************************************************************

    #if defined(Q_OS_LINUX)
        process.start( "unzip \"" + QDir::toNativeSeparators( s_FilenameIn ) + "\"" );
        process.waitForFinished();
    #endif

    #if defined(Q_OS_MAC)
        process.start( "/usr/bin/unzip \"" + QDir::toNativeSeparators( s_FilenameIn ) + "\"" );
        process.waitForFinished();
    #endif

    #if defined(Q_OS_WIN)
        process.start( "7z e \"" + QDir::toNativeSeparators( s_FilenameIn ) + "\"" );
        process.waitForFinished();
    #endif

// **********************************************************************************************

    return;
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doDecompressFiles()
{
    int		  i              = 0;
    int		  err            = 0;
    int       stopProgress   = 0;

    QFileInfo fi;
    QString   s_Extension    = "";

// **********************************************************************************************

    existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );

    if (  err == _NOERROR_ )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Decompressing files ..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            setStatusBar( tr( "Decompress " ) + QDir::toNativeSeparators( gsl_FilenameList.at( i ) ) + tr( " ..." ) );

            fi.setFile( gsl_FilenameList.at( i ) );
            s_Extension = fi.suffix().toLower();

            if ( s_Extension == "zip" )
                decompressFile( gsl_FilenameList.at( i ) );

            stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
        }

        resetFileProgress( gsl_FilenameList.count() );
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Decompress files was canceled" ), true );

    onError( err );
}
