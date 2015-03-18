/* 2014-04-18                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-09-27

/*! @brief Erzeuge Scriptdatei.
*          Schreibe den Shebang in die erste Zeile und macht ein ausführbare Datei draus.
*/

int MainWindow::createScript( const QString &s_FilenameIn, const int i_CodecInput, const int i_NumOfFiles )
{
    int         i               = 0;
    int         n               = 0;
    int         stopProgress    = 0;

    QString     s_arg           = "";
    QString     s_FilenameOut   = "";
    QString     s_Output        = "";
    QString     s_Shebang       = "#!/bin/bash";

    QStringList sl_Input;

    QFileInfo   fi( s_FilenameIn );

    QProcess    process;

// **********************************************************************************************

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

// **********************************************************************************************

    #if defined(Q_OS_LINUX)
        s_FilenameOut = fi.absolutePath() + "/" + fi.completeBaseName() + tr( ".sh" );
    #endif

    #if defined(Q_OS_MAC)
        s_FilenameOut = fi.absolutePath() + "/" + fi.completeBaseName() + tr( ".sh" );
    #endif

    #if defined(Q_OS_WIN)
        s_FilenameOut = fi.absolutePath() + "/" + fi.completeBaseName() + tr( ".cmd" );
    #endif

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

    if ( sl_Input.at( 0 ) != s_Shebang )
    {
        #if defined(Q_OS_LINUX)
            tout << s_Shebang << endl;
        #endif

        #if defined(Q_OS_MAC)
            tout << s_Shebang << endl;
        #endif

        #if defined(Q_OS_WIN)
            ; // Shebang not needed
        #endif
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Creating scripts..." ), sl_Input.count() );

    while ( ( i < n )  && ( stopProgress != _APPBREAK_ ) )
    {
        s_Output = sl_Input.at( i );

        tout << s_Output.replace( "\t", " " ) << endl;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    #if defined(Q_OS_LINUX)
        s_arg = "chmod u+x \"" + QDir::toNativeSeparators( s_FilenameOut ) + "\"";
        process.start( s_arg );
        process.waitForFinished();
    #endif

    #if defined(Q_OS_MAC)
        s_arg = "chmod u+x \"" + QDir::toNativeSeparators( s_FilenameOut ) + "\"";
        process.start( s_arg );
        process.waitForFinished();
    #endif

    #if defined(Q_OS_WIN)
        ; // not needed
    #endif

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCreateScript()
{
    int		i                   = 0;
    int		err                 = 0;
    int		stopProgress        = 0;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Create script files ..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            createScript( gsl_FilenameList.at( i ), gi_CodecInput, gsl_FilenameList.count() );

            stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
        }

        resetFileProgress( gsl_FilenameList.count() );
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( gsl_FilenameList.count() == 1 )
            QMessageBox::information( this, getApplicationName( true ), tr( "Script has been created." ) );
        else
            QMessageBox::information( this, getApplicationName( true ), tr( "Scripts have been created." ) );
    }

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Creating script files was canceled" ), true );

    onError( err );
}
