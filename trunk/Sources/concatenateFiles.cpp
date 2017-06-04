/* 2011-07-08                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

int MainWindow::concatenateFilesByLines( const int i_ActionNumber, const QString &s_FilenameFormat, const int i_Extension,
                                         const QStringList &sl_FilenameList, const QString &s_FilenameOut, const int i_CodecInput,
                                         const int i_CodecOutput, const int i_EOL, const int i_SkipNStartLines, const bool b_IncludeFilename,
                                         const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const bool b_DeleteInputFile )
{
    int         i               = 0;
    int         n               = 0;
    int         stopProgress    = 0;

    QString     s_Filename      = "";
    QString     s_FilenameIn    = "";
    QString     s_Dummy         = "";
    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;
    QStringList sl_FilenameListIn;

// **********************************************************************************************

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

    initFileProgress( sl_FilenameList.count(), sl_FilenameList.at( 0 ), tr( "Concatenating files..." ) );

// **********************************************************************************************
// first file

    if ( buildFilename( i_ActionNumber, s_FilenameFormat, i_Extension, sl_FilenameList.at( i ), s_FilenameIn, s_Dummy ) == true )
    {
        if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) > 0 )
        {
            sl_FilenameListIn.append( s_FilenameIn );

            if ( b_IncludeFilename == true )
            {
                QFileInfo fi( s_FilenameIn );
                s_Filename = fi.completeBaseName() + "\t";
            }

            if ( b_IncludeFilename == true )
                tout << "Event label\t";

            tout << sl_Input.at( 0 ) << s_EOL;

            initProgress( sl_FilenameList.count(), s_FilenameIn, tr( "Concatenating files..." ), n );

            for ( int j=1; j<n; j++ )
            {
                if ( LineCanBeWritten( sl_Input.at( j ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
                    tout << s_Filename << sl_Input.at( j ) << s_EOL;

                incProgress( sl_FilenameList.count(), j );
            }

            resetProgress( sl_FilenameList.count() );
        }
    }

    stopProgress = incFileProgress( sl_FilenameList.count(), ++i );

// **********************************************************************************************
// all files

    while ( ( i < sl_FilenameList.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( buildFilename( i_ActionNumber, s_FilenameFormat, i_Extension, sl_FilenameList.at( i ), s_FilenameIn, s_Dummy ) == true )
        {
            sl_FilenameListIn.append( s_FilenameIn );

            if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) > 0 )
            {
                if ( b_IncludeFilename == true )
                {
                    QFileInfo fi( s_FilenameIn );
                    s_Filename = fi.completeBaseName() + "\t";
                }

                initProgress( sl_FilenameList.count(), s_FilenameIn, tr( "Concatenating files..." ), n );

                for ( int j=i_SkipNStartLines; j<n; j++ )
                {
                    if ( LineCanBeWritten( sl_Input.at( j ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
                        tout << s_Filename << sl_Input.at( j ) << s_EOL;

                    incProgress( sl_FilenameList.count(), j );
                }

                resetProgress( sl_FilenameList.count() );

            }
        }

        stopProgress = incFileProgress( sl_FilenameList.count(), ++i );
    }

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
    {
        fout.remove();
    }
    else
    {
        if ( b_DeleteInputFile == true )
        {
            for ( int i=0; i<sl_FilenameListIn.count(); i++ )
                QFile::remove( sl_FilenameListIn.at( i ) );
        }
    }

    resetFileProgress( sl_FilenameList.count() );

    setNormalCursor();

    return( stopProgress );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-11

int MainWindow::concatenateFilesByColumns( const int i_ActionNumber, const QString &s_FilenameFormat, const int i_Extension,
                                           const QStringList &sl_FilenameList, const QString &s_FilenameOut, const int i_CodecInput,
                                           const int i_CodecOutput, const int i_EOL, const int i_SkipNStartLines,
                                           const bool b_IncludeFilename, const bool b_DeleteInputFile )
{
    int         i               = 0;
    int         n               = 0;
    int         n_first         = 0;
    int         e               = 0;
    int         i_maxNumColumn  = 0;
    int         err             = 0;
    int         stopProgress    = 0;

    QString     s_FilenameIn    = "";
    QString     s_Dummy         = "";
    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;
    QStringList sl_Output;

    QList<int>  il_maxNumberOfColumns;
    QList<int>  il_NumOfColumns;

    QFileInfo   fi;

// **********************************************************************************************

    setWaitCursor();

    initFileProgress( sl_FilenameList.count() + 1, sl_FilenameList.at( 0 ), tr( "Concatenating files..." ) );

// **********************************************************************************************
// read files

    if ( buildFilename( i_ActionNumber, s_FilenameFormat, i_Extension, sl_FilenameList.at( i ), s_FilenameIn, s_Dummy ) == true )
    {
        if ( ( n_first = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) > 0 )
        {
            initProgress( sl_FilenameList.count(), s_FilenameIn, tr( "Concatenating files..." ), n_first );

            i_maxNumColumn = 0;

            for ( int j=i_SkipNStartLines; j<n_first; j++ )
            {
                sl_Output.append( sl_Input.at( j ) );

                il_NumOfColumns.append( NumOfSections( sl_Input.at( j ) ) );
                i_maxNumColumn = qMax( i_maxNumColumn, il_NumOfColumns.last() );

                incProgress( sl_FilenameList.count(), j );
            }

            resetProgress( sl_FilenameList.count() );

            il_maxNumberOfColumns.append( i_maxNumColumn );

            stopProgress = incFileProgress( sl_FilenameList.count() + 1, ++i );

            while ( ( i < sl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( i_ActionNumber, s_FilenameFormat, i_Extension, sl_FilenameList.at( i ), s_FilenameIn, s_Dummy ) == true )
                {
                    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) == n_first )
                    {
                        initProgress( sl_FilenameList.count(), s_FilenameIn, tr( "Concatenating files..." ), n );

                        i_maxNumColumn = 0;

                        for ( int j=i_SkipNStartLines; j<n; j++ )
                        {
                            sl_Output.append( sl_Input.at( j ) );

                            il_NumOfColumns.append( NumOfSections( sl_Input.at( j ) ) );
                            i_maxNumColumn = qMax( i_maxNumColumn, il_NumOfColumns.last() );

                            incProgress( sl_FilenameList.count(), j );
                        }

                        resetProgress( sl_FilenameList.count() );

                        il_maxNumberOfColumns.append( i_maxNumColumn );

                    }
                    else
                    {
                       err = -70;
                    }
                }

                stopProgress = incFileProgress( sl_FilenameList.count() + 1, ++i );
            }
        }

// **********************************************************************************************
// write data

        if ( err == _NOERROR_ )
        {
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

            if ( b_IncludeFilename == true )
            {
                if ( buildFilename( i_ActionNumber, s_FilenameFormat, i_Extension, sl_FilenameList.at( 0 ), s_FilenameIn, s_Dummy ) == true )
                {
                    fi.setFile( s_FilenameIn );
                    tout << fi.completeBaseName();

                    for ( int k=1; k<il_maxNumberOfColumns.at( 0 ); k++ )
                        tout << "\t" << fi.completeBaseName();
                }

                for ( int j=1; j<sl_FilenameList.count(); j++ )
                {
                    if ( buildFilename( i_ActionNumber, s_FilenameFormat, i_Extension, sl_FilenameList.at( j ), s_FilenameIn, s_Dummy ) == true )
                    {
                        fi.setFile( s_FilenameIn );
                        tout << "\t" << fi.completeBaseName();

                        for ( int k=1; k<il_maxNumberOfColumns.at( j ); k++ )
                            tout << "\t" << fi.completeBaseName();
                    }
                }

                tout << s_EOL;
            }

            i = 0;
            n = n - i_SkipNStartLines;

            initProgress( sl_FilenameList.count(), s_FilenameOut, tr( "Concatenating files..." ), n );

            while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
            {
                tout << sl_Output.at( i );

                e = il_maxNumberOfColumns.at( 0 ) - il_NumOfColumns.at( i );

                for ( int k=0; k<e; k++ )
                    tout << "\t";

                for ( int j=1; j<sl_FilenameList.count(); j++ )
                {
                    tout << "\t" << sl_Output.at( j*n + i );

                    e = il_maxNumberOfColumns.at( 0 ) - il_NumOfColumns.at( j*n + i );

                    for ( int k=0; k<e; k++ )
                        tout << "\t";
                }

                tout << s_EOL;

                stopProgress = incProgress( sl_FilenameList.count(), ++i );

            }

            resetProgress( sl_FilenameList.count() );

            fout.close();
        }
    }

    resetFileProgress( sl_FilenameList.count() + 1 );

// **********************************************************************************************

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) )
    {
        if ( b_DeleteInputFile == true )
        {
            for ( i=0; i<sl_FilenameList.count(); i++ )
            {
                if ( buildFilename( i_ActionNumber, s_FilenameFormat, i_Extension, sl_FilenameList.at( i ), s_FilenameIn, s_Dummy ) == true )
                    removeFile( s_FilenameIn );
            }
        }
    }

// **********************************************************************************************

    setNormalCursor();

    if ( err != _NOERROR_ )
        return( err );

    return( stopProgress );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-08

void MainWindow::doConcatenateFilesByLines()
{
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( gsl_FilenameList.count() == 1 )
        clearFilenameList( gi_ActionNumber, gsl_FilenameList );

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( gsl_FilenameList.count() > 1 )
        {
            QFileInfo fi( gsl_FilenameList.at( 0 ) );

            s_FilenameOut = fi.absolutePath() + "/" + fi.absolutePath().section( "/", -1, -1 ) + "_all" + setExtension( gi_Extension );

            if ( doConcatenateFilesOptionsDialog( _CONCATENATEBYLINES, gi_cfl_SkipNStartLines, gb_cfl_IncludeFilename, gb_cfl_SkipEmptyLines, gb_cfl_SkipCommentLines, gb_cfl_DeleteInputFile ) == QDialog::Accepted )
                stopProgress = concatenateFilesByLines( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_cfl_SkipNStartLines, gb_cfl_IncludeFilename, gb_cfl_SkipEmptyLines, gb_cfl_SkipCommentLines, gb_cfl_DeleteInputFile );
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

    if ( err == _NOERROR_ )
    {
        clearFilenameList( gi_ActionNumber, gsl_FilenameList );

        QFileInfo fi( s_FilenameOut );
        if ( ( fi.exists() == true ) && ( fi.isFile() == true ) )
            gsl_FilenameList.append( s_FilenameOut );
    }

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Concatenating files was canceled" ), false, false );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-19

void MainWindow::doConcatenateFilesByColumns()
{
    int     err                     = 0;
    int     stopProgress            = 0;

    bool    b_dummy_SkipEmptyLines  = false;

    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( gsl_FilenameList.count() == 1 )
        clearFilenameList( gi_ActionNumber, gsl_FilenameList );

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( gsl_FilenameList.count() > 1 )
        {
            QFileInfo fi( gsl_FilenameList.at( 0 ) );

            s_FilenameOut = fi.absolutePath() + "/" + fi.absolutePath().section( "/", -1, -1 ) + "_all" + setExtension( gi_Extension );

            if ( doConcatenateFilesOptionsDialog( _CONCATENATEBYCOLUMNS, gi_cfc_SkipNStartLines, gb_cfc_IncludeFilename, b_dummy_SkipEmptyLines, gb_cfc_SkipCommentLines, gb_cfc_DeleteInputFile ) == QDialog::Accepted )
                stopProgress = concatenateFilesByColumns( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_cfc_SkipNStartLines, gb_cfc_IncludeFilename, gb_cfc_DeleteInputFile );
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

    if ( err == _NOERROR_ )
    {
        clearFilenameList( gi_ActionNumber, gsl_FilenameList );

        QFileInfo fi( s_FilenameOut );
        if ( ( fi.exists() == true ) && ( fi.isFile() == true ) )
            gsl_FilenameList.append( s_FilenameOut );
    }

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Concatenating files was canceled" ), false, false );

    onError( err );
}
