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
    connect( OK_pushButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel_pushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void FileListOptionsDialog::clearAll()
{
    ExternalWebPath_lineEdit->setText( "" );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doFileListOptionsDialog( QString &s_ExternalWebPath )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    FileListOptionsDialog dialog( this );

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
        s_ExternalWebPath = dialog.ExternalWebPath_lineEdit->text();

        if ( (s_ExternalWebPath.startsWith( "http" ) == true ) && ( s_ExternalWebPath.endsWith( "/" ) == false ) )
            s_ExternalWebPath.append( "/" );

        if (
                ( s_ExternalWebPath.startsWith( "http://hs.pangaea.de" ) == true )
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
