/* 2008-07-06                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::deleteMatchedColumns( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const QString &SearchString, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const int i_NumOfFiles )
{
    int         i                       = 0;
    int         j                       = 0;
    int         n                       = 0;
    int         m                       = 0;
    int         stopProgress            = 0;

    QString     s_Output                = "";
    QString     s_EOL                   = setEOLChar( i_EOL );

    QStringList sl_Input;

    QList<int>  il_ColumnList;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

    if ( ( m = NumOfSections( sl_Input.at( 0 ) ) ) < 1 )
        return( -80 );

    for ( int j=0; j<m; j++ )
    {
        if ( sl_Input.at( 0 ).section( "\t", j, j ).contains( SearchString ) == false )
            il_ColumnList.append( j );
    }

    if ( il_ColumnList.count() < 1 )
    {
        QFile::remove( s_FilenameOut ); // if exists from older run
        return( -81 );
    }

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Deleting columns..." ), sl_Input.count() );

    while ( ( i < n )  && ( stopProgress != _APPBREAK_ ) )
    {
        m			= NumOfSections( sl_Input.at( i ) );
        s_Output	= "";

        if ( il_ColumnList.count() > 0 )
        {
            if ( il_ColumnList.at( 0 ) < m )
                s_Output = sl_Input.at( i ).section( "\t", il_ColumnList.at( 0 ), il_ColumnList.at( 0 ) );
        }

        j = 1;
        while ( j < il_ColumnList.count() )
        {
            if ( il_ColumnList.at( j ) < m )
                s_Output += "\t" + sl_Input.at( i ).section( "\t", il_ColumnList.at( j ), il_ColumnList.at( j ) );

            j++;
        }

        if ( LineCanBeWritten( s_Output, b_SkipEmptyLines, b_SkipCommentLines ) == true )
            tout << s_Output << s_EOL;

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

void MainWindow::doDeleteMatchedColumns()
{
    int         i                               = 0;
    int         err                             = 0;
    int         stopProgress                    = 0;

    QString     s_dummy_ReplaceString           = "";
    QString     s_dummy_DatabaseName            = "";

    int         i_dummy_StartLine               = 1;
    int         i_dummy_NumberOfLines           = 0;
    int         i_dummy_SearchAndReplaceMode    = _LINE;

    bool        b_dummy_SaveFirstLine           = false;
    bool        b_dummy_SaveNoMatch             = false;
    bool        b_dummy_DeleteEmptyOutputFile   = false;

    QString     s_FilenameIn                    = "";
    QString     s_FilenameOut                   = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSearchDialog( _DELETECOLUMNS, gs_dmc_DeleteColumnsSearchString, s_dummy_ReplaceString, s_dummy_DatabaseName, i_dummy_StartLine, i_dummy_NumberOfLines, i_dummy_SearchAndReplaceMode, b_dummy_SaveFirstLine, b_dummy_SaveNoMatch, gb_dmc_SkipEmptyLines, gb_dmc_SkipCommentLines, b_dummy_DeleteEmptyOutputFile, gb_dmc_DeleteInputFile ) == QDialog::Accepted )
        {
            if ( gs_dmc_DeleteColumnsSearchString.isEmpty() == false )
            {
                initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Deleting columns..." ) );

                while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
                {
                    if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                    {
                        sl_FilenameListDelete.append( s_FilenameIn );

                        err = deleteMatchedColumns( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gs_dmc_DeleteColumnsSearchString, gb_dmc_SkipEmptyLines, gb_dmc_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_dmc_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Deleting columns was canceled" ) );

    onError( err );
}

