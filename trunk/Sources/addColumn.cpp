/* 2011-10-11                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::addColumn( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const QString &s_HeaderText, const QString &s_ColumnText, const bool b_AddFilename, const bool b_AddFullPath, const bool b_AddOrdinalNumber, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const bool b_DeleteInputFile, const int i_NumOfFiles )
{
    int         i               = 0;
    int         k               = 0;
    int         n               = 0;
    int         stopProgress    = 0;

    bool        b_AddColumn     = true;

    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

// **********************************************************************************************
// open output file

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

    QFileInfo fi( s_FilenameIn );

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Adding column..." ), sl_Input.count() );

    if ( ( s_HeaderText.isEmpty() == true ) && ( s_ColumnText.isEmpty() == true ) )
        b_AddColumn = false;

    if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
    {
        if ( b_AddFilename == true )
            tout << "Event label" << "\t";

        if ( b_AddFullPath == true )
            tout << "Filename" << "\t";

        if ( b_AddOrdinalNumber == true )
            tout << "No" << "\t";

        if ( b_AddColumn == true )
            tout << s_HeaderText << "\t";

        tout << sl_Input.at( i ) << s_EOL;
    }

    stopProgress = incProgress( i_NumOfFiles, ++i );

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
        {
            if ( b_AddFilename == true )
                tout << fi.baseName() << "\t";

            if ( b_AddFullPath == true )
                tout << fi.absoluteFilePath() << "\t";

            if ( b_AddOrdinalNumber == true )
                tout << QString( "%1" ).arg( ++k ) << "\t";

            if ( b_AddColumn == true )
                tout << s_ColumnText << "\t";

            tout << sl_Input.at( i ) << s_EOL;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    if ( b_DeleteInputFile == true )
        removeFile( s_FilenameIn );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-10-11

void MainWindow::doAddColumn()
{
    int         i                   = 0;
    int         err                 = 0;
    int         stopProgress        = 0;

    int         i_dummy_LineNo      = 0;

    QString     s_HeaderText        = "";
    QString     s_ColumnText        = "";

    QString     s_FilenameIn        = "";
    QString     s_FilenameOut       = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doAddDialog( _ADDCOLUMN, gs_ac_HeaderText, gs_ac_ColumnText, i_dummy_LineNo, gb_ac_AddFilename, gb_ac_AddFullPath, gb_ac_AddOrdinalNumber, gb_ac_SkipEmptyLines, gb_ac_SkipCommentLines, gb_ac_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Adding column..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    s_HeaderText = gs_ac_HeaderText;
                    s_HeaderText.replace( "^t", "\t" );
                    s_ColumnText = gs_ac_ColumnText;
                    s_ColumnText.replace( "^t", "\t" );

                    err = addColumn( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, s_HeaderText, s_ColumnText, gb_ac_AddFilename, gb_ac_AddFullPath, gb_ac_AddOrdinalNumber, gb_ac_SkipEmptyLines, gb_ac_SkipCommentLines, gb_ac_DeleteInputFile, gsl_FilenameList.count() );

                    stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
                }
                else
                {
                    err = _FILENOTEXISTS_;
                }
            }

            resetFileProgress( gsl_FilenameList.count() );
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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Adding column was canceled" ) );

    onError( err );
}
