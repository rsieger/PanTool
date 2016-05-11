/* 2008-07-17                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::saveFilelist( const QString &s_FilenameOut, const QStringList sl_FilenameList, const int i_CodecOutput, const QString &s_LocalRootDir, const QString &s_ExternalWebPath, const int i_EOL )
{
    QString     s_FilePath  = "";
    QString     s_EOL       = setEOLChar( i_EOL );

    QDateTime   CreationDateTime;

// **********************************************************************************************
// open output file

    QFileInfo fi( s_FilenameOut );

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

    switch ( i_CodecOutput )
    {
    case _SYSTEM_:
        break;
    case _LATIN1_:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case _APPLEROMAN_:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    if ( s_ExternalWebPath.isEmpty() == true )
    {
        for ( int i=0; i<sl_FilenameList.count(); i++ )
        {
            fi.setFile( sl_FilenameList.at( i ) );

            CreationDateTime.setDate( fi.created().date() );
            CreationDateTime.setTime( fi.created().time() );

            tout << QDir::toNativeSeparators( fi.absoluteFilePath() ) << "\t";
            tout << QDir::toNativeSeparators( fi.absolutePath() + "/" ) << "\t";
            tout << fi.fileName() << "\t";
            tout << fi.completeBaseName() << "\t";
            tout << fi.suffix() << "\t";
            tout << fi.size() << "\t";
            tout << CreationDateTime.toString( "yyyy-MM-ddThh:mm:ss" );
            tout << s_EOL;
        }
    }
    else
    {
        tout << "Event label\tFile content []\tFile name []\tFile name []\tFile format []\tFile size [kByte]\tURL file []" << s_EOL;

        for ( int i=0; i<sl_FilenameList.count(); i++ )
        {
            fi.setFile( sl_FilenameList.at( i ) );

            s_FilePath = fi.absoluteFilePath();
            s_FilePath = s_FilePath.replace( s_LocalRootDir.section( "/", 0, s_LocalRootDir.count( "/" )-1 ) + "/", "" );

            tout << "???" << "\t";                      // Event label
            tout << "\t";                               // File content
            tout << fi.fileName() << "\t";              // File name
            tout << fi.completeBaseName() << "\t";      // File name
            tout << fi.suffix().toUpper() << "\t";      // File format
            tout << QString( "%1").arg( (float) fi.size()/1024., 0,'f', 3 ) << "\t";    // File size in KByte
            tout << s_ExternalWebPath << s_FilePath;    // External web path plus file name
            tout << s_EOL;
        }
    }

    fout.close();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doSaveFilelist()
{
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameOut   = "";

// **********************************************************************************************

    existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );

    if ( gsl_FilenameList.count() > 0 )
    {
        QFileInfo fi( gsl_FilenameList.at( 0 ) );

        switch ( gi_Extension )
        {
        case _CSV_:
            s_FilenameOut = QFileDialog::getSaveFileName( this, tr( "Save list of files to" ), fi.absolutePath() + tr( "_filelist.csv" ), tr( "Text (*.csv)" ), 0, QFileDialog::DontUseNativeDialog );
            break;
        default:
            s_FilenameOut = QFileDialog::getSaveFileName( this, tr( "Save list of files to" ), fi.absolutePath() + tr( "_filelist.txt" ), tr( "Text (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
            break;
        }

        if ( ( s_FilenameOut.isEmpty() == false ) && ( doFileListOptionsDialog( gs_LocalRootDir, gs_ExternalWebPath ) == QDialog::Accepted ) )
            err = saveFilelist( s_FilenameOut, gsl_FilenameList, gi_CodecOutput, gs_LocalRootDir, gs_ExternalWebPath, gi_EOL );
        else
            err = _CHOOSEABORTED_;
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Save list of files was canceled" ), false, false );

    onError( err );
}


