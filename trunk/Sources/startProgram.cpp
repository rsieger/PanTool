// ***********************************************************************************************
// *                                                                                             *
// * startProgram.cpp - start an external program                                                *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-04-10                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

int MainWindow::startProgram( const QString &s_Program, const QString &s_Filename )
{
    QProcess    process;
    QStringList sl_args;
    QString     s_ProgramFilePath = "";

// ***********************************************************************************************

    QFileInfo fi( s_Program );

    if ( fi.exists() == false )
    {
        QString s_Message = "Cannot find the program\n\n    " + QDir::toNativeSeparators( s_Program ) + "\n\n Please start the program manually from your shell.";
        QMessageBox::warning( this, getApplicationName( true ), s_Message );

        return( _ERROR_ );
    }
    else
    {
        #if defined(Q_OS_LINUX)
            s_ProgramFilePath = s_Program;
        #endif

        #if defined(Q_OS_WIN)
            s_ProgramFilePath = s_Program;
        #endif

        #if defined(Q_OS_MAC)
            s_ProgramFilePath = QDir::toNativeSeparators( fi.absoluteFilePath() );
            s_ProgramFilePath.append( "/Contents/MacOS/odv4" );
        #endif

        sl_args.append( QDir::toNativeSeparators( s_Filename ) );

        if ( process.startDetached( s_ProgramFilePath, sl_args ) == false )
        {
            QString s_Message = "Cannot start the program\n\n    " + QDir::toNativeSeparators( s_Program ) + "\n\n Please start the program manually from your shell.";
            QMessageBox::warning( this, getApplicationName( true ), s_Message );

            return( _ERROR_ );
        }
    }

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::startGoogleEarth( const QString &s_FilenameGoogleEarthProgram, const QString &s_FilenameGoogleEarth )
{
    int err = _NOERROR_;

    if ( ( s_FilenameGoogleEarth.contains( " " ) == true ) && ( s_FilenameGoogleEarthProgram.contains( "googleearth" ) == true ) )
    {
        QString s_Message = "The file name\n\n    " + QDir::toNativeSeparators( s_FilenameGoogleEarth ) + "\n\n contains spaces. Google Earth cannot open files in directories with spaces.";
        QMessageBox::warning( this, getApplicationName( true ), s_Message );
    }
    else
    {
        err = startProgram( s_FilenameGoogleEarthProgram, s_FilenameGoogleEarth );
    }

    return( err );
}
