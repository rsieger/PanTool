/* 2011-10-06                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::splitFileByColumns( const QString &s_FilenameIn, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_Extension, const int i_NumberOfColumns, const int i_NumberOfMetadataColumns, const int i_NumOfFiles )
{
    int         i               = 0;
    int         j               = 0;
    int         n               = 0;
    int         nc              = 0;
    int         m               = 0;
    int         s               = 0;
    int         e               = 0;
    int         ec              = 0;
    int         i_fieldWidth    = 1;

    int         stopProgress    = 0;

    QString     s_Out           = "";
    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;

    QFile       fout;

// **********************************************************************************************
// read file

    QFileInfo fi( s_FilenameIn );

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

    nc = NumOfSections( sl_Input.at( 0 ) );

    if ( nc > i_NumberOfMetadataColumns )
        m = nc - i_NumberOfMetadataColumns;
    else
        m = 1;

    if ( ( m > i_NumberOfColumns ) && ( i_NumberOfColumns > 1 ) )
        m = m/i_NumberOfColumns + 1;

    if ( i_NumberOfMetadataColumns > 0 )
        ec = qMin( i_NumberOfMetadataColumns, nc ) - 1;
    else
        ec = 0;

    if ( ( m > 9 ) && ( m < 100 ) ) i_fieldWidth = 2;
    if ( ( m > 99 ) && ( m < 1000 ) ) i_fieldWidth = 3;
    if ( ( m > 999 ) && ( m < 10000 ) ) i_fieldWidth = 4;
    if ( m > 9999 ) return( -84 );

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Split file..." ), sl_Input.count() );

    while ( ( ++j <= m )  && ( stopProgress != _APPBREAK_ ) )
    {
        if ( s == 0 )
            s = ec + 1;
        else
            s = e + 1;

        e = qMin( s + i_NumberOfColumns, nc + 1 ) - 1;

        if ( s <= e )
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

            i = 0;

            while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
            {
                s_Out.clear();

                if ( ec > 0 ) // i_NumberOfMetadataColumns > 0
                {
                    s_Out.append( sl_Input.at( i ).section( "\t", 0, ec ) + "\t" );
                    s_Out.append( sl_Input.at( i ).section( "\t", s, e ) );
                    tout << s_Out << s_EOL;
                }
                else
                {
                    s_Out.append( sl_Input.at( i ).section( "\t", s-1, e-1 ) );
                    tout << s_Out << s_EOL;
                }

                stopProgress = incProgress( i_NumOfFiles, ++i );
            }

            fout.close();
        }
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
// 2008-04-07

void MainWindow::doSplitFilesByColumns()
{
    int     i                           = 0;
    int     err                         = 0;
    int     stopProgress                = 0;

    int     i_dummy_NumberOfLines       = 0;
    int     i_dummy_NumberOfHeaderLines = 0;

    bool    b_dummy_SkipEmptyLines      = 0;
    bool    b_dummy_SkipCommentLines    = 0;

    QString s_FilenameIn                = "";
    QString s_FilenameOut               = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doSplitFileDialog( _SPLITBYCOLUMNS, i_dummy_NumberOfLines, i_dummy_NumberOfHeaderLines, gi_sfc_NumberOfColumns, gi_sfc_NumberOfMetadataColumns, b_dummy_SkipEmptyLines, b_dummy_SkipCommentLines, gb_sfc_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Split files..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = splitFileByColumns( s_FilenameIn, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_Extension, gi_sfc_NumberOfColumns, gi_sfc_NumberOfMetadataColumns, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_sfc_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Split files was canceled" ), true, false );

    onError( err );
}



