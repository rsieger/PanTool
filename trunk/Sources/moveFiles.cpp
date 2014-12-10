/* 2012-11-18                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

int MainWindow::moveFile( const QString &s_FilenameIn, const QString &s_TargetDirectory )
{
    QFile       fc( s_FilenameIn );
    QFileInfo   fin( s_FilenameIn );

    fc.copy( s_TargetDirectory + "/" + fin.fileName() );

    if ( fc.error() == QFile::NoError )
        fc.remove();

    return( fc.error() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::moveFiles( const QString &s_FilenameListFile, const QString &s_TargetDirectory )
{
    int         i               = 0;
    int         n               = 0;
    int         stopProgress    = 0;

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameListFile, sl_Input, _SYSTEM_ ) ) < 1 )
        return( n );

// **********************************************************************************************

    initFileProgress( 1, s_FilenameListFile, tr( "Moving files..." ) );

    while ( ( i<n ) && ( stopProgress != _APPBREAK_ ) )
    {
        moveFile( sl_Input.at( i ).section( "\t", 0, 0 ), s_TargetDirectory );

        stopProgress = incFileProgress( 1, ++i );
    }

    resetFileProgress( 1 );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-11-18

void MainWindow::doMoveFiles()
{
    int     err             = 0;
    int     stopProgress    = 0;

// **********************************************************************************************

    if ( doMoveFilesDialog( gs_FilenameListFile, gs_TargetDirectory ) == QDialog::Accepted )
        err = moveFiles( gs_FilenameListFile, gs_TargetDirectory );
    else
        err = _CHOOSEABORTED_;

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Moving files was canceled" ) );

    onError( err );
}



