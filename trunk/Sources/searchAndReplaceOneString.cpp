/* 2010-10-05                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::searchAndReplaceOneString( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL,
                                           const QString &SearchStrIn, const QString &ReplaceStrIn, const int i_StartLine, const int i_NumberOfLines,
                                           const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const int i_NumOfFiles )
{
    int         i               = 0;
    int         n               = 0;
    int         s               = 0;
    int         e               = 0;

    int         stopProgress    = 0;

    QString     s_Out           = "";
    QString     s_EOL           = setEOLChar( i_EOL );

    QString     SearchStr       = SearchStrIn;
    QString     ReplaceStr      = ReplaceStrIn;

    QStringList sl_Input;

// **********************************************************************************************

    if ( SearchStr.isEmpty() == true )
        return( _ERROR_ );

    SearchStr.replace( "^t", "\t" );
    ReplaceStr.replace( "^t", "\t" );
    SearchStr.replace( "^z", "" );
    ReplaceStr.replace( "^z", "" );
    SearchStr.replace( "^n", "\n" );
    ReplaceStr.replace( "^n", "\n" );

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Search and replacing one string..." ), n );

    s = i_StartLine - 1;

    if ( i_NumberOfLines == 0 )
        e = n;
    else
        e = qMin( n, s + i_NumberOfLines );

    while ( ( i<s ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
            tout << sl_Input.at( i ) << s_EOL;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    while ( ( i<e ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
        {
            s_Out = sl_Input.at( i );
            s_Out.replace( SearchStr, ReplaceStr );
            tout << s_Out << s_EOL;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
            tout << sl_Input.at( i ) << s_EOL;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

    fout.close();

// **********************************************************************************************

    if ( stopProgress == _APPBREAK_ )
    {
        fout.remove();
        return( _APPBREAK_ );
    }

    return( stopProgress );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doSearchAndReplaceOneString()
{
    int     i                               = 0;
    int     err                             = 0;
    int     stopProgress                    = 0;

    QString s_dummy_DatabaseName            = "";

    QString s_FilenameIn                    = "";
    QString s_FilenameOut                   = "";

    int     i_dummy_SearchAndReplaceMode    = _LINE;

    bool    b_dummy_SaveFirstLine           = false;  // not used
    bool    b_dummy_SaveNoMatch             = false;  // not used
    bool    b_dummy_DeleteEmptyOutputFile   = false;  // not used

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSearchDialog( _SEARCHANDREPLACEONESTRING, gs_srs_SearchString, gs_srs_ReplaceString, s_dummy_DatabaseName, gi_srs_StartLine, gi_srs_NumberOfLines, i_dummy_SearchAndReplaceMode, b_dummy_SaveFirstLine, b_dummy_SaveNoMatch, gb_srs_SkipEmptyLines, gb_srs_SkipCommentLines, b_dummy_DeleteEmptyOutputFile, gb_srs_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Search and replacing one string..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = searchAndReplaceOneString( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gs_srs_SearchString, gs_srs_ReplaceString, gi_srs_StartLine, gi_srs_NumberOfLines, gb_srs_SkipEmptyLines, gb_srs_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_srs_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Search and replacing one string was canceled" ) );

    onError( err );
}

