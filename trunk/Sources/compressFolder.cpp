/* 2007-11-07                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-09-27

/*! @brief Komprimieren eines Verzeichnisses mit zip. zip
*   muss sich im gleichen Verzeichnis befinden wie das Programm "PanTool".
*/

int MainWindow::compressFolder( const QString &s_Folder )
{
    QFileInfo fi( s_Folder );

    QProcess process;

    QString  s_arg      = "";
    QString  s_Script   = "";
    QString  s_Shebang  = "#!/bin/bash";

// **********************************************************************************************

    #if defined(Q_OS_LINUX)
        s_Script = fi.absolutePath() + "/zip_" + fi.completeBaseName() + tr( ".sh" );
    #endif

    #if defined(Q_OS_MAC)
        s_Script = fi.absolutePath() + "/zip_" + fi.completeBaseName() + tr( ".sh" );
    #endif

    #if defined(Q_OS_WIN)
        s_Script = fi.absolutePath() + "/zip_" + fi.completeBaseName() + tr( ".cmd" );
    #endif

    QFile fout( s_Script );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

    #if defined(Q_OS_LINUX)
        tout << s_Shebang << endl;
    #endif

    #if defined(Q_OS_MAC)
        tout << s_Shebang << endl;
    #endif

    #if defined(Q_OS_WIN)
        ; // Shebang not needed
    #endif

    tout << "cd \"" + QDir::toNativeSeparators( fi.absolutePath() ) + "\"" << endl;
    tout << "zip -r \"" + fi.completeBaseName() + ".zip\"" + " \"" + fi.completeBaseName() + "/\" -x *.DS_Store";

    fout.close();

    #if defined(Q_OS_LINUX)
        s_arg = "chmod u+x \"" + QDir::toNativeSeparators( s_Script ) + "\"";
        process.start( s_arg );
        process.waitForFinished();
    #endif

    #if defined(Q_OS_MAC)
        s_arg = "chmod u+x \"" + QDir::toNativeSeparators( s_Script ) + "\"";
        process.start( s_arg );
        process.waitForFinished();
    #endif

    #if defined(Q_OS_WIN)
        ;
    #endif

    process.start( s_Script );
    process.waitForFinished( -1 );

    removeFile( s_Script );

// **********************************************************************************************

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCompressFolder()
{
    int		err                 = 0;

    if ( chooseFolder() == _NOERROR_ )
        err = compressFolder( gs_Path );
    else
        err = _CHOOSEABORTED_;

    endTool( err, 0, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Compressing folder was canceled" ), true );

    onError( err );
}
