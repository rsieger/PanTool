/* 2017-09-06                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-09-06

int MainWindow::convertExcelToText( const QString &s_FilenameIn, const QString &s_Program, const int i_Extension )
{
    QProcess    process;

    QStringList sl_args;
    QStringList sl_Message;

    QFileInfo   fiFilenameIn( s_FilenameIn );
    QFile       fRScript( fiFilenameIn.absolutePath() + "/convertExcelToText.R" );

// **********************************************************************************************

    if ( fiFilenameIn.exists() == false )
        return( _FILENOTEXISTS_ );

    if ( fRScript.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fRScript );

    tout << "library( openxlsx )" << endl;
    tout << "loadWorkbook( \"" + QDir::toNativeSeparators( fiFilenameIn.absoluteFilePath() ) + "\" )->wb" << endl;

    tout << "L=list()" << endl;

    tout << "for ( s in sheets( wb ) )";
    tout << "{" << endl;
    tout << " read.xlsx( wb, sheet=s )->L[[s]]" << endl;
    tout << " write.table( L[[s]],";
    tout << " file=paste(\"" << fiFilenameIn.baseName() << "_\", s, \"" << setExtension( i_Extension ) << "\", sep=\"\" ),";
    tout << " dec=\".\", sep=\"\t\", quote=FALSE, row.names=FALSE )" << endl;
    tout << "}" << endl;

    fRScript.close();

// **********************************************************************************************

    sl_Message.clear();

    showMessage( tr( "Converting " ) + QDir::toNativeSeparators( fiFilenameIn.absoluteFilePath() ) + tr( " ..." ), sl_Message );

    sl_args.append( "--vanilla" );
    sl_args.append( "convertExcelToText.R" );

    process.setWorkingDirectory( fiFilenameIn.absolutePath() );
    process.setProgram( s_Program );
    process.setArguments( sl_args );

    process.start();
    process.waitForFinished( -1 );

    removeFile( fRScript.fileName() );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-09-06

void MainWindow::doConvertExcelToText()
{
    int		  i                 = 0;
    int		  err               = 0;
    int       stopProgress      = 0;
    int       i_DialogResult    = QDialog::Accepted;

// **********************************************************************************************

    QFileInfo fi( gs_rs_FilenameRScriptProgram );

    if ( fi.exists() == false )
        i_DialogResult = doRScriptOptionsDialog();

    if ( i_DialogResult == QDialog::Accepted )
    {
        existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );

        if ( gsl_FilenameList.count() > 0 )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting files ..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                setStatusBar( tr( "Convert " ) + QDir::toNativeSeparators( gsl_FilenameList.at( i ) ) + tr( " ..." ) );

                fi.setFile( gsl_FilenameList.at( i ) );

                if ( fi.suffix().toLower() == "xlsx" )
                    err = convertExcelToText( gsl_FilenameList.at( i ), gs_rs_FilenameRScriptProgram, gi_Extension );

                stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
            }

            resetFileProgress( gsl_FilenameList.count() );
        }
        else
        {
            err = _CHOOSEABORTED_;
        }
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting files was canceled" ), true );

    onError( err );
}
