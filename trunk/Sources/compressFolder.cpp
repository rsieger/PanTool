/* 2007-11-07                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCompressFolder()
{
    int     err       = _NOERROR_;

    QString s_Program = "";

// **********************************************************************************************

    s_Program = findZip( _ZIP_ );

    if ( s_Program != "Zip not found" )
    {
        if ( chooseFolder() == _NOERROR_ )
            compressFolder( s_Program, gs_Path );
        else
            err = _CHOOSEABORTED_;
    }

    endTool( err, 0, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Compressing folder was canceled" ), true );

    onError( err );
}
