/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::extractMatchedLines( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const QString &s_SearchString, const bool b_SaveFirstLine, const bool b_SaveNoMatch, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const bool b_DeleteEmptyOutputFile, const int i_NumOfFiles )
{
    int         i                   = 0;
    int         n                   = 0;

    int         stopProgress        = 0;

    bool        b_SearchStringFound = false;

    QString     s_FilenameNoMatch   = "";
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

// **********************************************************************************************
// open no match output file

    QFileInfo fi( s_FilenameOut );

    s_FilenameNoMatch = fi.absolutePath() + "/" + fi.completeBaseName() + "_nomatch." + fi.suffix();

    QFile foutnomatch( s_FilenameNoMatch );

    if ( foutnomatch.open( QIODevice::WriteOnly | QIODevice::Text) == false )
    {
        fout.close();
        return( -20 );
    }

    QTextStream toutnomatch( &foutnomatch );

// **********************************************************************************************

    switch ( i_CodecOutput )
    {
    case _SYSTEM_:
        break;
    case _LATIN1_:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        toutnomatch.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case _APPLEROMAN_:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        toutnomatch.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        toutnomatch.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Extracting lines..." ), sl_Input.count() );

    if ( b_SaveFirstLine == true )
    {
        tout << sl_Input.at( 0 ) << s_EOL;

        if ( b_SaveNoMatch == true )
            toutnomatch << sl_Input.at( 0 ) << s_EOL;

        i++;
    }

    while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).contains( s_SearchString ) == true )
        {
            if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
            {
                tout << sl_Input.at( i ) << s_EOL;

                b_SearchStringFound = true;
            }
        }
        else
        {
            if ( ( b_SaveNoMatch == true ) && ( LineCanBeWritten( sl_Input.at( i ), true, b_SkipCommentLines ) == true ) )
                toutnomatch << sl_Input.at( i ) << s_EOL;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();
    foutnomatch.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    if ( b_SaveNoMatch == false )
        QFile::remove( s_FilenameNoMatch );

    if ( ( b_SearchStringFound == false ) && ( b_DeleteEmptyOutputFile == true ) )
    {
        QFile::remove( s_FilenameOut );
        return( -82 );
    }

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doExtractMatchedLines()
{
    int     i                            = 0;
    int     err                          = 0;
    int     stopProgress                 = 0;

    int     i_dummy_SearchAndReplaceMode = _LINE;

    QString s_dummy_ReplaceString        = "";
    QString s_dummy_DatabaseName         = "";

    QString s_FilenameIn                 = "";
    QString s_FilenameOut                = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSearchDialog( _EXTRACTLINES, gs_eml_ExtractLinesSearchString, s_dummy_ReplaceString, s_dummy_DatabaseName, gi_eml_StartLine, gi_eml_NumberOfLines, i_dummy_SearchAndReplaceMode, gb_eml_SaveFirstLine, gb_eml_SaveNoMatch, gb_eml_SkipEmptyLines, gb_eml_SkipCommentLines, gb_eml_DeleteEmptyOutputFile, gb_eml_DeleteInputFile ) == QDialog::Accepted )
        {
            if ( gs_eml_ExtractLinesSearchString.isEmpty() == false )
            {
                initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Extracting lines..." ) );

                while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
                {
                    if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                    {
                        sl_FilenameListDelete.append( s_FilenameIn );

                        err = extractMatchedLines( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gs_eml_ExtractLinesSearchString, gb_eml_SaveFirstLine, gb_eml_SaveNoMatch, gb_eml_SkipEmptyLines, gb_eml_SkipCommentLines, gb_eml_DeleteEmptyOutputFile, gsl_FilenameList.count() );

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
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_eml_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Extracting lines was canceled" ) );

    onError( err );
}

