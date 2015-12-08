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

    connect( ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll() ) );
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
        gs_ExternalWebPath = dialog.ExternalWebPath_lineEdit->text();
        i_DialogResult     = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    if ( ( gs_ExternalWebPath.startsWith( "http://" ) == true ) && ( gs_ExternalWebPath.endsWith( "/" ) == false ) )
        gs_ExternalWebPath.append( "/" );

    posDialog = dialog.pos();

    return( i_DialogResult );
}
