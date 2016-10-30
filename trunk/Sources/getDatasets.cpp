/* getDatasets.cpp			  */
/* 2016-04-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-10-04

void MainWindow::getDatasets( const QString &s_Query, const QString &s_IDListFile, const QString &s_DownloadDirectory, const bool b_DownloadData, const bool b_DownloadCitation, const bool b_DownloadMetadata, const int i_CodecDownload, const int i_EOL, const int i_Extension )
{
    int err                     = _NOERROR_;

    int i       				= 0;
    int i_NumOfQueries          = 0;
    int i_NumOfDatasetIDs       = 0;

    int	i_stopProgress			= 0;
    int	i_NumOfParents			= 0;
    int i_totalNumOfDownloads	= 0;
    int i_removedDatasets		= 0;

    QString s_EOL               = setEOLChar( i_EOL );

    QString s_Message			= "";
    QString s_DatasetID			= "";
    QString	s_Data				= "";
    QString s_Domain            = "";
    QString s_Filename          = "";
    QString s_Url				= "";
    QString s_Curl              = "";
    QString s_tempFile          = "";

    QString s_PathDir           = "";
    QString s_PathFile          = "";

    QStringList	sl_Queries;
    QStringList sl_DatasetIDs;
    QStringList sl_Data;

    bool	b_ExportFileExists	= false;
    bool	b_isURL             = false;

// **********************************************************************************************
// read ID list

    s_Curl = findCurl();

    if ( s_Query.isEmpty() == false )
    {
        s_tempFile = s_DownloadDirectory + "/" + "Query_result_json.txt";
        s_Url      = "https://www.pangaea.de/advanced/search.php?" + s_Query.section( "/?", 1, 1 );

        if ( s_Url.contains( "&count=" ) == false )
            s_Url.append( "&count=500" );

        downloadFile( s_Curl, s_Url, s_tempFile );

        i_NumOfQueries = readFile( s_tempFile, sl_Queries, _SYSTEM_ ); // System encoding

        removeFile( s_tempFile );
    }

    if ( s_IDListFile.isEmpty() == false )
        i_NumOfDatasetIDs = readFile( s_IDListFile, sl_DatasetIDs, _SYSTEM_ ); // System encoding

    if ( i_NumOfQueries + i_NumOfDatasetIDs < 1 )
        return;

// **********************************************************************************************
// create log file

    QFileInfo fidd( s_DownloadDirectory );
    QFileInfo fifailed( s_IDListFile );

    s_PathDir = fidd.absoluteFilePath();

    if ( s_IDListFile.isEmpty() == false )
        s_PathFile = fifailed.absolutePath();

    QFile fout;

    if ( s_PathDir != s_PathFile )
        fout.setFileName( fidd.absoluteFilePath().section( "/", 0, fidd.absoluteFilePath().count( "/" )-1 ) + "/" + fidd.absoluteFilePath().section( "/", -1, -1 ) + "_failed" + setExtension( i_Extension ) );
    else
        fout.setFileName( fifailed.absolutePath() + "/" + fifailed.completeBaseName() + "_failed" + setExtension( i_Extension ) );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text ) == false )
        return;

    QTextStream tout( &fout );

    switch ( i_CodecDownload )
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
// Read data and build dataset list

    if ( sl_Queries.count() > 0 )
    {
        if ( ( sl_Queries.at( 0 ).startsWith( "{" ) == true ) && ( sl_Queries.at( 0 ).endsWith( "}" ) == true )  )
        {
            QStringList sl_URIs;

            QJsonDocument jsonResponse = QJsonDocument::fromJson( sl_Queries.join( "" ).toUtf8());
            QJsonObject   jsonObject   = jsonResponse.object();
            QJsonArray    jsonArray    = jsonObject[ "results" ].toArray();

            foreach ( const QJsonValue & value, jsonArray )
            {
                QJsonObject obj = value.toObject();
                sl_URIs.append( obj[ "URI" ].toString() );
            }

            for ( int i=0; i<sl_URIs.count(); i++ )
                sl_Data.append( sl_URIs.at( i ).section( "PANGAEA.", 1, 1 ) );
        }
    }

    if ( sl_DatasetIDs.count() > 0 )
    {
        sl_DatasetIDs.removeDuplicates();

        if ( sl_DatasetIDs.at( 0 ).section( "\t", 0, 0 ).toLower().startsWith( "url" ) == true )
            b_isURL = true;

        if ( sl_DatasetIDs.at( 0 ).section( "\t", 0, 0 ).toLower().startsWith( "uniform resource locator" ) == true )
            b_isURL = true;

        if ( sl_DatasetIDs.at( 0 ).section( "\t", 1, 1 ).toLower().startsWith( "filename" ) == true )
            b_ExportFileExists = true;

        if ( sl_DatasetIDs.at( 0 ).section( "\t", 1, 1 ).toLower().startsWith( "file name" ) == true )
            b_ExportFileExists = true;

        while ( ++i < sl_DatasetIDs.count() )
        {
            s_Data = sl_DatasetIDs.at( i );
            s_Data.replace( " ", "" );

            if ( s_Data.isEmpty() == false )
                sl_Data.append( s_Data );
        }
    }

// **********************************************************************************************

    if ( b_isURL == true )
        tout << "URL\tfile name\tComment" << endl;
    else
        tout << "Comment" << endl;

// **********************************************************************************************

    i_totalNumOfDownloads = sl_Data.count();

    if ( i_totalNumOfDownloads <= 0 )
    {
        s_Message = tr( "No datasets downloaded. See\n\n" ) + QDir::toNativeSeparators( fout.fileName() ) + tr( "\n\nfor details." );
        QMessageBox::information( this, getApplicationName( true ), s_Message );
        return;
    }

// **********************************************************************************************
// Download

    initFileProgress( i_totalNumOfDownloads, "", tr( "Downloading data sets..." ) );

    i = 0;

    while ( ( i < i_totalNumOfDownloads ) && ( err == _NOERROR_ ) && ( i_stopProgress != _APPBREAK_ ) )
    {
        if ( b_isURL == true )
        {
            s_Domain = sl_Data.at( i ).section( "\t", 0, 0 ).section( "/", 0, 2 ); // eg. http://iodp.tamu.edu/

            if ( sl_Data.at( i ).section( "\t", 0, 0 ).section( "/", 3 ).isEmpty() == false )
                s_Url = s_Domain + "/" + sl_Data.at( i ).section( "\t", 0, 0 ).section( "/", 3 ); // eg. /janusweb/chemistry/chemcarb.cgi?leg=197&site=1203&hole=A
        }
        else
        {
            s_Domain    = "https://doi.pangaea.de"; // PANGAEA datasets

            s_DatasetID = sl_Data.at( i ).section( "\t", 0, 0 ).toLower();

            s_DatasetID.replace( tr( "http://doi.pangaea.de/10.1594/pangaea." ), tr( "" ) );
            s_DatasetID.replace( tr( "https://doi.pangaea.de/10.1594/pangaea." ), tr( "" ) );
            s_DatasetID.replace( tr( "doi:10.1594/pangaea." ), tr( "" ) );
            s_DatasetID.replace( tr( "dataset id: " ), tr( "" ) );
            s_DatasetID.replace( tr( ", unpublished dataset" ), tr( "" ) );
            s_DatasetID.replace( tr( ", doi registration in progress" ), tr( "" ) );
        }

        if ( b_ExportFileExists == true )
        {
            s_Filename = sl_Data.at( i ).section( "\t", 1, 1 );
            s_Filename.replace( " ", "_" );

            if ( s_Filename.isEmpty() == true )
            {
                if ( b_isURL == true )
                    s_Filename = sl_Data.at( i ).section( "\t", 0, 0 ).section( "/", -1, -1 );
                else
                    s_Filename = tr( "not_given" );
            }

            if ( b_isURL == false )
            {
                s_Filename.append( tr( "~" ) );
                s_Filename.append( s_DatasetID );
            }
        }
        else
        {
            if ( b_isURL == true )
                s_Filename = sl_Data.at( i ).section( "\t", 0, 0 ).section( "/", -1, -1 );
            else
                s_Filename.sprintf( "%06d", s_DatasetID.toInt() );
        }

        if ( ( b_DownloadCitation == true ) && ( b_isURL == false ) )
        {
            s_Url = s_Domain + "/10.1594/PANGAEA."+ s_DatasetID + "?format=citation_text";

            if ( sl_Data.at( i ).section( "\t", 1, 1 ) == "parent" )
                downloadFile( s_Curl, s_Url, s_DownloadDirectory + "/" + "is_parent_" + s_Filename + "_citation" + setExtension( i_Extension ) );
            else
                downloadFile( s_Curl, s_Url, s_DownloadDirectory + "/" + s_Filename + "_citation" + setExtension( i_Extension ) );

            wait( 100 );
        }

        if ( ( b_DownloadMetadata == true ) && ( b_isURL == false ) )
        {
            s_Url = s_Domain + "/10.1594/PANGAEA."+ s_DatasetID + "?format=metainfo_xml";

            if ( sl_Data.at( i ).section( "\t", 1, 1 ) == "parent" )
                downloadFile( s_Curl, s_Url, s_DownloadDirectory + "/" + "is_parent_" + s_Filename + "_metadata.xml" );
            else
                downloadFile( s_Curl, s_Url, s_DownloadDirectory + "/" + s_Filename + "_metadata.xml" );

            wait( 100 );
        }

        if ( b_DownloadData == true )
        {
            if ( ( b_isURL == true ) || ( s_DatasetID.toInt() >= 50000 ) )
            {
                s_Filename = s_DownloadDirectory + "/" + s_Filename;

                if ( b_isURL == false )
                {
                    // download PANGAEA data sets

                    s_Filename.append( setExtension( i_Extension ) );
                    s_Url = s_Domain + "/10.1594/PANGAEA." + s_DatasetID + "?format=textfile";

                    switch ( i_CodecDownload )
                    {
                    case _LATIN1_:
                        s_Url.append( "&charset=ISO-8859-1" );
                        break;

                    case _APPLEROMAN_:
                        s_Url.append( "&charset=x-MacRoman" );
                        break;

                    default:
                        s_Url.append( "&charset=UTF-8" );
                        break;
                    }

                    downloadFile( s_Curl, s_Url, s_Filename );

                    switch( checkFile( s_Filename, false ) )
                    {
                    case -10: // File size = 0 Byte
                        tout << "Data set " << s_DatasetID << " is login required or static URL" << s_EOL;
                        removeFile( s_Filename );
                        i_removedDatasets++;
                        break;
                    case -20:
                        tout << "Data set " <<  s_DatasetID << " was substituted by an other version." << s_EOL;
                        removeFile( s_Filename );
                        i_removedDatasets++;
                        break;
                    case -30:
                        tout << "Data set " <<  s_DatasetID << " data set is a parent." << s_EOL;
                        removeFile( s_Filename );
                        i_removedDatasets++;
                        i_NumOfParents++;
                        break;
                    case -40:
                        tout << "Something wrong, no data available for dataset " << s_DatasetID << ". Please ask Rainer Sieger (rsieger@pangaea.de)" << s_EOL;
                        removeFile( s_Filename );
                        i_removedDatasets++;
                        break;
                    case -50:
                        tout << "Data set " <<  s_DatasetID << " not exist!" << s_EOL;
                        removeFile( s_Filename );
                        i_removedDatasets++;
                        break;
                    case -60:
                        tout << "Data set " << s_DatasetID << " not available at this time. Please try again later." << s_EOL;
                        removeFile( s_Filename );
                        i_removedDatasets++;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    // download binary data with curl

                    downloadFile( s_Curl, s_Url, s_Filename );

                    switch( checkFile( s_Filename, true ) )
                    {
                    case -10: // File size = 0 Byte
                        tout << s_Url << "\t" << QDir::toNativeSeparators( s_Filename ) << "\t" << "login required or file not found" << s_EOL;
                        removeFile( s_Filename );
                        i_removedDatasets++;
                        break;
                    default:
                        break;
                    }
                }

                wait( 100 );
            }
            else
            {
                err = _ERROR_;
            }
        }

        i_stopProgress = incFileProgress( i_totalNumOfDownloads, i++ );
    }

// **********************************************************************************************

    resetFileProgress( i_totalNumOfDownloads );

    fout.close();

// **********************************************************************************************

    if ( b_isURL == false )
    {
        if ( i-i_removedDatasets == 0 )
        {
            s_Message = tr( "No datasets downloaded. See\n\n" ) + QDir::toNativeSeparators( fout.fileName() ) + tr( "\n\nfor details." );
            QMessageBox::information( this, getApplicationName( true ), s_Message );
        }
        else
        {
            if ( i_removedDatasets > 0 )
            {
                s_Message = QString( "%1" ).arg( i-i_removedDatasets ) + tr( " datasets downloaded to\n" ) + QDir::toNativeSeparators( s_DownloadDirectory ) + "\n\n" + QString( "%1" ).arg( i_removedDatasets ) + tr( " datasets removed after download. See\n" ) + QDir::toNativeSeparators( fout.fileName() ) + tr( "\nfor details." );
                QMessageBox::information( this, getApplicationName( true ), s_Message );
            }
            else
            {
                if ( i_NumOfParents > 0 )
                {
                    s_Message = QString( "%1" ).arg( i ) + tr( " datasets downloaded to\n" ) + QDir::toNativeSeparators( s_DownloadDirectory ) + "\n\n" + QString( "%1" ).arg( i_NumOfParents ) + tr( " parents removed from download list. See\n" ) + QDir::toNativeSeparators( fout.fileName() ) + tr( "\nfor details." );;
                    QMessageBox::information( this, getApplicationName( true ), s_Message );
                }
                else
                {
                    s_Message = QString( "%1" ).arg( i ) + tr( " datasets downloaded to\n" ) + QDir::toNativeSeparators( s_DownloadDirectory );
                    QMessageBox::information( this, getApplicationName( true ), s_Message );
                }
            }

            if ( ( i_removedDatasets == 0 ) && ( i_NumOfParents == 0 ) )
                fout.remove();
        }
    }
    else
    {
        s_Message = QString( "%1" ).arg( i ) + tr( " files downloaded to\n" ) + QDir::toNativeSeparators( s_DownloadDirectory );
        QMessageBox::information( this, getApplicationName( true ), s_Message );

        if ( i_removedDatasets == 0 )
            fout.remove();
    }

// **********************************************************************************************

    return;
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::checkFile( const QString &s_Filename, const bool isbinary )
{
    QStringList sl_Input;
    QStringList sl_Result;

    QFileInfo   fd( s_Filename );

// **********************************************************************************************

    if ( fd.size() == 0 )
        return( -10 );

    if ( isbinary == true )
        return( 0 );

// **********************************************************************************************

    if ( readFile( s_Filename, sl_Input, _SYSTEM_, 8000 ) > 0 )
    {
        if ( sl_Input.at( 0 ).startsWith( "/* DATA DESCRIPTION:" ) == false  )
        {
            sl_Result = sl_Input.filter( "was substituted by an other version at" );

            if ( sl_Result.count() > 0 )
                return( -20 );

            sl_Result = sl_Input.filter( "TEXTFILE format is not available for collection data sets!" );

            if ( sl_Result.count() > 0 )
                return( -30 );

            sl_Result = sl_Input.filter( "No data available!" );

            if ( sl_Result.count() > 0 )
                return( -40 );

            sl_Result = sl_Input.filter( "A data set identified by" );

            if ( sl_Result.count() > 0 )
                return( -50 );

            sl_Result = sl_Input.filter( "The dataset is currently not available for download. Try again later!" );

            if ( sl_Result.count() > 0 )
                return( -60 );
        }
    }

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doGetDatasets()
{
    if ( doGetDatasetsDialog( gs_Query, gs_IDListFile, gs_DownloadDirectory, gb_DownloadData, gb_DownloadCitation, gb_DownloadMetadata, gi_CodecDownload, gi_Extension ) == QDialog::Accepted )
    {
        getDatasets( gs_Query, gs_IDListFile, gs_DownloadDirectory, gb_DownloadData, gb_DownloadCitation, gb_DownloadMetadata, gi_CodecDownload, gi_EOL, gi_Extension );

        if ( gs_DownloadDirectory.isEmpty() == false )
            chooseFolder( gs_DownloadDirectory );
    }

// **********************************************************************************************

    onError( _NOERROR_ );
}
