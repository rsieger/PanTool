/* 2008-07-17                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::saveFilelist( const QString &s_FilenameOut, const QStringList sl_FilenameList, const int i_CodecOutput, const int i_EOL )
{
    QString     s_EOL = setEOLChar( i_EOL );

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

    for ( int i=0; i<sl_FilenameList.count(); i++ )
    {
        fi.setFile( sl_FilenameList.at( i ) );

        CreationDateTime.setDate( fi.created().date() );
        CreationDateTime.setTime( fi.created().time() );

        tout << QDir::toNativeSeparators( fi.absoluteFilePath() ) << "\t";
        tout << QDir::toNativeSeparators( fi.absolutePath() ) << "\t";
        tout << fi.fileName() << "\t" << fi.baseName() << "\t";
        tout << fi.completeSuffix() << "\t" << fi.size() << "\t" << CreationDateTime.toString( "yyyy-MM-ddThh:mm:ss" ) << s_EOL;
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

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
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

        if ( s_FilenameOut.isEmpty() == false )
            err = saveFilelist( s_FilenameOut, gsl_FilenameList, gi_CodecOutput, gi_EOL );
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


