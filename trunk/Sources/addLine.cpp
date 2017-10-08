/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include <QtGui>

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::addLine( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL,
                         const QStringList &sl_Text, const int i_LineNo,  const bool b_AddFilename, const bool b_AddFullPath, const bool b_AddOrdinalNumber,
                         const bool b_PrependMetadataColumn, const bool b_AppendMetadataColumn, const bool b_SkipEmptyLines, const bool b_SkipCommentLines, const int i_NumOfFiles )
{
    int         i               = 0;
    int         k               = 0;
    int         n               = 0;
    int         e               = 0;
    int         stopProgress    = 0;

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

    QFileInfo fi( s_FilenameIn );

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Adding lines..." ), sl_Input.count() );

    e = i_LineNo - 1;

    if ( b_PrependMetadataColumn == true )
    {
        if ( b_AddFilename == true )
            tout << "Event label" << "\t";

        if ( b_AddFullPath == true )
            tout << "Filename" << "\t";

        if ( b_AddOrdinalNumber == true )
            tout << "No" << "\t";
    }

    tout << sl_Input.at( 0 );

    if ( b_AppendMetadataColumn == true )
    {
        if ( b_AddFilename == true )
            tout << "\t" << "Event label";

        if ( b_AddFullPath == true )
            tout << "\t"<< "Filename";

        if ( b_AddOrdinalNumber == true )
            tout << "\t" << "No";
    }

    tout << s_EOL;

    stopProgress = incProgress( i_NumOfFiles, ++i );

    while ( ( i<e ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
        {
            if ( b_PrependMetadataColumn == true )
            {
                if ( b_AddFilename == true )
                    tout << fi.baseName() << "\t";

                if ( b_AddFullPath == true )
                    tout << fi.absoluteFilePath() << "\t";

                if ( b_AddOrdinalNumber == true )
                    tout << QString( "%1" ).arg( ++k ) << "\t";
            }

            tout << sl_Input.at( i );

            if ( b_AppendMetadataColumn == true )
            {
                if ( b_AddFilename == true )
                    tout << "\t" << fi.baseName();

                if ( b_AddFullPath == true )
                    tout << "\t" << fi.absoluteFilePath();

                if ( b_AddOrdinalNumber == true )
                {
                    if ( b_PrependMetadataColumn == false )
                        tout << "\t" << QString( "%1" ).arg( ++k );
                    else
                        tout << "\t" << QString( "%1" ).arg( k );
                }
            }

            tout << s_EOL;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    for ( int j=0; j<sl_Text.count(); j++ )
    {
        if ( b_PrependMetadataColumn == true )
        {
            if ( b_AddFilename == true )
                tout << fi.baseName() << "\t";

            if ( b_AddFullPath == true )
                tout << fi.absoluteFilePath() << "\t";

            if ( b_AddOrdinalNumber == true )
                tout << QString( "%1" ).arg( ++k ) << "\t";
        }

        tout << sl_Text.at( j );

        if ( b_AppendMetadataColumn == true )
        {
            if ( b_AddFilename == true )
                tout << "\t" << fi.baseName();

            if ( b_AddFullPath == true )
                tout << "\t" << fi.absoluteFilePath();

            if ( b_AddOrdinalNumber == true )
            {
                if ( b_PrependMetadataColumn == false )
                    tout << "\t" << QString( "%1" ).arg( ++k );
                else
                    tout << "\t" << QString( "%1" ).arg( k );
            }
        }

        tout << s_EOL;
    }

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( LineCanBeWritten( sl_Input.at( i ), b_SkipEmptyLines, b_SkipCommentLines ) == true )
        {
            if ( b_PrependMetadataColumn == true )
            {
                if ( b_AddFilename == true )
                    tout << fi.baseName() << "\t";

                if ( b_AddFullPath == true )
                    tout << fi.absoluteFilePath() << "\t";

                if ( b_AddOrdinalNumber == true )
                    tout << QString( "%1" ).arg( ++k ) << "\t";
            }

            tout << sl_Input.at( i );

            if ( b_AppendMetadataColumn == true )
            {
                if ( b_AddFilename == true )
                    tout << "\t" << fi.baseName();

                if ( b_AddFullPath == true )
                    tout << "\t" << fi.absoluteFilePath();

                if ( b_AddOrdinalNumber == true )
                {
                    if ( b_PrependMetadataColumn == false )
                        tout << "\t" << QString( "%1" ).arg( ++k );
                    else
                        tout << "\t" << QString( "%1" ).arg( k );
                }
            }

            tout << s_EOL;
        }

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

void MainWindow::doAddLine()
{
    int         i                   = 0;
    int         err                 = 0;
    int         stopProgress        = 0;

    QString     s_Text              = "";
    QString     s_dummy             = "";

    QString     s_FilenameIn        = "";
    QString     s_FilenameOut       = "";

    QStringList sl_Text;
    QStringList sl_FilenameListDelete;

    bool        b_dummy             = false;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doAddDialog( _ADDLINE, gs_al_Text, s_dummy, gi_al_LineNo, b_dummy, b_dummy, gb_al_AddFilename, gb_al_AddFullPath, gb_al_AddOrdinalNumber, gb_al_PrependMetadataColumn, gb_al_AppendMetadataColumn, gb_al_SkipEmptyLines, gb_al_SkipCommentLines, gb_al_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Adding lines..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    s_Text = gs_al_Text;
                    s_Text.replace( "^t", "\t" );
                    sl_Text = s_Text.split( "^n" );

                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = addLine( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, sl_Text, gi_al_LineNo, gb_al_AddFilename, gb_al_AddFullPath, gb_al_AddOrdinalNumber, gb_al_PrependMetadataColumn, gb_al_AppendMetadataColumn, gb_al_SkipEmptyLines, gb_al_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_al_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Adding lines was canceled" ) );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doAddBlock()
{
    int         i                   = 0;
    int         err                 = 0;
    int         stopProgress        = 0;

    QString     s_Text              = "";
    QString     s_dummy             = "";

    QString     s_FilenameIn        = "";
    QString     s_FilenameOut       = "";

    QStringList sl_Text;
    QStringList sl_FilenameListDelete;

    bool        b_dummy             = false;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doAddDialog( _ADDBLOCK, gs_ab_Text, s_dummy, gi_ab_LineNo, b_dummy, b_dummy, gb_ab_AddFilename, gb_ab_AddFullPath, gb_ab_AddOrdinalNumber, gb_ab_PrependMetadataColumn, gb_ab_AppendMetadataColumn, gb_ab_SkipEmptyLines, gb_ab_SkipCommentLines, gb_ab_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Adding text block..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    s_Text = gs_ab_Text;
                    s_Text.replace( "^t", "\t" );
                    sl_Text = s_Text.split( "^n" );

                    sl_FilenameListDelete.append( s_FilenameIn );

                    err = addLine( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, sl_Text, gi_ab_LineNo, gb_ab_AddFilename, gb_ab_AddFullPath, gb_ab_AddOrdinalNumber, gb_ab_PrependMetadataColumn, gb_ab_AppendMetadataColumn, gb_ab_SkipEmptyLines, gb_ab_SkipCommentLines, gsl_FilenameList.count() );

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

    if ( ( stopProgress != _APPBREAK_ ) && ( err == _NOERROR_ ) && ( gb_ab_DeleteInputFile == true ) )
    {
        for ( int i=0; i<sl_FilenameListDelete.count(); i++ )
            QFile::remove( sl_FilenameListDelete.at( i ) );
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Adding text block was canceled" ) );

    onError( err );
}


