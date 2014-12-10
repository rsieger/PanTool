/* 2012-10-01                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

/*      hdl:10013/epic.27914.d001
 *
 *      Leg             leg number
 *      Site            site number
 *  	Hole            hole letter
 *      Core            core number
 *      Type            core-type letter
 *      Section         section number
 *      Top (cm)        interval from top to
 *      Bottom (cm) 	bottom
 *
 *      => Example: 113-693A-4R-3,23-25
 */

int MainWindow::createODPSampleLabel( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL,
                                      const int i_LegColumn, const int i_SiteColumn, const int i_HoleColumn, const int i_CoreColumn, const int i_CoreTypeColumn,
                                      const int i_SectionColumn, const int i_TopColumn, const int i_BottomColumn, const bool b_DeleteInputFile, const int i_NumOfFiles )
{
    int         i                 = 0;
    int         n                 = 0;

    int         stopProgress      = 0;

    QString     s_EOL             = setEOLChar( i_EOL );
    QString     s_Event           = "";
    QString     s_ODPSampleLabel  = "";
    QString     s_Top             = "";
    QString     s_Bottom          = "";

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Creating ODP sample label..." ), n );

    tout << "Event label\tLabel []@ODP sample designation\t" << sl_Input.at( i++ ) << s_EOL;

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( i_LegColumn < 1 )
            s_Event = "???";
        else
            s_Event = sl_Input.at( i ).section( "\t", i_LegColumn-1, i_LegColumn-1 );

        if ( i_SiteColumn < 1 )
            s_Event += "-???";
        else
            s_Event += "-" + sl_Input.at( i ).section( "\t", i_SiteColumn-1, i_SiteColumn-1 );

        if ( i_HoleColumn > 0 )
            s_Event += sl_Input.at( i ).section( "\t", i_HoleColumn-1, i_HoleColumn-1 );

        s_ODPSampleLabel = s_Event;

        if ( i_CoreColumn > 0 )
            s_ODPSampleLabel += "-" + sl_Input.at( i ).section( "\t", i_CoreColumn-1, i_CoreColumn-1 );

        if ( i_CoreTypeColumn > 0 )
            s_ODPSampleLabel += sl_Input.at( i ).section( "\t", i_CoreTypeColumn-1, i_CoreTypeColumn-1 );

        if ( ( i_CoreColumn > 0 ) && ( i_SectionColumn > 0 ) )
            s_ODPSampleLabel += "-" + sl_Input.at( i ).section( "\t", i_SectionColumn-1, i_SectionColumn-1 );

        if ( ( i_SectionColumn > 0 ) && ( i_TopColumn > 0 ) )
        {
            s_Top = sl_Input.at( i ).section( "\t", i_TopColumn-1, i_TopColumn-1 );
            s_Top.replace( ".00", "" );
            s_Top.replace( ".0", "" );
            s_ODPSampleLabel += "," + s_Top;
        }

        if ( ( i_SectionColumn > 0 ) && ( i_TopColumn > 0 ) && ( i_BottomColumn > 0 ) )
        {
            s_Bottom = sl_Input.at( i ).section( "\t", i_BottomColumn-1, i_BottomColumn-1 );
            s_Bottom.replace( ".00", "" );
            s_Bottom.replace( ".0", "" );
            s_ODPSampleLabel += "-" + s_Bottom;
        }

        if ( LineCanBeWritten( sl_Input.at( i ), true, true ) == true )
            tout << s_Event.replace( " ", "" ) << "\t" << s_ODPSampleLabel.replace( " ", "" ) << "\t" << sl_Input.at( i ) << s_EOL;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    if ( b_DeleteInputFile == true )
        removeFile( s_FilenameIn );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-27

void MainWindow::doCreateODPSampleLabel()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doODPSampleLabelDialog( gi_odp_LegColumn, gi_odp_SiteColumn, gi_odp_HoleColumn, gi_odp_CoreColumn, gi_odp_CoreTypeColumn, gi_odp_SectionColumn, gi_odp_TopColumn, gi_odp_BottomColumn, gb_odp_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Creating ODP sample label..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    err = createODPSampleLabel( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_odp_LegColumn, gi_odp_SiteColumn, gi_odp_HoleColumn, gi_odp_CoreColumn, gi_odp_CoreTypeColumn, gi_odp_SectionColumn, gi_odp_TopColumn, gi_odp_BottomColumn, gb_odp_DeleteInputFile, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Creating ODP sample label was canceled" ) );

    onError( err );
}

