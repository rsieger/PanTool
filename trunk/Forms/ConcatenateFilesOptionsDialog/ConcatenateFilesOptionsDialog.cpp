// ***********************************************************************************************
// *                                                                                             *
// * ConcatenateFilesOptionsDialog.cpp - Sets options for the concatenate files routine       *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-07-18                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "ConcatenateFilesOptionsDialog.h"

ConcatenateFilesOptionsDialog::ConcatenateFilesOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    SkipNStartLines_lineEdit->setValidator( new QIntValidator( 0, 9999999, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( SkipNStartLines_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void ConcatenateFilesOptionsDialog::clearAll()
{
    SkipNStartLines_lineEdit->setText( "0" );

    IncludeFilename_checkBox->setChecked( false );
    SkipEmptyLines_checkBox->setChecked( false );
    SkipCommentLines_checkBox->setChecked( false );
    DeleteInputFile_checkBox->setChecked( false );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void ConcatenateFilesOptionsDialog::enableOKButton()
{
    bool b_OK = true;

    if ( SkipNStartLines_lineEdit->text().isEmpty() == true )
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

int MainWindow::doConcatenateFilesOptionsDialog( const int mode, int &i_SkipNStartLines, bool &b_IncludeFilename, bool &b_SkipEmptyLines, bool &b_SkipCommentLines, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    ConcatenateFilesOptionsDialog dialog( this );

    dialog.SkipNStartLines_lineEdit->setText( QString( "%1" ).arg( i_SkipNStartLines ) );

    dialog.IncludeFilename_checkBox->setChecked( b_IncludeFilename );
    dialog.SkipEmptyLines_checkBox->setChecked( b_SkipEmptyLines );
    dialog.SkipCommentLines_checkBox->setChecked( b_SkipCommentLines );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    switch ( mode )
    {
    case _CONCATENATEBYCOLUMNS:
        dialog.setWindowTitle( tr( "Concatenate files by columns dialog" ) );
        dialog.IncludeFilename_checkBox->setText( tr( "Include filenames in first line" ) );
        dialog.SkipEmptyLines_checkBox->setEnabled( false );
        dialog.SkipCommentLines_checkBox->setEnabled( false );
        break;

    case _CONCATENATEBYLINES:
        dialog.setWindowTitle( tr( "Concatenate files by lines dialog" ) );
        break;

    default:
        break;
    }

    dialog.OK_pushButton->setWhatsThis( tr( "Close dialog" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    dialog.SkipNStartLines_lineEdit->selectAll();

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
        i_SkipNStartLines  = (int) dialog.SkipNStartLines_lineEdit->text().toInt();

        b_IncludeFilename  = dialog.IncludeFilename_checkBox->isChecked();
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
