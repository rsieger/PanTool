/* 2011-10-06                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::splitFileByLines( const QString &s_FilenameIn, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_Extension, const int i_NumberOfLines, const int i_NumberOfHeaderLines, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const int i_NumOfFiles )
{
    int         i               = 0;
    int         j               = 0;
    int         n               = 0;
    int         m               = 0;
    int         e               = 0;
    int         i_fieldWidth    = 1;

    int         stopProgress    = 0;

    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;

    QFile       fout;

// **********************************************************************************************
// read file

    QFileInfo fi( s_FilenameIn );

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

    if ( n > i_NumberOfLines )
        m = n/i_NumberOfLines + 1;
    else
        m = 1;

    if ( ( m > 9 ) && ( m < 100 ) ) i_fieldWidth = 2;
    if ( ( m > 99 ) && ( m < 1000 ) ) i_fieldWidth = 3;
    if ( ( m > 999 ) && ( m < 10000 ) ) i_fieldWidth = 4;
    if ( m > 9999 ) return( -84 );

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Split file..." ), sl_Input.count() );

    while ( ( ++j <= m )  && ( stopProgress != _APPBREAK_ ) )
    {
        fout.setFileName( fi.absolutePath() + "/" + fi.completeBaseName() + QString( "_%1" ).arg( j, i_fieldWidth, 10, QLatin1Char( '0' ) ) + setExtension( i_Extension ) );

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

        if ( j > 1 )
        {
            int k = 0;
            while ( ( k < i_NumberOfHeaderLines ) && ( k < n ) )
                tout << sl_Input.at( k++ ) << s_EOL;
        }

        e = qMin( j*i_NumberOfLines, n );

        while ( ( i<e ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
                tout << sl_Input.at( i ) << s_EOL;

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }

        fout.close();
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-01-31

int MainWindow::splitLargeFile( const QString &s_FilenameIn, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_Extension, const int i_NumberOfHeaderLines, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const int i_NumOfFiles )
{
    int             i                   = 0;
    int             j                   = 0;

    unsigned int	ui_length           = 1;
    unsigned int	ui_filesize         = 1;

    unsigned int    ui_maxFilesize      = 100000000;
    unsigned int    ui_OutputFilesize   = 1;
    unsigned int    ui_maxNumOfLines    = 1000000;
    unsigned int    ui_NumOfLines       = 1;

    bool            b_HeaderOnly        = false;

    QString         InputStr            = "";
    QString         s_EOL               = setEOLChar( i_EOL );

    QStringList     sl_Header;

    QFile           fout;

// **********************************************************************************************

    QFileInfo fi( s_FilenameIn );
    QFile fin( s_FilenameIn );

// **********************************************************************************************
// open input file

    ui_filesize = fin.size();

    if ( b_HeaderOnly == false )
    {
        if ( ui_filesize < ui_maxFilesize )
            return( _NOERROR_ );
    }

    if ( fin.open( QIODevice::ReadOnly | QIODevice::Text ) == false )
        return( -10 );

    QTextStream tin( &fin );

    switch ( i_CodecInput )
    {
    case _SYSTEM_:
        break;
    case 1:
        tin.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case 2:
        tin.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tin.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    while ( ( tin.atEnd() == false ) && ( ++i <= i_NumberOfHeaderLines ) && ( ui_NumOfLines < ui_maxNumOfLines ) && ( ui_OutputFilesize < ui_maxFilesize ) && ( ui_length != (unsigned int) _APPBREAK_ ) )
    {
        InputStr  = tin.readLine();

        ui_maxNumOfLines--;

        sl_Header.append( InputStr );
    }

// ************

    if ( b_HeaderOnly == true )
    {
        fout.setFileName( fi.absolutePath() + "/" + fi.completeBaseName() + "_Header" + setExtension( i_Extension ) );

        if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
            return( -20 );

        QTextStream tout( &fout );

        switch ( i_CodecOutput )
        {
        case _SYSTEM_:
            break;
        case 1:
            tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
            break;
        case 2:
            tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
            break;
        default:
            tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
            break;
        }

        for ( i=0; i < i_NumberOfHeaderLines; i++ )
            tout << sl_Header.at( i ) << s_EOL;

//      tout << NumOfSections( sl_Header.at( 0 ) ) << s_EOL;

        fout.close();

        return( _NOERROR_ );
    }

// ***********

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Spliting large file..." ), 100 );
    ui_length = incProgress( i_NumOfFiles, ui_filesize, ui_length, sl_Header.join( "\t" ) );

    while ( ( tin.atEnd() == false ) && ( ui_length != (unsigned int) _APPBREAK_ ) )
    {
        fout.setFileName( fi.absolutePath() + "/" + fi.completeBaseName() + QString( "_%1" ).arg( ++j, 3, 10, QLatin1Char( '0' ) ) + setExtension( i_Extension ) );

        if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
            return( -20 );

        QTextStream tout( &fout );

        switch ( i_CodecOutput )
        {
        case _SYSTEM_:
            break;
        case 1:
            tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
            break;
        case 2:
            tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
            break;
        default:
            tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
            break;
        }

        ui_OutputFilesize = 1;
        ui_NumOfLines     = 1;
        i                 = 0;

        for ( i=0; i < i_NumberOfHeaderLines; i++ )
            tout << sl_Header.at( i ) << s_EOL;

        while ( ( tin.atEnd() == false ) && ( ui_NumOfLines <= ui_maxNumOfLines ) && ( ui_OutputFilesize < ui_maxFilesize ) && ( ui_length != (unsigned int) _APPBREAK_ ) )
        {
            InputStr  = tin.readLine();
            ui_length = incProgress( i_NumOfFiles, ui_filesize, ui_length, InputStr );

            if ( LineCanBeWritten( InputStr, b_SkipEmptyLines, b_SkipCommentLines ) == true )
            {
                tout << InputStr << s_EOL;

                ui_NumOfLines++;
                ui_OutputFilesize += (unsigned int) InputStr.length();
            }
        }

        fout.close();
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    if ( ui_length == (unsigned int) _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doSplitFilesByLines()
{
    int     i                               = 0;
    int     err                             = 0;
    int     stopProgress                    = 0;

    int     i_dummy_NumberOfColumns         = 0;

    QString s_dummy_FixedColumnsList        = "";

    QString s_FilenameIn                    = "";
    QString s_FilenameOut                   = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSplitFileDialog( _SPLITBYLINES, gi_sfl_NumberOfLines, gi_sfl_NumberOfHeaderLines, i_dummy_NumberOfColumns, s_dummy_FixedColumnsList, gb_sfl_SkipEmptyLines, gb_sfl_SkipCommentLines, gb_sfl_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Split files..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = splitFileByLines( s_FilenameIn, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_Extension, gi_sfl_NumberOfLines, gi_sfl_NumberOfHeaderLines, gb_sfl_SkipEmptyLines, gb_sfl_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_sfl_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Split files was canceled" ), true, false );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doSplitLargeFiles()
{
    int     i                               = 0;
    int     err                             = 0;
    int     stopProgress                    = 0;

    int     i_dummy_NumberOfLines           = 0;
    int     i_dummy_NumberOfColumns         = 0;

    QString s_dummy_FixedColumnsList        = "";

    QString s_FilenameIn                    = "";
    QString s_FilenameOut                   = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSplitFileDialog( _SPLITLARGE, i_dummy_NumberOfLines, gi_sfl_NumberOfHeaderLines, i_dummy_NumberOfColumns, s_dummy_FixedColumnsList, gb_sfl_SkipEmptyLines, gb_sfl_SkipCommentLines, gb_sfl_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Split large files..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = splitLargeFile( s_FilenameIn, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_Extension, gi_sfl_NumberOfHeaderLines, gb_sfl_SkipEmptyLines, gb_sfl_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_sfl_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Split large files was canceled" ), true, false );

    onError( err );
}

