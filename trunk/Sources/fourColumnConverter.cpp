/* 2012-02-02                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-02-02

int MainWindow::convertColumns2TableFormat( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_FieldDelimiter, const QString &s_MissingValue, const int i_NumOfFiles )
{
    int         i                   = 1;
    int         n                   = 0;

    int         i_NumOfGeocodes     = 0;

    int         stopProgress        = 0;

    QString     s_EventLabel        = "";
    QString     s_Geocode           = "";
    QString     s_newGeocode        = "";
    QString     s_FieldDelimiter    = "";
    QString     s_EOL               = setEOLChar( i_EOL );

    QString     InputStr            = "";

    QStringList sl_Input;
    QStringList sl_Output;
    QStringList sl_Header;
    QStringList sl_Parameter;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

    sl_Input.append( "End of data" );

    i_NumOfGeocodes = NumOfSections( sl_Input.at( 0 ) ) - 3;

    switch ( i_FieldDelimiter )
    {
        case _COMMA_:
            s_FieldDelimiter = ",";
            break;
        case _SEMICOLON_:
            s_FieldDelimiter = ";";
            break;
        case _SPACE_:
            s_FieldDelimiter = " ";
            break;
        default:
            s_FieldDelimiter = "\t";
            break;
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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting columns to table format..." ), sl_Input.count() );

    switch ( i_NumOfGeocodes )
    {
    case 2:
        sl_Header.append( "Event label" );
        sl_Header.append( sl_Input.at( 0 ).section( "\t", 1, 1 ) );
        sl_Header.append( sl_Input.at( 0 ).section( "\t", 2, 2 ) );
        break;

    default:
        sl_Header.append( "Event label" );
        sl_Header.append( sl_Input.at( 0 ).section( "\t", 1, 1 ) );
        break;
    }

    for ( int j=1; j<n; j++ )
    {
        if ( sl_Parameter.contains( tr( "@" ) + sl_Input.at( j ).section( "\t", i_NumOfGeocodes+1, i_NumOfGeocodes+1 ) + tr( "@" ), Qt::CaseInsensitive ) == false )
        {
            sl_Header.append( sl_Input.at( j ).section( "\t", i_NumOfGeocodes+1, i_NumOfGeocodes+1 ) );
            sl_Parameter.append( tr( "@" ) + sl_Input.at( j ).section( "\t", i_NumOfGeocodes+1, i_NumOfGeocodes+1 ) + tr( "@" ) );
        }
    }

    tout << sl_Header.join( s_FieldDelimiter ) << s_EOL;

    n++;

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        InputStr = sl_Input.at( i );
        InputStr.replace( "\"", "" );

        switch ( i_NumOfGeocodes )
        {
        case 2:
            s_newGeocode = InputStr.section( "\t", 1, 1 ) + "@" + InputStr.section( "\t", 2, 2 );
            break;

        default:
            s_newGeocode = InputStr.section( "\t", 1, 1 );
            break;
        }

        if ( ( s_EventLabel != InputStr.section( "\t", 0, 0 ) ) || ( s_Geocode != s_newGeocode ) )
        {
            if ( sl_Output.count() > 0 )
            {
                for ( int j=0; j<sl_Parameter.count(); j++ )
                    sl_Output.replaceInStrings( sl_Parameter.at( j ), s_MissingValue );

                tout << sl_Output.join( s_FieldDelimiter ) << s_EOL;
            }

            sl_Output.clear();
            sl_Output.append( InputStr.section( "\t", 0, 0 ) );

            s_EventLabel = InputStr.section( "\t", 0, 0 );

            switch ( i_NumOfGeocodes )
            {
            case 2:
                s_Geocode = InputStr.section( "\t", 1, 1 ) + "@" + InputStr.section( "\t", 2, 2 );
                sl_Output.append( InputStr.section( "\t", 1, 1 ) );
                sl_Output.append( InputStr.section( "\t", 2, 2 ) );
                break;

            default:
                s_Geocode = InputStr.section( "\t", 1, 1 );
                sl_Output.append( InputStr.section( "\t", 1, 1 ) );
                break;
            }

            for ( int j=0; j<sl_Parameter.count(); j++ )
                sl_Output.append( sl_Parameter.at( j ) );
        }

        sl_Output.replaceInStrings( tr( "@" ) + InputStr.section( "\t", i_NumOfGeocodes+1, i_NumOfGeocodes+1 ) + tr( "@" ), InputStr.section( "\t", i_NumOfGeocodes+2, i_NumOfGeocodes+2 ) );

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
// 2012-02-02

int MainWindow::convertTable2ColumnsFormat( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfGeocodes, const int i_NumOfFiles )
{
    int         i               = 1;
    int         n               = 0;
    int         m               = 0;

    int         stopProgress    = 0;

    QString     HeaderStr       = "";
    QString     InputStr        = "";
    QString     s_EventLabel    = "";
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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting table to columns format..." ), sl_Input.count() );

    HeaderStr = sl_Input.at( 0 );
    HeaderStr.replace( "\"", "" );

    m = NumOfSections( HeaderStr );

    switch ( i_NumOfGeocodes )
    {
    case 2:
        tout << HeaderStr.section( "\t", 0, 0 ) << "\t" << HeaderStr.section( "\t", 1, 1 ) << "\t" << HeaderStr.section( "\t", 2, 2 ) << "\tParameter\tValue" << s_EOL;

        while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
        {
            InputStr = sl_Input.at( i );
            InputStr.replace( "\"", "" );

            if ( InputStr.section( "\t", 0, 0 ).isEmpty() == false )
                s_EventLabel = InputStr.section( "\t", 0, 0 );

            for ( int j=3; j<m; j++ )
            {
                if ( ( InputStr.section( "\t", 1, 1 ).isEmpty() == false ) && ( InputStr.section( "\t", 2, 2 ).isEmpty() == false ) && ( InputStr.section( "\t", j, j ).isEmpty() == false ) )
                {
                    tout << s_EventLabel << "\t";
                    tout << InputStr.section( "\t", 1, 1 ) << "\t" <<  InputStr.section( "\t", 2, 2 ) << "\t";
                    tout << HeaderStr.section( "\t", j, j ) << "\t" << InputStr.section( "\t", j, j ) << s_EOL;
                }
            }

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }

        break;

    default:
        tout << HeaderStr.section( "\t", 0, 0 ) << "\t" << HeaderStr.section( "\t", 1, 1 ) << "\t" << "Parameter\tValue" << s_EOL;

        while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
        {
            InputStr = sl_Input.at( i );
            InputStr.replace( "\"", "" );

            if ( InputStr.section( "\t", 0, 0 ).isEmpty() == false )
                s_EventLabel = InputStr.section( "\t", 0, 0 );

            for ( int j=2; j<m; j++ )
            {
                if ( ( InputStr.section( "\t", 1, 1 ).isEmpty() == false ) && ( InputStr.section( "\t", j, j ).isEmpty() == false ) )
                {
                    tout << s_EventLabel << "\t" << InputStr.section( "\t", 1, 1 ) << "\t";
                    tout << HeaderStr.section( "\t", j, j ) << "\t" << InputStr.section( "\t", j, j ) << s_EOL;
                }
            }

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }

        break;
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
// 2012-02-02

void MainWindow::doColumns2TableFormat()
{
    int     i                       = 0;
    int     err                     = 0;
    int     stopProgress            = 0;

    int     i_dummy_NumOfGeocodes   = 0;

    QString s_FilenameIn            = "";
    QString s_FilenameOut           = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doFourColumnsDialog( _COLUMNSTABLE, i_dummy_NumOfGeocodes, gi_cft_FieldDelimiter, gs_cft_MissingValue, gb_cft_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting columns to table format..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = convertColumns2TableFormat( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_cft_FieldDelimiter, gs_cft_MissingValue, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_cft_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting four columns format was canceled" ) );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-02-02

void MainWindow::doTable2ColumnsFormat()
{
    int     i                       = 0;
    int     err                     = 0;
    int     stopProgress            = 0;

    int     i_dummy_FieldDelimiter  = 0;
    QString s_dummy_MissingValue    = "";

    QString s_FilenameIn            = "";
    QString s_FilenameOut           = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doFourColumnsDialog( _TABLECOLUMNS, gi_tcf_NumOfGeocodes, i_dummy_FieldDelimiter, s_dummy_MissingValue, gb_tcf_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Building four columns format..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = convertTable2ColumnsFormat( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_tcf_NumOfGeocodes, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_tcf_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Building four columns format was canceled" ) );

    onError( err );
}

