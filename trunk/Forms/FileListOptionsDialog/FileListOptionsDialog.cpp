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
    connect( browseLocalRootDirButton, SIGNAL( clicked() ), this, SLOT( browseLocalRootDirDialog() ) );
    connect( OK_pushButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel_pushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void FileListOptionsDialog::clearAll()
{
    LocalRootDir_lineEdit->setText( "" );
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

void FileListOptionsDialog::browseLocalRootDirDialog()
{
    QString fp	= "";
    QString dir	= LocalRootDir_lineEdit->text();

    if ( dir.isEmpty() == true )
        dir = getDocumentDir();

    fp = QFileDialog::getExistingDirectory( this, tr( "Choose Directory" ), dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog );

    if ( fp.isEmpty() == false )
    {
        QFileInfo fi( fp );

        if ( fi.exists() == true )
        {
            if ( fp.endsWith( QDir::toNativeSeparators( "/" ) ) == false )
                fp = fp.append( "/" );

            LocalRootDir_lineEdit->setText( QDir::toNativeSeparators( fp ) );
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

    if ( fi.isFile() == false )
    {
        if ( s_fileName.endsWith( QDir::toNativeSeparators( "/" ) ) == false )
            s_fileName = s_fileName.append( "/" );

        LocalRootDir_lineEdit->setText( QDir::toNativeSeparators( s_fileName ) );
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doFileListOptionsDialog( QString &s_LocalRootDir, QString &s_ExternalWebPath )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    FileListOptionsDialog dialog( this );

    dialog.LocalRootDir_lineEdit->setText( s_LocalRootDir );
    dialog.ExternalWebPath_lineEdit->setText( s_ExternalWebPath );

    dialog.ExternalWebPath_lineEdit->selectAll();

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        s_LocalRootDir = dialog.LocalRootDir_lineEdit->text();

        if ( s_LocalRootDir.endsWith( "/" ) == false )
            s_LocalRootDir.append( "/" );

        s_ExternalWebPath = dialog.ExternalWebPath_lineEdit->text();

        if ( (s_ExternalWebPath.startsWith( "http" ) == true ) && ( s_ExternalWebPath.endsWith( "/" ) == false ) )
            s_ExternalWebPath.append( "/" );

        if (    ( s_ExternalWebPath.isEmpty() == true )
             || ( s_ExternalWebPath.startsWith( "http://hs.pangaea.de" ) == true )
             || ( s_ExternalWebPath.startsWith( "http://store.pangaea.de" ) == true )
             || ( s_ExternalWebPath.startsWith( "https://store.pangaea.de" ) == true )
             || ( s_ExternalWebPath.startsWith( "doi:" ) == true )
             || ( s_ExternalWebPath.startsWith( "hdl:" ) == true )
           )
           i_DialogResult = QDialog::Accepted;
        else
           i_DialogResult = QDialog::Rejected;

        if ( i_DialogResult != QDialog::Accepted )
            QMessageBox::information( this, getApplicationName( true ), tr( "Wrong format of URI" ) );

        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
