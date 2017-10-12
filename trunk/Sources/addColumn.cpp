/* 2011-10-11                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::addColumn( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL,
                           const QString &s_HeaderText, const QString &s_ColumnText, const bool b_PrependColumn, const bool b_AppendColumn,
                           const bool b_AddFilename, const bool b_AddFullPath, const bool b_AddOrdinalNumber, const bool b_PrependMetadataColumn, const bool b_AppendMetadataColumn,
                           const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const int i_NumOfFiles )
{
    int         i                   = 0;
    int         k                   = 0;
    int         n                   = 0;
    int         stopProgress        = 0;

    bool        b_AddColumnPrepend  = false;
    bool        b_AddColumenAppend  = false;

    QString     s_EOL               = setEOLChar( i_EOL );

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

    if ( ( s_HeaderText.isEmpty() == false ) || ( s_ColumnText.isEmpty() == false ) )
    {
        if ( b_PrependColumn == true )
            b_AddColumnPrepend = true;

        if ( b_AppendColumn == true )
            b_AddColumenAppend = true;
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Adding column..." ), sl_Input.count() );

    if ( b_PrependMetadataColumn == true )
    {
        if ( b_AddFilename == true )
            tout << "Event label" << "\t";

        if ( b_AddFullPath == true )
            tout << "Filename" << "\t";

        if ( b_AddOrdinalNumber == true )
            tout << "No" << "\t";
    }

    if ( b_AddColumnPrepend == true )
        tout << s_HeaderText << "\t";

    tout << sl_Input.at( 0 );

    if ( b_AppendMetadataColumn == true )
    {
        if ( b_AddFilename == true )
            tout << "\t" << "Event label";

        if ( b_AddFullPath == true )
            tout << "\t"<< "Filename";

        if ( b_AddOrdinalNumber == true )
            tout << "\t" << "No";
    }

    if ( b_AddColumenAppend == true )
        tout << "\t" << s_HeaderText;

    tout << s_EOL;

    stopProgress = incProgress( i_NumOfFiles, ++i );

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
        {
            if ( b_PrependMetadataColumn == true )
            {
                if ( b_AddFilename == true )
                    tout << fi.baseName() << "\t";

                if ( b_AddFullPath == true )
                    tout << fi.absoluteFilePath() << "\t";

                if ( b_AddOrdinalNumber == true )
                    tout << QString( "%1" ).arg( ++k ) << "\t";
            }

            if ( b_AddColumnPrepend == true )
                tout << s_ColumnText << "\t";

            tout << sl_Input.at( i );

            if ( b_AppendMetadataColumn == true )
            {
                if ( b_AddFilename == true )
                    tout << "\t" << fi.baseName();

                if ( b_AddFullPath == true )
                    tout << "\t" << fi.absoluteFilePath();

                if ( b_AddOrdinalNumber == true )
                {
                    if ( b_PrependMetadataColumn == false )
                        tout << "\t" << QString( "%1" ).arg( ++k );
                    else
                        tout << "\t" << QString( "%1" ).arg( k );
                }
            }

            if ( b_AddColumenAppend == true )
                tout << "\t" << s_ColumnText;

            tout << s_EOL;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

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

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doAddDialog( _ADDCOLUMN, gs_ac_HeaderText, gs_ac_ColumnText, i_dummy_LineNo, gb_ac_PrependColumn, gb_ac_AppendColumn, gb_ac_AddFilename, gb_ac_AddFullPath, gb_ac_AddOrdinalNumber, gb_ac_PrependMetadataColumn, gb_ac_AppendMetadataColumn, gb_ac_SkipEmptyLines, gb_ac_SkipCommentLines, gb_ac_DeleteInputFile ) == QDialog::Accepted )
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

                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = addColumn( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, s_HeaderText, s_ColumnText, gb_ac_PrependColumn, gb_ac_AppendColumn, gb_ac_AddFilename, gb_ac_AddFullPath, gb_ac_AddOrdinalNumber, gb_ac_PrependMetadataColumn, gb_ac_AppendMetadataColumn, gb_ac_SkipEmptyLines, gb_ac_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_ac_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Adding column was canceled" ) );

    onError( err );
}
