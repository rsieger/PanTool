/* 2008-07-06                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2013-11-29

int MainWindow::extractMatchedColumns( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const QString &SearchString, const bool b_SaveNoMatch, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const bool b_DeleteEmptyOutputFile, const int i_NumOfFiles )
{
    int         i                       = 0;
    int         j                       = 0;
    int         n                       = 0;
    int         m                       = 0;
    int         stopProgress            = 0;

    QString     s_Output                = "";
    QString     s_OutputNoMatch         = "";
    QString     s_FilenameNoMatch       = "";
    QString     s_EOL                   = setEOLChar( i_EOL );

    QStringList sl_Input;

    QList<int>  il_ColumnList;
    QList<int>  il_ColumnListNoMatch;

// **********************************************************************************************

    QFileInfo fi( s_FilenameOut );

    s_FilenameNoMatch = fi.absolutePath() + "/" + fi.completeBaseName() + "_nomatch." + fi.suffix();

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

    if ( ( m = NumOfSections( sl_Input.at( 0 ) ) ) < 1 )
        return( -80 );

    for ( int j=0; j<m; j++ )
    {
        if ( sl_Input.at( 0 ).section( "\t", j, j ).contains( SearchString ) == true )
            il_ColumnList.append( j );
        else
            il_ColumnListNoMatch.append( j );
    }

// **********************************************************************************************
// open output file

    QFile::remove( s_FilenameOut ); // if exists from older run
    QFile::remove( s_FilenameNoMatch ); // if exists from older run

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

// **********************************************************************************************
// open no match output file

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Extracting columns..." ), sl_Input.count() );

    while ( ( i < n )  && ( stopProgress != _APPBREAK_ ) )
    {
        m				= NumOfSections( sl_Input.at( i ) );
        s_Output		= "";
        s_OutputNoMatch	= "";

        if ( il_ColumnList.count() > 0 )
        {
            if ( il_ColumnList.at( 0 ) < m )
                s_Output = sl_Input.at( i ).section( "\t", il_ColumnList.at( 0 ), il_ColumnList.at( 0 ) );
        }

        if ( ( b_SaveNoMatch == true ) && ( il_ColumnListNoMatch.count() > 0 ) )
        {
            if ( il_ColumnListNoMatch.at( 0 ) < m )
                s_OutputNoMatch = sl_Input.at( i ).section( "\t", il_ColumnListNoMatch.at( 0 ), il_ColumnListNoMatch.at( 0 ) );
        }

        j = 1;
        while ( j < il_ColumnList.count() )
        {
            if ( il_ColumnList.at( j ) < m )
                s_Output += "\t" + sl_Input.at( i ).section( "\t", il_ColumnList.at( j ), il_ColumnList.at( j ) );

            j++;
        }

        if ( b_SaveNoMatch == true )
        {
            j = 1;
            while ( j < il_ColumnListNoMatch.count() )
            {
                if ( il_ColumnListNoMatch.at( j ) < m )
                    s_OutputNoMatch += "\t" + sl_Input.at( i ).section( "\t", il_ColumnListNoMatch.at( j ), il_ColumnListNoMatch.at( j ) );

                j++;
            }
        }

        if ( LineCanBeWritten( s_Output, b_SkipEmptyLines, b_SkipCommentLines ) == true )
            tout << s_Output << s_EOL;

        if ( ( b_SaveNoMatch == true ) && ( LineCanBeWritten( s_OutputNoMatch, true, b_SkipCommentLines ) == true ) )
            toutnomatch << s_OutputNoMatch << s_EOL;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();
    foutnomatch.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    if ( ( il_ColumnList.count() < 1 ) && ( b_DeleteEmptyOutputFile == true ) )
        QFile::remove( s_FilenameOut );

    if ( b_SaveNoMatch == false )
        QFile::remove( s_FilenameNoMatch );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doExtractMatchedColumns()
{
    int         i                            = 0;
    int         err                          = 0;
    int         stopProgress                 = 0;

    int         i_dummy_StartLine            = 1;
    int         i_dummy_NumberOfLines        = 0;

    int         i_dummy_SearchAndReplaceMode = _LINE;

    bool        b_dummy_SaveFirstLine        = true;

    QString     s_dummy_ReplaceString        = "";
    QString     s_dummy_DatabaseName         = "";

    QString     s_FilenameIn                 = "";
    QString     s_FilenameOut                = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSearchDialog( _EXTRACTCOLUMNS, gs_emc_ExtractColumnsSearchString, s_dummy_ReplaceString, s_dummy_DatabaseName, i_dummy_StartLine, i_dummy_NumberOfLines, i_dummy_SearchAndReplaceMode, b_dummy_SaveFirstLine, gb_emc_SaveNoMatch, gb_emc_SkipEmptyLines, gb_emc_SkipCommentLines, gb_emc_DeleteEmptyOutputFile, gb_emc_DeleteInputFile ) == QDialog::Accepted )
        {
            if ( gs_emc_ExtractColumnsSearchString.isEmpty() == false )
            {
                initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Extracting columns..." ) );

                while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
                {
                    if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                    {
                        sl_FilenameListDelete.append( s_FilenameIn );

                        err = extractMatchedColumns( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gs_emc_ExtractColumnsSearchString, gb_emc_SaveNoMatch, gb_emc_SkipEmptyLines, gb_emc_SkipCommentLines, gb_emc_DeleteEmptyOutputFile, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_emc_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Extracting columns was canceled" ) );

    onError( err );
}

