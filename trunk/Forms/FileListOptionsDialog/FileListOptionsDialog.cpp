// ***********************************************************************************************
// *                                                                                             *
// * FileListOptionsDialog.cpp - Sets external web path                                          *
// *                                                                                             *
// * Dr. Rainer Sieger - 2015-12-08                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "FileListOptionsDialog.h"

FileListOptionsDialog::FileListOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect( ClearAll_pushButton, SIGNAL( clicked() ), this, SLOT( clearAll() ) );
    connect( browseLocalDataDirButton, SIGNAL( clicked() ), this, SLOT( browseLocalDataDirDialog() ) );
    connect( OK_pushButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel_pushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void FileListOptionsDialog::clearAll()
{
    LocalDataDir_lineEdit->setText( "" );
    ExternalWebPath_lineEdit->setText( "" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString FileListOptionsDialog::getDocumentDir()
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

void FileListOptionsDialog::browseLocalDataDirDialog()
{
    QString fp	= "";
    QString dir	= LocalDataDir_lineEdit->text();

    if ( dir.isEmpty() == true )
        dir = getDocumentDir();

    fp = QFileDialog::getExistingDirectory( this, tr( "Choose Directory" ), dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog );

    if ( fp.isEmpty() == false )
    {
        QFileInfo fi( fp );

        if ( ( fi.isFile() == false ) && ( fi.exists() == true ) )
        {
            if ( fp.endsWith( QDir::toNativeSeparators( "/" ) ) == false )
                fp = fp.append( QDir::toNativeSeparators( "/" ) );

            LocalDataDir_lineEdit->setText( QDir::toNativeSeparators( fp ) );
        }
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void FileListOptionsDialog::dragEnterEvent( QDragEnterEvent *event )
{
    if ( event->mimeData()->hasFormat( "text/uri-list" ) )
        event->acceptProposedAction();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void FileListOptionsDialog::dropEvent( QDropEvent *event )
{
    QList<QUrl> urls = event->mimeData()->urls();

    if ( urls.isEmpty() == true )
        return;

    QString s_fileName = urls.first().toLocalFile();

    if ( s_fileName.isEmpty() == true )
        return;

    QFileInfo fi( s_fileName );

    if ( ( fi.isFile() == false ) && ( fi.exists() == true ) )
    {
        if ( s_fileName.endsWith( QDir::toNativeSeparators( "/" ) ) == false )
            s_fileName = s_fileName.append( QDir::toNativeSeparators( "/" ) );

        LocalDataDir_lineEdit->setText( QDir::toNativeSeparators( s_fileName ) );
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doFileListOptionsDialog( QString &s_LocalDataDir, QString &s_ExternalWebPath )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    FileListOptionsDialog dialog( this );

    QFileInfo fi( s_LocalDataDir );

    if ( ( fi.isFile() == false ) && ( fi.exists() == true ) )
        dialog.LocalDataDir_lineEdit->setText( QDir::toNativeSeparators( s_LocalDataDir ) );
    else
        dialog.LocalDataDir_lineEdit->setText( QDir::toNativeSeparators( getDocumentDir() + "/" ) );

    dialog.ExternalWebPath_lineEdit->setText( s_ExternalWebPath );

    dialog.LocalDataDir_lineEdit->selectAll();

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        s_LocalDataDir =  QDir::toNativeSeparators( dialog.LocalDataDir_lineEdit->text() );

        if ( s_LocalDataDir.endsWith( QDir::toNativeSeparators( "/" ) ) == false )
            s_LocalDataDir.append( QDir::toNativeSeparators( "/" ) );

        s_ExternalWebPath = dialog.ExternalWebPath_lineEdit->text();

        if ( s_ExternalWebPath.contains( "/hs/usero/") == true )
            s_ExternalWebPath = tr( "http://hs.pangaea.de/" ) + s_ExternalWebPath.section( "/hs/usero/", 1, 1 );

        if ( s_ExternalWebPath.contains( "/pangaea-family/store/") == true )
            s_ExternalWebPath = tr( "http://store.pangaea.de/" ) + s_ExternalWebPath.section( "/pangaea-family/store/", 1, 1 );

        if ( s_ExternalWebPath.contains( "/pangaea-family/www/") == true )
            s_ExternalWebPath = tr( "https://www.pangaea.de/" ) + s_ExternalWebPath.section( "/pangaea-family/www/", 1, 1 );

        if ( (s_ExternalWebPath.startsWith( "http" ) == true ) && ( s_ExternalWebPath.endsWith( "/" ) == false ) )
            s_ExternalWebPath.append( "/" );

        s_ExternalWebPath = s_ExternalWebPath.toLower();

        if (    ( s_ExternalWebPath.isEmpty() == true )
             || ( s_ExternalWebPath.startsWith( "http://hs.pangaea.de/" ) == true )
             || ( s_ExternalWebPath.startsWith( "http://store.pangaea.de/" ) == true )
             || ( s_ExternalWebPath.startsWith( "https://www.pangaea.de/" ) == true )
             || ( s_ExternalWebPath.startsWith( "doi:" ) == true )
             || ( s_ExternalWebPath.startsWith( "hdl:" ) == true )
           )
           i_DialogResult = QDialog::Accepted;
        else
           i_DialogResult = QDialog::Rejected;

        if ( i_DialogResult != QDialog::Accepted )
        {
            QString s_Message = tr( "Wrong format of URI\n  " ) + s_ExternalWebPath +
                                tr( "\n\nURI must start with:\n  - http://hs.pangaea.de/ or \n  - http://store.pangaea.de/ or \n  - https://www.pangaea.de/ or \n  - doi: or \n  - hdl:" );
            QMessageBox::information( this, getApplicationName( true ), s_Message );
        }
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
