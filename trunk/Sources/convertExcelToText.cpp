/* 2017-09-06                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-09-06

int MainWindow::createConvertExcelToTextScript( const QString &s_Rscript, const int i_Extension )
{
    QFile       fRscript( s_Rscript );

// **********************************************************************************************

    if ( fRscript.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fRscript );

    tout << "library( openxlsx )" << endl;
    tout << "library( tools )" << endl;

    tout << "filePath = commandArgs()[7]" << endl;
    tout << "fileName = file_path_sans_ext( basename( filePath ) )" << endl;

    tout << "loadWorkbook( filePath )->wb" << endl;

    tout << "L=list()" << endl;

    tout << "for ( s in sheets( wb ) )";
    tout << "{" << endl;
    tout << " read.xlsx( wb, sheet=s )->L[[s]]" << endl;
    tout << " names( L[[s]] )<-gsub( \"\\\\.\", \" \", names( L[[s]] ) )" << endl;

    tout << " write.table( L[[s]],";
    tout << " file=paste( fileName, \"_\", s, \"" << setExtension( i_Extension ) << "\", sep=\"\" ),";
    tout << " dec=\".\", sep=\"\\t\", quote=FALSE, row.names=FALSE, fileEncoding=\"UTF-8\" )" << endl;
    tout << "}" << endl;

    fRscript.close();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-09-06

void MainWindow::doConvertExcelToText()
{
    int		   i                = 0;
    int		    err             = 0;
    int         stopProgress    = 0;
    int         i_DialogResult  = QDialog::Accepted;

    QString     s_Rscript       = "";

    QStringList sl_args;
    QStringList sl_Message;

    QProcess    process;

// **********************************************************************************************

    QFileInfo fiRscriptProgram( gs_rs_FilenameRScriptProgram );

    if ( fiRscriptProgram.exists() == false )
        i_DialogResult = doRScriptOptionsDialog();

    if ( i_DialogResult == QDialog::Accepted )
    {
        existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );

        if ( gsl_FilenameList.count() > 0 )
        {
            QFileInfo fiFilenameIn( gsl_FilenameList.at( 0 ) );

            s_Rscript = fiFilenameIn.absolutePath() + "/convertExcelToText.R";

            err = createConvertExcelToTextScript( s_Rscript, gi_Extension );

            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting files ..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                setStatusBar( tr( "Convert " ) + QDir::toNativeSeparators( gsl_FilenameList.at( i ) ) + tr( " ..." ) );

                fiFilenameIn.setFile( gsl_FilenameList.at( i ) );

                if ( fiFilenameIn.suffix().toLower() == "xlsx" )
                {
                    sl_Message.clear();
                    showMessage( tr( "Converting " ) + QDir::toNativeSeparators( fiFilenameIn.absoluteFilePath() ) + tr( " ..." ), sl_Message );

                    sl_args.clear();
                    sl_args.append( "--vanilla" );
                    sl_args.append( s_Rscript );
                    sl_args.append( fiFilenameIn.fileName() );

                    process.setWorkingDirectory( fiFilenameIn.absolutePath() );
                    process.setProgram( fiRscriptProgram.absoluteFilePath() );
                    process.setArguments( sl_args );

                    process.start();
                    process.waitForFinished( -1 );
                }

                stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
            }

            removeFile( s_Rscript );

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
