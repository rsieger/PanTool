// ***********************************************************************************************
// *                                                                                             *
// * ColumnListDialog.cpp - Sets column list                                                     *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-07-21                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "ColumnListDialog.h"

ColumnListDialog::ColumnListDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    QRegExp regExp( "[0-9,-end]{1024}" );
    ColumnList_lineEdit->setValidator( new QRegExpValidator( regExp, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( ColumnList_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void ColumnListDialog::clearAll()
{
    ColumnList_lineEdit->setText( "" );

    SkipEmptyLines_checkBox->setChecked( false );
    SkipCommentLines_checkBox->setChecked( false );
    DeleteInputFile_checkBox->setChecked( false );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void ColumnListDialog::enableOKButton()
{
    bool b_OK = true;

    if ( ColumnList_lineEdit->text().isEmpty() == true )
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

int MainWindow::doColumnListDialog( const int mode, QString &s_ColumnList, bool &b_SkipEmptyLines, bool &b_SkipCommentLines, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    ColumnListDialog dialog( this );

    switch ( mode )
    {
    case _EXTRACTCOLUMNS:
        dialog.setWindowTitle( tr( "Extract columns dialog" ) );
        break;
    case _DELETECOLUMNS:
        dialog.setWindowTitle( tr( "Delete columns dialog" ) );
    default:
        break;
    }

    dialog.ColumnList_lineEdit->setText( s_ColumnList );
    dialog.SkipEmptyLines_checkBox->setChecked( b_SkipEmptyLines );
    dialog.SkipCommentLines_checkBox->setChecked( b_SkipCommentLines );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    dialog.OK_pushButton->setWhatsThis( tr( "Close dialog" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    if ( gsl_FilenameList.count() > 1 )
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );

    dialog.ColumnList_lineEdit->selectAll();

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
        s_ColumnList       = dialog.ColumnList_lineEdit->text().toLower();
        b_SkipEmptyLines   = dialog.SkipEmptyLines_checkBox->isChecked();
        b_SkipCommentLines = dialog.SkipCommentLines_checkBox->isChecked();
        b_DeleteInputFile  = dialog.DeleteInputFile_checkBox->isChecked();

        i_DialogResult     = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
