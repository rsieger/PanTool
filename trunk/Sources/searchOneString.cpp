/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::searchOneString( const int i_ActionNumber, const QString &s_FilenameFormat, const int i_Extension, const QStringList &sl_FilenameList,
                                 const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const QString &SearchStr,
                                 const int i_StartLine, const int i_NumberOfLines, const bool b_SkipEmptyLines, const bool b_SkipCommentLines,
                                 const bool b_DeleteEmptyOutputFile )
{
    int         j               = 0;

    int         s               = 0;
    int         e               = 0;
    int         n               = 0;

    int         stopProgress    = 0;

    bool        b_isEmpty       = true;

    QString     s_FilenameIn    = "";
    QString     s_Dummy         = "";
    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;

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

    setWaitCursor();

    initFileProgress( sl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Searching one string..." ) );

// **********************************************************************************************
// read file

    tout << tr( "Filename" ) << "\t" << tr( "Line" ) << "\t" << tr( "String" ) << s_EOL;

    while ( ( j < sl_FilenameList.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( buildFilename( i_ActionNumber, s_FilenameFormat, i_Extension, sl_FilenameList.at( j ), s_FilenameIn, s_Dummy ) == true )
        {
            if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) > 0 )
            {
                initProgress( sl_FilenameList.count(), s_FilenameIn, tr( "Searching one string..." ), n );

                s = i_StartLine - 1;

                if ( i_NumberOfLines == 0 )
                    e = n;
                else
                    e = qMin( n, s + i_NumberOfLines );

                for ( int i=s; i<e; i++ )
                {
                    if ( ( sl_Input.at( i ).contains( SearchStr ) == true ) && ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true ) )
                    {
                        tout << QDir::toNativeSeparators( s_FilenameIn ) << "\t" << i+1 << "\t" << sl_Input.at( i ) << s_EOL;

                        b_isEmpty = false;
                    }

                    incProgress( sl_FilenameList.count(), i );
                }

                resetProgress( sl_FilenameList.count() );
            }
        }

        stopProgress = incFileProgress( sl_FilenameList.count(), ++j );
    }


// **********************************************************************************************

    fout.close();

    resetFileProgress( sl_FilenameList.count() );

    setNormalCursor();

// **********************************************************************************************

    if ( stopProgress == _APPBREAK_ )
        fout.remove();

    if ( ( b_isEmpty == true ) && ( b_DeleteEmptyOutputFile == true ) && ( stopProgress != _APPBREAK_ ) )
    {
        fout.remove();
        stopProgress = -82; // no match
    }

    if ( ( b_isEmpty == true ) && ( b_DeleteEmptyOutputFile == false ) && ( stopProgress != _APPBREAK_ ) )
        stopProgress = -83; // no match

    return( stopProgress );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doSearchOneString()
{
    int     err                             = 0;
    int     stopProgress                    = 0;

    QString s_FilenameOut                   = "";

    int     i_dummy_SearchAndReplaceMode    = _LINE;

    QString s_dummy_ReplaceString           = "";
    QString s_dummy_DatabaseName            = "";

    bool    b_dummy_SaveFirstLine           = false;  // not used
    bool    b_dummy_SaveNoMatch             = false;  // not used


// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( gsl_FilenameList.count() > 0 )
        {
            QFileInfo fi( gsl_FilenameList.at( 0 ) );

            s_FilenameOut = fi.absolutePath() + "/" + fi.absolutePath().section( "/", -1, -1 ) + "_ResultOfSearch" + setExtension( gi_Extension );

            if ( doSearchDialog( _SEARCHONESTRING, gs_sos_SearchString, s_dummy_ReplaceString, s_dummy_DatabaseName, gi_sos_StartLine, gi_sos_NumberOfLines, i_dummy_SearchAndReplaceMode, b_dummy_SaveFirstLine, b_dummy_SaveNoMatch, gb_sos_SkipEmptyLines, gb_sos_SkipCommentLines, gb_sos_DeleteEmptyOutputFile, gb_sos_DeleteInputFile ) == QDialog::Accepted )
                stopProgress = searchOneString( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gs_sos_SearchString, gi_sos_StartLine, gi_sos_NumberOfLines, gb_sos_SkipEmptyLines, gb_sos_SkipCommentLines, gb_sos_DeleteEmptyOutputFile );
            else
                stopProgress = _CHOOSEABORTED_;
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

    if ( ( stopProgress < 0 ) || ( stopProgress == _CHOOSEABORTED_ ) )
        err = stopProgress;

// **********************************************************************************************
/*
    if ( err == _NOERROR_ )
    {
        clearFilenameList( gi_ActionNumber, gsl_FilenameList );

        QFileInfo fi( s_FilenameOut );
        if ( ( fi.exists() == true ) && ( fi.isFile() == true ) )
            gsl_FilenameList.append( s_FilenameOut );
    }
*/
// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Searching one string was canceled" ), false, false );

    onError( err );
}

