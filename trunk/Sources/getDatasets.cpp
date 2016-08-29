/* getDatasets.cpp			  */
/* 2016-04-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::getDatasets( const QString &s_IDListFile, const QString &s_DownloadDirectory, const bool b_DownloadData, const bool b_DownloadCitation, const bool b_DownloadMetadata, const int i_CodecDownload, const int i_EOL, const int i_Extension )
{
    int i       				= 0;
    int n                       = 0;
    int err                     = _NOERROR_;

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
    QString s_Size				= "";
    QString s_Curl              = "";

    QStringList	sl_Input;
    QStringList sl_Data;
    QStringList sl_Result;

    bool	b_ExportFileExists	= false;
    bool	b_isURL             = false;

// **********************************************************************************************
// read ID list

    if ( ( n = readFile( s_IDListFile, sl_Input, _SYSTEM_ ) ) < 1 ) // System encoding
        return;

// **********************************************************************************************

    QFileInfo fi( s_DownloadDirectory );

    QFile fout( fi.absoluteFilePath().section( "/", 0, fi.absoluteFilePath().count( "/" )-1 ) + "/" + fi.absoluteFilePath().section( "/", -1, -1 ) + "_failed" + setExtension( i_Extension ) );

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

    s_Curl = findCurl();

// **********************************************************************************************

    if ( b_isURL == true )
        tout << "URL\tfile name\tComment" << endl;
    else
        tout << "*ID\tComment" << endl;

// **********************************************************************************************
// Read data and build dataset list

    if ( ( sl_Input.at( 0 ).startsWith( "<html>", Qt::CaseInsensitive ) == true ) || ( sl_Input.at( 0 ).startsWith( "<!doctype html", Qt::CaseInsensitive ) == true ) || ( sl_Input.at( 0 ).startsWith( "PANGAEA Home </>" ) == true ) )
    {
        while ( i < sl_Input.count() )
        {
            if ( sl_Input.at( i ).contains( "<!--RESULT ITEM START-->" ) == true )
            {
                while ( ( sl_Input.at( i ).contains( "/PANGAEA." ) == false ) && ( i < sl_Input.count() ) )
                    i++;

                s_Data = sl_Input.at( i ).section( "/PANGAEA.", 1, 1 ).section( "\"", 0, 0 );

                while ( ( sl_Input.at( i ).toLower().contains( "size:</td>" ) == false ) && ( i < sl_Input.count() ) )
                    i++;

                if ( ++i < sl_Input.count() )
                    s_Size = sl_Input.at( i );

                if ( s_Size.toLower().contains( "data points</td>" ) == true )
                    sl_Data.append( s_Data );

                if ( s_Size.toLower().contains( "unknown</td>" ) == true )
                    sl_Data.append( s_Data );

                if ( ( s_Size.toLower().contains( "datasets</td>" ) == true ) && ( ( b_DownloadCitation == true ) || ( b_DownloadMetadata == true ) ) )
                    sl_Data.append( s_Data + "\t" + "parent" );

                if ( s_Size.toLower().contains( "datasets</td>" ) == true )
                {
                    tout << "\t\t" << "Dataset " << s_Data << " is a parent" << s_EOL;
                    i_NumOfParents++;
                }
            }

            ++i;
        }
    }
    else
    {
        sl_Input.removeDuplicates();

        if ( sl_Input.at( 0 ).section( "\t", 0, 0 ).toLower() == "url" )
            b_isURL = true;

        if ( sl_Input.at( 0 ).section( "\t", 0, 0 ).toLower() == "uri" )
            b_isURL = true;

        if ( sl_Input.at( 0 ).section( "\t", 1, 1 ).toLower() == "filename" )
            b_ExportFileExists = true;

        if ( sl_Input.at( 0 ).section( "\t", 1, 1 ).toLower() == "file name" )
            b_ExportFileExists = true;

        if ( sl_Input.at( 0 ).section( "\t", 1, 1 ).toLower() == "file" )
            b_ExportFileExists = true;

        while ( ++i < sl_Input.count() )
        {
            s_Data = sl_Input.at( i );
            s_Data.replace( " ", "" );

            if ( s_Data.isEmpty() == false )
                sl_Data.append( s_Data );
        }
    }

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

    initFileProgress( i_totalNumOfDownloads, "", tr( "Downloading datasets..." ) );

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
                    // dowload PANGAEA data sets

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

                    QFileInfo fd( s_Filename );

                    if ( fd.size() == 0 )
                    {
                        i_removedDatasets++;
                        removeFile( s_Filename );
                        tout << s_DatasetID << "\t" << "login required or data set is a parent or static URL" << s_EOL;
                    }
                    else
                    {
                        if ( ( ( s_Filename.toLower().endsWith( ".txt" ) == true ) || ( s_Filename.toLower().endsWith( ".csv" ) == true ) ) && ( readFile( s_Filename, sl_Input, _SYSTEM_, 8000 ) > 0 ) )
                        {
                            if ( sl_Input.at( 0 ).startsWith( "/* DATA DESCRIPTION:" ) == false  )
                            {
                                removeFile( s_Filename );

                                i_removedDatasets++;

                                sl_Result = sl_Input.filter( "was substituted by an other version at" );

                                if ( sl_Result.count() > 0 )
                                    tout << "\t\t" << "Dataset " <<  s_DatasetID << " was substituted by an other version." << s_EOL;

                                sl_Result = sl_Input.filter( "No data available!" );

                                if ( sl_Result.count() > 0 )
                                    tout << "\t\t" << "Something wrong, no data available for dataset " << s_DatasetID << ". Please ask Rainer Sieger (rsieger@pangaea.de)" << s_EOL;

                                sl_Result = sl_Input.filter( "A data set identified by" );

                                if ( sl_Result.count() > 0 )
                                    tout << "\t\t" << "Dataset " <<  s_DatasetID << " not exist!" << s_EOL;

                                sl_Result = sl_Input.filter( "The dataset is currently not available for download. Try again later!" );

                                if ( sl_Result.count() > 0 )
                                    tout << s_DatasetID << "\t" << "Dataset not available at this time. Please try again later." << s_EOL;
                            }
                        }
                    }
                }
                else
                {
                    // dowload binary data with curl

                    downloadFile( s_Curl, s_Url, s_Filename );

                    QFileInfo fd( s_Filename );

                    if ( fd.size() == 0 )
                    {
                        i_removedDatasets++;
                        removeFile( s_Filename );
                        tout << s_Url << "\t" << QDir::toNativeSeparators( s_Filename ) << "\t" << "login required or file not found" << s_EOL;
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

void MainWindow::doGetDatasets()
{
    if ( doGetDatasetsDialog( gs_IDListFile, gs_DownloadDirectory, gb_DownloadData, gb_DownloadCitation, gb_DownloadMetadata, gi_CodecDownload, gi_Extension ) == QDialog::Accepted )
    {
        getDatasets( gs_IDListFile, gs_DownloadDirectory, gb_DownloadData, gb_DownloadCitation, gb_DownloadMetadata, gi_CodecDownload, gi_EOL, gi_Extension );

        if ( gs_DownloadDirectory.isEmpty() == false )
            chooseFolder( gs_DownloadDirectory );
    }

// **********************************************************************************************

    onError( _NOERROR_ );
}
