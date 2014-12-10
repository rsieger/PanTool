/* 2010-10-05                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::searchAndReplaceManyStringsAtOnce( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL,
                                                   const QStringList &sl_SearchStings, const QStringList &sl_ReplaceStings, const int i_StartLine, const int i_NumberOfLines,
                                                   const int i_SearchAndReplaceMode, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const bool b_DeleteInputFile,
                                                   const int i_NumOfFiles )
{
    int         i               = 0;
    int         n               = 0;
    int         m               = 0;
    int         s               = 0;
    int         e               = 0;

    int         stopProgress    = 0;

    QString     s_Out           = "";
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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Search and replacing one string..." ), n );

    m = sl_SearchStings.count();
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

    if ( i_SearchAndReplaceMode == _LINE )
    {
        while ( ( i<e ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
            {
                s_Out = sl_Input.at( i );

                if ( s_Out.isEmpty() == false )
                {
                    for ( int j=0; j<m; j++ )
                        s_Out.replace( sl_SearchStings.at( j ), sl_ReplaceStings.at( j ) );
                }

                tout << s_Out << s_EOL;
            }

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }
    }
    else
    {
        while ( ( i<e ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
            {
                int i_NumberOfSections = NumOfSections( sl_Input.at( i ) );

                s_Out = sl_Input.at( i ).section( "\t", 0, 0 );  // first column

                if ( s_Out.isEmpty() == false )
                {
                    for ( int j=0; j<m; j++ )
                    {
                        if ( s_Out == sl_SearchStings.at( j ) )
                            s_Out = sl_ReplaceStings.at( j );
                    }
                }

                tout << s_Out;

                for ( int k=1; k<i_NumberOfSections; k++ )  // all columns
                {
                    s_Out = sl_Input.at( i ).section( "\t", k, k );

                    if ( s_Out.isEmpty() == false )
                    {
                        for ( int j=0; j<m; j++ )
                        {
                            if ( s_Out == sl_SearchStings.at( j ) )
                                s_Out = sl_ReplaceStings.at( j );
                        }
                    }

                    tout << "\t" << s_Out;
                }

                tout << s_EOL;
            }

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }
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
        fout.remove();

    if ( ( b_DeleteInputFile == true ) && ( stopProgress != _APPBREAK_ ) )
    {
        QFile fin( s_FilenameIn );
        fin.remove();
    }

    return( stopProgress );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doSearchAndReplaceManyStringsAtOnce()
{
    int     i                               = 0;
    int     n                               = 0;
    int     err                             = 0;
    int     stopProgress                    = 0;

    QString s_dummy_SearchString            = "";
    QString s_dummy_ReplaceString           = "";

    bool    b_dummy_SaveFirstLine           = false;  // not used
    bool    b_dummy_SaveNoMatch             = false;  // not used
    bool    b_dummy_DeleteEmptyOutputFile   = false;  // not used

    QString s_FilenameIn                    = "";
    QString s_FilenameOut                   = "";

    QStringList sl_Input;
    QStringList sl_SearchStrings;
    QStringList sl_ReplaceStrings;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSearchDialog( _SEARCHANDREPLACEMANYSTRINGSATONCE, s_dummy_SearchString, s_dummy_ReplaceString, gs_srm_DatabaseName, gi_srm_StartLine, gi_srm_NumberOfLines, gi_srm_SearchAndReplaceMode, b_dummy_SaveFirstLine, b_dummy_SaveNoMatch, gb_srm_SkipEmptyLines, gb_srm_SkipCommentLines, b_dummy_DeleteEmptyOutputFile, gb_srm_DeleteInputFile ) == QDialog::Accepted )
        {
            QFileInfo fi( gs_srm_DatabaseName );

            if ( ( fi.isFile() == true ) && ( fi.exists() == true ) )
            {
                if ( ( n = readFile( gs_srm_DatabaseName, sl_Input, gi_CodecInput ) ) > 0 )
                {
                    sl_SearchStrings.clear();
                    sl_ReplaceStrings.clear();

                    for ( int j=0; j<sl_Input.count(); j++ )
                    {
                        s_dummy_SearchString  = sl_Input.at( j ).section( "\t", 0, 0 );
                        s_dummy_ReplaceString = sl_Input.at( j ).section( "\t", 1, 1 );

                        s_dummy_SearchString.replace( "^t", "\t" );
                        s_dummy_ReplaceString.replace( "^t", "\t" );
                        s_dummy_SearchString.replace( "^z", "" );
                        s_dummy_ReplaceString.replace( "^z", "" );
                        s_dummy_SearchString.replace( "^n", "\n" );
                        s_dummy_ReplaceString.replace( "^n", "\n" );

                        if ( ( s_dummy_SearchString.isEmpty() == false ) && ( s_dummy_SearchString != s_dummy_ReplaceString ) )
                        {
                            sl_SearchStrings.append( s_dummy_SearchString );
                            sl_ReplaceStrings.append( s_dummy_ReplaceString );
                        }
                    }
                }

                if ( sl_SearchStrings.count() > 0 )
                {
                    initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Search and replacing many strings at once..." ) );

                    while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
                    {
                        if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                        {
                            stopProgress = searchAndReplaceManyStringsAtOnce( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, sl_SearchStrings, sl_ReplaceStrings, gi_srm_StartLine, gi_srm_NumberOfLines, gi_srm_SearchAndReplaceMode, gb_srm_SkipEmptyLines, gb_srm_SkipCommentLines, gb_srm_DeleteInputFile, gsl_FilenameList.count() );

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
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Search and replacing many strings at once was canceled" ) );

    onError( err );
}


