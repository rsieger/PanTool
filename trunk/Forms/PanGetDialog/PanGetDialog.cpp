/* PanGetDialog.cpp			  */
/* 2016-04-23                 */
/* Dr. Rainer Sieger          */

#include <QtWidgets>

#include "Application.h"
#include "PanGetDialog.h"

PanGetDialog::PanGetDialog( QWidget *parent ) : QDialog( parent )
{
    int		i_minWidth = 8*fontMetrics().width( 'w' ) + 2;

// **********************************************************************************************

    s_PrefVersion = "2007-08-06";

// **********************************************************************************************
// Dialog

    setupUi( this );

    connect( GetDatasets_pushButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel_pushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( Help_pushButton, SIGNAL( clicked() ), this, SLOT( displayHelp() ) );
    connect( browseIDListFile_pushButton, SIGNAL( clicked() ), this, SLOT( browseIDListFileDialog() ) );
    connect( browseDownloadDirectory_pushButton, SIGNAL( clicked() ), this, SLOT( browseDownloadDirectoryDialog() ) );
    connect( QueryLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableBuildButton() ) );
    connect( IDListFileLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableBuildButton() ) );
    connect( DownloadDirectoryLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableBuildButton() ) );
    connect( Clear_pushButton, SIGNAL( clicked() ), this, SLOT( clear() ) );

// **********************************************************************************************

    FileTextLabel->setMinimumWidth( i_minWidth );
    DirTextLabel->setMinimumWidth( i_minWidth );

    enableBuildButton();

    GetDatasets_pushButton->setFocus();
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

    if ( ( ( fi.isFile() == false ) || ( fi.exists() == false ) ) && ( QueryLineEdit->text().toLower().startsWith( "https://pangaea.de/?q" ) == false ) )
        b_OK = false;

    QFileInfo di( DownloadDirectoryLineEdit->text() );

    if ( di.isDir() == false )
        b_OK = false;

    if ( b_OK == true )
    {
        GetDatasets_pushButton->setEnabled( true );
        GetDatasets_pushButton->setDefault( true );
    }
    else
    {
        GetDatasets_pushButton->setEnabled( false );
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

        QDesktopServices::openUrl( QUrl( tr( "https://wiki.pangaea.de/wiki/" ) + s_ApplicationName ) );
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

void PanGetDialog::clear()
{
    QueryLineEdit->clear();
    IDListFileLineEdit->clear();
    DownloadDirectoryLineEdit->clear();

    enableBuildButton();
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
        if ( ( fi.suffix().toLower() == "txt" ) || ( fi.suffix().toLower() == "csv" ) )
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

int MainWindow::doGetDatasetsDialog( QString &s_Query, QString &s_IDListFile, QString &s_DownloadDirectory, bool &b_DownloadData, bool &b_DownloadCitation, bool &b_DownloadMetadata, int &i_CodecDownload, int &i_Extension )
{
    int i_DialogResult = QDialog::Rejected;

// **********************************************************************************************

    PanGetDialog dialog( this );

// **********************************************************************************************
// set Codec

    dialog.CodecDownload_ComboBox->setCurrentIndex( i_CodecDownload );

// **********************************************************************************************
// set PANGAEA Query

    if ( s_Query.toLower().startsWith( "https://pangaea.de/?q" ) == true )
        dialog.QueryLineEdit->setText( s_Query );

// **********************************************************************************************
// set ID File

    if ( s_IDListFile.isEmpty() == false )
    {
        QFileInfo fi( s_IDListFile );

        if ( ( fi.isFile() == true ) && ( fi.exists() == true ) )
            dialog.IDListFileLineEdit->setText( QDir::toNativeSeparators( s_IDListFile ) );
    }

// **********************************************************************************************
// set Download directory

    if ( s_DownloadDirectory.isEmpty() == false )
    {
        QFileInfo di( s_DownloadDirectory );

        if ( ( di.isDir() == true ) && ( di.exists() == true ) )
        {
            if ( s_DownloadDirectory.endsWith( QDir::toNativeSeparators( "/" ) ) == true )
                s_DownloadDirectory = s_DownloadDirectory.remove( s_DownloadDirectory.length()-1, 1 );

            dialog.DownloadDirectoryLineEdit->setText( QDir::toNativeSeparators( s_DownloadDirectory ) );
        }
    }

// **********************************************************************************************

    switch ( i_Extension )
    {
    case _CSV_:
        dialog.CSV_radioButton->setChecked( true );
        break;
    default:
        dialog.TXT_radioButton->setChecked( true );
        break;
    }

// **********************************************************************************************

    dialog.DownloadData_checkBox->setChecked( b_DownloadData );
    dialog.DownloadCitation_checkBox->setChecked( b_DownloadCitation );
    dialog.DownloadMetadata_checkBox->setChecked( b_DownloadMetadata );

// **********************************************************************************************

    dialog.setWindowTitle( getApplicationName( true ) + tr( " - download PANGAEA datasets" ) );
    dialog.setSizeGripEnabled( true );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        i_CodecDownload     = dialog.CodecDownload_ComboBox->currentIndex();
        s_Query             = dialog.QueryLineEdit->text();
        s_IDListFile        = dialog.IDListFileLineEdit->text();
        s_DownloadDirectory = dialog.DownloadDirectoryLineEdit->text();

        i_Extension         = _TXT_;

        if ( dialog.CSV_radioButton->isChecked() )
            i_Extension = _CSV_;

        b_DownloadData     = dialog.DownloadData_checkBox->isChecked();
        b_DownloadCitation = dialog.DownloadCitation_checkBox->isChecked();
        b_DownloadMetadata = dialog.DownloadMetadata_checkBox->isChecked();

        i_DialogResult      = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
