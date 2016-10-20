/* 2007-11-07                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCompressFolderZip()
{
    int     err       = _NOERROR_;

    QString s_Program = "";

// **********************************************************************************************

    s_Program = findZip( _ZIP_ );

    if ( s_Program != "Zip not found" )
    {
        if ( chooseFolder() == _NOERROR_ )
            compressFolder( gs_Path, _ZIP_, s_Program );
        else
            err = _CHOOSEABORTED_;
    }

    endTool( err, 0, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Compressing folder was canceled" ), true );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCompressFolderGZip()
{
    int     err       = _NOERROR_;

    QString s_Program = "";

// **********************************************************************************************

    s_Program = findZip( _GZIP_ );

    if ( s_Program != "Zip not found" )
    {
        if ( chooseFolder() == _NOERROR_ )
            compressFolder( gs_Path, _GZIP_, s_Program );
        else
            err = _CHOOSEABORTED_;
    }

    endTool( err, 0, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Compressing folder was canceled" ), true );

    onError( err );
}
