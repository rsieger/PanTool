/* 2012-11-18                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-11-18

void MainWindow::doRenameFiles()
{
    int         i                     = 0;
    int         err                   = 0;
    int         stopProgress          = 0;
    int         NumOfFilesRenamed     = 0;

    QString     s_FilenameIn          = "";
    QString     s_FilenameOut         = "";

    QStringList sl_tempFilenameList;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doRenameFilesDialog( gs_rf_SearchString, gs_rf_ReplaceString ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Renaming files..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    s_FilenameOut = s_FilenameIn;
                    s_FilenameOut.replace( gs_rf_SearchString, gs_rf_ReplaceString );

                    if ( ( QFile::exists( s_FilenameIn ) == true ) && ( s_FilenameOut != s_FilenameIn ) && ( sl_tempFilenameList.contains( s_FilenameOut ) == false ) )
                    {
                        if ( s_FilenameOut.toLower() != s_FilenameIn.toLower() )
                            QFile::remove( s_FilenameOut );

                        QFile::rename( s_FilenameIn, s_FilenameOut );

                        NumOfFilesRenamed++;
                    }

                    NumOfFilesRenamed--;
                    sl_tempFilenameList.append( s_FilenameOut );

                    stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
                }
                else
                {
                    err = _FILENOTEXISTS_;
                }
            }

            resetFileProgress( gsl_FilenameList.count() );

            gi_ActionNumber  = 1;
            gsl_FilenameList = sl_tempFilenameList;

            if ( NumOfFilesRenamed < 0 )
                QMessageBox::information( this, getApplicationName( true ), tr( "Some files have not been renamed." ) );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Renaming files was canceled" ), false, false );

    onError( err );
}



