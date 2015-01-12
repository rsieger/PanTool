/* PanGetDialog.cpp			  */
/* 2012-09-30                 */
/* Dr. Rainer Sieger          */

#include <QtWidgets>

#include "Application.h"
#include "PanGetDialog.h"

PanGetDialog::PanGetDialog(QWidget *parent) : QDialog(parent)
{
    int		i_minWidth = 8*fontMetrics().width( 'w' ) + 2;

// **********************************************************************************************

    s_PrefVersion = "2007-08-06";

// **********************************************************************************************
// Dialog

    setupUi( this );

    connect( BuildScript_pushButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel_pushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( HelpButton, SIGNAL( clicked() ), this, SLOT( displayHelp() ) );
    connect( browseIDListFileButton, SIGNAL( clicked() ), this, SLOT( browseIDListFileDialog() ) );
    connect( browseDownloadDirectoryButton, SIGNAL( clicked() ), this, SLOT( browseDownloadDirectoryDialog() ) );
    connect( IDListFileLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableBuildButton() ) );
    connect( DownloadDirectoryLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableBuildButton() ) );

// **********************************************************************************************

    FileTextLabel->setMinimumWidth( i_minWidth );
    DirTextLabel->setMinimumWidth( i_minWidth );

    enableBuildButton();

    BuildScript_pushButton->setFocus();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString PanGetDialog::getDocumentDir()
{
#if defined(Q_OS_LINUX)
    return( QDir::homePath() );
#endif

#if defined(Q_OS_MAC)
    return( QDir::homePath() );
#endif

#if defined(Q_OS_WIN)
    return( QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation ) );
#endif
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void PanGetDialog::enableBuildButton()
{
    bool b_OK = true;

    QFileInfo fi( IDListFileLineEdit->text() );

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        b_OK = false;

    QFileInfo di( DownloadDirectoryLineEdit->text() );

    if ( di.isDir() == false )
        b_OK = false;

    if ( b_OK == true )
    {
        BuildScript_pushButton->setEnabled( true );
        BuildScript_pushButton->setDefault( true );
    }
    else
    {
        BuildScript_pushButton->setEnabled( false );
        Cancel_pushButton->setDefault( true );
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void PanGetDialog::displayHelp()
{
    textViewer = new QTextEdit;
    textViewer->setReadOnly(true);

    QFile file("readme.html");

    if (file.open(QIODevice::ReadOnly))
    {
        textViewer->setHtml(file.readAll());
        textViewer->resize(750, 700);
        textViewer->show();
    }
    else
    {
        QString s_ApplicationName = "PanGet";

        QDesktopServices::openUrl( QUrl( tr( "http://wiki.pangaea.de/wiki/" ) + s_ApplicationName ) );
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void PanGetDialog::browseIDListFileDialog()
{
    QString	fn   = "";
    QString file = IDListFileLineEdit->text();

    QFileInfo fi( file );

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        file = getDocumentDir();

    fn = QFileDialog::getOpenFileName( this, tr( "Select an ID file (*.txt, *.csv, *.html)" ), file, tr( "ID file (*.txt *.csv *.html *.htm)" ), 0, QFileDialog::DontUseNativeDialog );

    if ( fn.isEmpty() == false )
        fi.setFile( fn );
    else
        fn = file;

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        IDListFileLineEdit->clear();
    else
        IDListFileLineEdit->setText( QDir::toNativeSeparators( fn ) );

    IDListFileLineEdit->setFocus();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void PanGetDialog::browseDownloadDirectoryDialog()
{
    QString fp	= "";
    QString dir	= DownloadDirectoryLineEdit->text();

    if ( dir.isEmpty() == true )
        dir = getDocumentDir();

    fp = QFileDialog::getExistingDirectory( this, tr( "Choose Directory" ), dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog );

    if ( fp.isEmpty() == false )
    {
        QFileInfo fi( fp );

        if ( fi.exists() == true )
        {
            if ( fp.endsWith( QDir::toNativeSeparators( "/" ) ) == true )
                fp = fp.remove( fp.length()-1, 1 );

            DownloadDirectoryLineEdit->setText( QDir::toNativeSeparators( fp ) );
        }
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void PanGetDialog::dragEnterEvent( QDragEnterEvent *event )
{
    if ( event->mimeData()->hasFormat( "text/uri-list" ) )
        event->acceptProposedAction();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void PanGetDialog::dropEvent( QDropEvent *event )
{
    QList<QUrl> urls = event->mimeData()->urls();

    if ( urls.isEmpty() == true )
        return;

    QString s_fileName = urls.first().toLocalFile();

    if ( s_fileName.isEmpty() == true )
        return;

    QFileInfo fi( s_fileName );

    if ( fi.isFile() == true )
    {
        if ( ( fi.suffix().toLower() == "txt" ) || ( fi.suffix().toLower() == "csv" ) || ( fi.suffix().toLower() == "html" )  || ( fi.suffix().toLower() == "htm" ) )
            IDListFileLineEdit->setText( QDir::toNativeSeparators( s_fileName ) );
    }
    else
    {
        if ( s_fileName.endsWith( QDir::toNativeSeparators( "/" ) ) == true )
            s_fileName = s_fileName.remove( s_fileName.length()-1, 1 );

        DownloadDirectoryLineEdit->setText( QDir::toNativeSeparators( s_fileName ) );
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::downloadDatasets( const QString &s_IDListFile, const QString &s_DownloadDirectory, const bool b_DownloadData, const bool b_DownloadCitation, const bool b_DownloadMetadata, const int i_CodecDownload, const int i_EOL, const int i_Extension )
{
    int i       							= 0;
    int n                                   = 0;
    int err                                 = _NOERROR_;

    int	stopProgress						= 0;
    int	i_NumOfParents						= 0;
    int i_totalNumOfDownloads				= 0;
    int i_removedDatasets					= 0;

    QString s_Message						= "";
    QString s_DatasetID						= "";
    QString	s_Data							= "";
    QString s_Domain                        = "";
    QString s_ExportFilename				= "";
    QString s_Url							= "";
    QString s_Size							= "";
    QString s_EOL                           = setEOLChar( i_EOL );

    QStringList	sl_Input;
    QStringList sl_Data;
    QStringList sl_Result;

    bool	b_ExportFilenameExists			= false;
    bool	b_isURL             			= false;

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

// **********************************************************************************************

    tout << "*ID\tExport filename\tComment" << s_EOL;

// **********************************************************************************************
// Read data

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

        if ( sl_Input.at( 0 ).section( "\t", 1, 1 ).toLower() == "export filename" )
            b_ExportFilenameExists = true;
        if ( sl_Input.at( 0 ).section( "\t", 1, 1 ).toLower() == "filename" )
            b_ExportFilenameExists = true;
        if ( sl_Input.at( 0 ).section( "\t", 1, 1 ).toLower() == "file" )
            b_ExportFilenameExists = true;

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

    initFileProgress( i_totalNumOfDownloads, "", tr( "Downloading files..." ) );

    i = 0;

    while ( ( i < i_totalNumOfDownloads ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( b_isURL == true )
        {
            s_Domain = sl_Data.at( i ).section( "/", 0, 2 ); // eg. http://iodp.tamu.edu/

            if ( sl_Data.at( i ).section( "\t", 0, 0 ).section( "/", 3 ).isEmpty() == false )
                s_Url = "/" + sl_Data.at( i ).section( "\t", 0, 0 ).section( "/", 3 ); // eg. /janusweb/chemistry/chemcarb.cgi?leg=197&site=1203&hole=A
        }
        else
        {
            s_Domain    = "http://doi.pangaea.de"; // PANGAEA datasets

            s_DatasetID = sl_Data.at( i ).section( "\t", 0, 0 );

            s_DatasetID.replace( tr( "http://doi.pangaea.de/10.1594/PANGAEA." ), tr( "" ) );
            s_DatasetID.replace( tr( "doi:10.1594/PANGAEA." ), tr( "" ) );
            s_DatasetID.replace( tr( "Dataset ID: " ), tr( "" ) );
            s_DatasetID.replace( tr( ", unpublished dataset" ), tr( "" ) );
            s_DatasetID.replace( tr( ", DOI registration in progress" ), tr( "" ) );
        }

        if ( b_ExportFilenameExists == true )
        {
            s_ExportFilename = sl_Data.at( i ).section( "\t", 1, 1 );
            s_ExportFilename.replace( " ", "_" );

            if ( s_ExportFilename.isEmpty() == true )
            {
                if ( b_isURL == true )
                    s_ExportFilename = sl_Data.at( i ).section( "\t", 0, 0 ).section( "/", -1, -1 );
                else
                    s_ExportFilename = tr( "not_given" );
            }

            if ( b_isURL == false )
            {
                s_ExportFilename.append( tr( "~" ) );
                s_ExportFilename.append( s_DatasetID );
            }
        }
        else
        {
            if ( b_isURL == true )
                s_ExportFilename = sl_Data.at( i ).section( "\t", 0, 0 ).section( "/", -1, -1 );
            else
                s_ExportFilename.sprintf( "%06d", s_DatasetID.toInt() );
        }

        if ( ( b_DownloadCitation == true ) && ( b_isURL == false ) )
        {
            s_Url = "http://doi.pangaea.de/10.1594/PANGAEA."+ s_DatasetID + "?format=citation_text";

            downloadFile( s_Url, s_DownloadDirectory + "/" + s_ExportFilename + "_citation" + setExtension( i_Extension ) );

            wait( 100 );
        }

        if ( ( b_DownloadMetadata == true ) && ( b_isURL == false ) )
        {
            s_Url = "http://doi.pangaea.de/10.1594/PANGAEA."+ s_DatasetID + "?format=metainfo_xml";

            downloadFile( s_Url, s_DownloadDirectory + "/" + s_ExportFilename + "_metadata.xml" );

            wait( 100 );
        }

        if ( b_DownloadData == true )
        {
            if ( ( s_Url.isEmpty() == false ) || ( s_DatasetID.toInt() >= 50000 ) )
            {
                s_ExportFilename = s_DownloadDirectory + "/" + s_ExportFilename + setExtension( i_Extension );

                if ( b_isURL == false )
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

                downloadFile( s_Url, s_ExportFilename );

                wait( 100 );

                QFile fileExport( s_ExportFilename );
                QFileInfo fd( s_ExportFilename );

                if ( fd.size() == 0 )
                {
                    fileExport.remove();

                    i_removedDatasets++;

                    switch ( i_Extension )
                    {
                    case _CSV_:
                        tout << s_DatasetID << "\t" << s_ExportFilename.sprintf( "%06d.csv", s_DatasetID.toInt() ) << "\t" << "login required or data set is parent" << s_EOL;
                        break;
                    default:
                        tout << s_DatasetID << "\t" << s_ExportFilename.sprintf( "%06d.txt", s_DatasetID.toInt() ) << "\t" << "login required or data set is parent" << s_EOL;
                        break;
                    }
                }
                else
                {
                    if ( ( ( s_ExportFilename.toLower().endsWith( ".txt" ) == true ) || ( s_ExportFilename.toLower().endsWith( ".csv" ) == true ) ) && ( readFile( s_ExportFilename, sl_Input, _SYSTEM_, 8000 ) > 0 ) )
                    {
                        if ( sl_Input.at( 0 ).startsWith( "/* DATA DESCRIPTION:" ) == false  )
                        {
                            fileExport.remove();

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
                            {
                                switch ( i_Extension )
                                {
                                case _CSV_:
                                    tout << s_DatasetID << "\t" << s_ExportFilename.sprintf( "%06d.csv", s_DatasetID.toInt() ) << "\t" << "Dataset not available at this time. Please try again later." << s_EOL;
                                    break;
                                default:
                                    tout << s_DatasetID << "\t" << s_ExportFilename.sprintf( "%06d.txt", s_DatasetID.toInt() ) << "\t" << "Dataset not available at this time. Please try again later." << s_EOL;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                err = _ERROR_;
            }
        }

        stopProgress = incFileProgress( i_totalNumOfDownloads, i++ );
    }

// **********************************************************************************************

    resetFileProgress( i_totalNumOfDownloads );

    fout.close();

// **********************************************************************************************

    if ( err == _NOERROR_ )
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

                    fout.remove();
                }
            }
        }
    }
    else
    {
        if ( b_isURL == true )
        {
            s_Message = tr( "URL has to be given." );
            QMessageBox::information( this, getApplicationName( true ), s_Message );
        }
        else
        {
            if ( s_DatasetID.toInt() != 0 )
            {
                s_Message = tr( "The dataset ID\nmust be greater than 50,000." );
                QMessageBox::information( this, getApplicationName( true ), s_Message );
            }
            else
            {
                s_Message = tr( "Wrong format! The dataset ID\nmust be in the first column." );
                QMessageBox::information( this, getApplicationName( true ), s_Message );
            }
        }
    }

// **********************************************************************************************

    return;
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doGetDatasets()
{
    int err = _ERROR_;

    PanGetDialog dialog( this );

    dialog.setWindowTitle( getApplicationName( true ) + tr( " - download PANGAEA datasets" ) );
    dialog.setSizeGripEnabled ( true );

    dialog.move( posDialog );
    dialog.resize( 600, dialog.minimumHeight() );

    dialog.CodecDownload_ComboBox->setCurrentIndex( gi_CodecDownload );

    switch ( gi_Extension )
    {
    case _CSV_:
        dialog.CSV_radioButton->setChecked( true );
        break;
    default:
        dialog.TXT_radioButton->setChecked( true );
        break;
    }

    dialog.DownloadData_checkBox->setChecked( gb_DownloadData );
    dialog.DownloadCitation_checkBox->setChecked( gb_DownloadCitation );
    dialog.DownloadMetadata_checkBox->setChecked( gb_DownloadMetadata );

// **********************************************************************************************

    QFileInfo fi( gs_IDListFile );

    if ( ( fi.isFile() == true ) && ( fi.exists() == true ) && ( ( fi.suffix().toLower() == "txt" ) || ( fi.suffix().toLower() == "csv" ) || ( fi.suffix().toLower() == "html" )  || ( fi.suffix().toLower() == "htm" ) ) )
        dialog.IDListFileLineEdit->setText( QDir::toNativeSeparators( gs_IDListFile ) );
    else
        dialog.IDListFileLineEdit->clear();

// **********************************************************************************************

    QFileInfo di( gs_DownloadDirectory );

    if ( ( di.isDir() == true ) && ( di.exists() == true ) )
    {
        if ( gs_DownloadDirectory.endsWith( QDir::toNativeSeparators( "/" ) ) == true )
            gs_DownloadDirectory = gs_DownloadDirectory.remove( gs_DownloadDirectory.length()-1, 1 );

        dialog.DownloadDirectoryLineEdit->insert( QDir::toNativeSeparators( gs_DownloadDirectory ) );
    }
    else
    {
        dialog.DownloadDirectoryLineEdit->clear();
    }

// **********************************************************************************************

    dialog.BuildScript_pushButton->setWhatsThis( "Build script" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gi_CodecDownload     = dialog.CodecDownload_ComboBox->currentIndex();
        gs_IDListFile        = dialog.IDListFileLineEdit->text();
        gs_DownloadDirectory = dialog.DownloadDirectoryLineEdit->text();

        gi_Extension         = _TXT_;

        if ( dialog.CSV_radioButton->isChecked() )
            gi_Extension = _CSV_;

        gb_DownloadData     = dialog.DownloadData_checkBox->isChecked();
        gb_DownloadCitation = dialog.DownloadCitation_checkBox->isChecked();
        gb_DownloadMetadata = dialog.DownloadMetadata_checkBox->isChecked();

        err = _NOERROR_;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

// **********************************************************************************************

    if ( err == _NOERROR_ )
    {
        downloadDatasets( gs_IDListFile, gs_DownloadDirectory, gb_DownloadData, gb_DownloadCitation, gb_DownloadMetadata, gi_CodecDownload, gi_EOL, gi_Extension );

        if ( ( gs_DownloadDirectory.isEmpty() == false ) && ( err == _NOERROR_ ) )
            chooseFolder( gs_DownloadDirectory );
    }

    return;
}
