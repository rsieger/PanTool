/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::deleteCommentBlocks( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const int i_NumOfFiles )
{
    int         i               = 0;
    int         n               = 0;

    int         stopProgress    = 0;

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

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Deleting comment blocks..." ), sl_Input.count() );

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).contains( "/*" ) == true )
        {
            if ( LineCanBeWritten( sl_Input.at( i ).section( "/*", 0, 0 ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
                tout << sl_Input.at( i ).section( "/*", 0, 0 );

            if ( sl_Input.at( i ).contains( "*/" ) == true )
            {
                if ( LineCanBeWritten( sl_Input.at( i ).section( "*/", 1, 1 ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
                    tout << sl_Input.at( i ).section( "*/", 1, 1 );

                tout << s_EOL;
            }
            else
            {
                while ( ( i < n ) && ( sl_Input.at( i ).contains( "*/" ) == false ) )
                    i++;

                if ( LineCanBeWritten( sl_Input.at( i ).section( "*/", 1, 1 ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
                    if ( sl_Input.at( i ).section( "*/", 1, 1 ).isEmpty() == false )
                        tout << sl_Input.at( i ).section( "*/", 1, 1 ) << s_EOL;
            }

            i++;
        }

        if ( ( i < n ) && ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true ) )
            tout << sl_Input.at( i ) << s_EOL;

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
// 2008-04-07

void MainWindow::doDeleteCommentBlocks()
{
    int     i                     = 0;
    int     err                   = 0;
    int     stopProgress          = 0;

    bool    b_dummy_SkipFirstLine = false;

    QString s_FilenameIn          = "";
    QString s_FilenameOut         = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSkipLinesDialog( _DELETECOMMENTBLOCKS, b_dummy_SkipFirstLine, gb_dcl_SkipEmptyLines, gb_dcl_SkipCommentLines, gb_dcl_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Deleting comment blocks..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = deleteCommentBlocks( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gb_dcl_SkipEmptyLines, gb_dcl_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_dcl_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Deleting comment blocks was canceled" ) );

    onError( err );
}

