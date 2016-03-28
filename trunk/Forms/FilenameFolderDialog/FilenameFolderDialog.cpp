/* FilenameFolderDialog.cpp			  */
/* 2012-11-18                         */
/* Dr. Rainer Sieger                  */

#include <QtWidgets>

#include "Application.h"
#include "FilenameFolderDialog.h"

FilenameFolderDialog::FilenameFolderDialog(QWidget *parent) : QDialog(parent)
{
    int		i_minWidth = 8*fontMetrics().width( 'w' ) + 2;

// **********************************************************************************************
// Dialog

    setupUi( this );

    connect( OKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( CancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( HelpButton, SIGNAL( clicked() ), this, SLOT( displayHelp() ) );
    connect( browseFilenameListFileButton, SIGNAL( clicked() ), this, SLOT( browseFilenameListFileDialog() ) );
    connect( browseTargetDirectoryButton, SIGNAL( clicked() ), this, SLOT( browseTargetDirectoryDialog() ) );
    connect( FilenameListFileLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( TargetDirectoryLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

// **********************************************************************************************

    FileTextLabel->setMinimumWidth( i_minWidth );
    DirTextLabel->setMinimumWidth( i_minWidth );

    enableOKButton();

    OKButton->setFocus();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString FilenameFolderDialog::getDocumentDir()
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

void FilenameFolderDialog::enableOKButton()
{
    bool b_OK = true;

    QFileInfo fi( FilenameListFileLineEdit->text() );

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        b_OK = false;

    QFileInfo di( TargetDirectoryLineEdit->text() );

    if ( di.isDir() == false )
        b_OK = false;

    if ( b_OK == true )
    {
        OKButton->setEnabled( true );
        OKButton->setDefault( true );
    }
    else
    {
        OKButton->setEnabled( false );
        CancelButton->setDefault( true );
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void FilenameFolderDialog::displayHelp()
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

void FilenameFolderDialog::browseFilenameListFileDialog()
{
    QString	fn   = "";
    QString file = FilenameListFileLineEdit->text();

    QFileInfo fi( file );

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        file = getDocumentDir();

    fn = QFileDialog::getOpenFileName( this, tr( "Select an ID file (*.txt, *.csv, *.html)" ), file, tr( "ID file (*.txt *.csv *.html *.htm)" ), 0, QFileDialog::DontUseNativeDialog );

    if ( fn.isEmpty() == false )
        fi.setFile( fn );
    else
        fn = file;

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        FilenameListFileLineEdit->clear();
    else
        FilenameListFileLineEdit->setText( QDir::toNativeSeparators( fn ) );

    FilenameListFileLineEdit->setFocus();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void FilenameFolderDialog::browseTargetDirectoryDialog()
{
    QString fp	= "";
    QString dir	= TargetDirectoryLineEdit->text();

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

            TargetDirectoryLineEdit->setText( QDir::toNativeSeparators( fp ) );
        }
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void FilenameFolderDialog::dragEnterEvent( QDragEnterEvent *event )
{
    if ( event->mimeData()->hasFormat( "text/uri-list" ) )
        event->acceptProposedAction();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void FilenameFolderDialog::dropEvent( QDropEvent *event )
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
            FilenameListFileLineEdit->setText( QDir::toNativeSeparators( s_fileName ) );
    }
    else
    {
        if ( s_fileName.endsWith( QDir::toNativeSeparators( "/" ) ) == true )
            s_fileName = s_fileName.remove( s_fileName.length()-1, 1 );

        TargetDirectoryLineEdit->setText( QDir::toNativeSeparators( s_fileName ) );
    }
}

