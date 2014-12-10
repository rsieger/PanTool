// ***********************************************************************************************
// *                                                                                             *
// * RenameFilesDialog.cpp - Search and replace dialog for filenames                             *
// *                                                                                             *
// * Dr. Rainer Sieger - 2013-03-06                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "RenameFilesDialog.h"

RenameFilesDialog::RenameFilesDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( SearchFor_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( ReplaceWith_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void RenameFilesDialog::clearAll()
{
    SearchFor_lineEdit->setText( "" );
    ReplaceWith_lineEdit->setText( "" );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void RenameFilesDialog::enableOKButton()
{
    bool b_OK = true;

    if ( SearchFor_lineEdit->text().isEmpty() == true )
        b_OK = false;

    if ( b_OK == true )
    {
        OK_pushButton->setEnabled( true );
        OK_pushButton->setDefault( true );
    }
    else
    {
        OK_pushButton->setEnabled( false );
        Cancel_pushButton->setDefault( true );
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doRenameFilesDialog( QString &s_SearchString, QString &s_ReplaceString )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    RenameFilesDialog dialog( this );

    dialog.SearchFor_lineEdit->setText( s_SearchString );
    dialog.ReplaceWith_lineEdit->setText( s_ReplaceString );

    dialog.SearchFor_lineEdit->selectAll();

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.enableOKButton();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        s_SearchString   = dialog.SearchFor_lineEdit->text();
        s_ReplaceString  = dialog.ReplaceWith_lineEdit->text();

        i_DialogResult   = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
