/* 2007-11-07                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCompressFiles( const int mode )
{
    int		i                   = 0;
    int		err                 = 0;
    int		stopProgress        = 0;

    QString s_Program           = "";

// **********************************************************************************************

    s_Program = findZip( mode );

    if ( s_Program != "Zip not found" )
    {
        existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );

        if ( gsl_FilenameList.count() > 0 )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Compressing files ..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( stopProgress != _APPBREAK_ ) )
            {
                setStatusBar( tr( "Compress " ) + QDir::toNativeSeparators( gsl_FilenameList.at( i ) ) + tr( " ..." ) );

                compressFile( gsl_FilenameList.at( i ), mode, s_Program );

                stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
            }

            resetFileProgress( gsl_FilenameList.count() );
        }
        else
        {
            err = _CHOOSEABORTED_;
        }
    }

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Compressing files was canceled" ), true );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCompressFilesZip()
{
    doCompressFiles( _ZIP_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCompressFilesGZip()
{
    doCompressFiles( _GZIP_ );
}
