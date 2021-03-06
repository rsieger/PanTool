/* 2008-07-06                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::recalcColumns( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const QString &s_ColumnsList, const float f_MulX, const float f_AddX, const int i_NumOfDigits, const bool b_OnlyIfEmpty, const int i_NumOfFiles )
{
    int         i               = 0;
    int         n               = 0;
    int         m               = 0;
    int         stopProgress    = 0;

    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;

    QList<int>  il_ColumnList;

    bool        b_OK            = true;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

    if ( ( m = NumOfSections( sl_Input.at( 0 ) ) ) < 1 )
        return( -80 );

    il_ColumnList = scanList( _RECALCCOLUMNS, m, s_ColumnsList );

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Recalculation of columns..." ), sl_Input.count() );

    while ( ( i < n )  && ( stopProgress != _APPBREAK_ ) )
    {
        QString s_Output = "";

        m = NumOfSections( sl_Input.at( i ) );

        for ( int j=0; j < m; j++ )
        {
            if ( isInColumnList( il_ColumnList, j ) == true )
            {
                QString s_Value = "";
                double  d_Value = 0.;

                d_Value = sl_Input.at( i ).section( "\t", j, j ).toDouble( &b_OK );

                if ( ( b_OK == true ) && ( b_OnlyIfEmpty == false ) ) // b_OK == true -> cell contains a number
                    s_Value += QString( "%1" ).arg( d_Value*(double) f_MulX + (double) f_AddX, 0, 'f', i_NumOfDigits );

                if ( ( b_OK == false ) && ( sl_Input.at( i ).section( "\t", j, j ).isEmpty() == true ) && ( b_OnlyIfEmpty == true ) )
                    s_Value += QString( "%1" ).arg( (double) f_AddX, 0, 'f', i_NumOfDigits );

                if ( s_Value.isEmpty() == false )
                    s_Output += "\t" + s_Value;
                else
                    s_Output += "\t" + sl_Input.at( i ).section( "\t", j, j );
            }
            else
            {
                s_Output += "\t" + sl_Input.at( i ).section( "\t", j, j );
            }
        }

        tout << s_Output.section( "\t", 1, m ) << s_EOL;

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

void MainWindow::doRecalcColumns()
{
    int         i               = 0;
    int         err             = 0;
    int         stopProgress    = 0;

    QString     s_FilenameIn    = "";
    QString     s_FilenameOut   = "";

    QStringList sl_FilenameListDelete;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doRecalcDialog( gs_rec_RecalcColumnsList, gf_rec_MulX, gf_rec_AddX, gi_rec_NumOfDigits, gb_rec_OnlyIfEmpty, gb_rec_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Recalculation of columns..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = recalcColumns( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gs_rec_RecalcColumnsList, gf_rec_MulX, gf_rec_AddX, gi_rec_NumOfDigits, gb_rec_OnlyIfEmpty, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_rec_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Recalculation of columns was canceled" ) );

    onError( err );
}
