// ***********************************************************************************************
// *                                                                                             *
// * ExtractLinesDialog.cpp - Ask for a number lines to be extracted                             *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-07-26                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "ExtractLinesDialog.h"

ExtractLinesDialog::ExtractLinesDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    StartLine_lineEdit->setValidator( new QIntValidator( 1, 9999999, this ) );
    Increment_lineEdit->setValidator( new QIntValidator( 1, 9999999, this ) );
    NumberOfLines_lineEdit->setValidator( new QIntValidator( 0, 9999999, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ExtractFirstLastLine_checkBox, SIGNAL(stateChanged( int )), this, SLOT(setEnabledLineEdit( int )));

    connect( StartLine_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( Increment_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( NumberOfLines_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void ExtractLinesDialog::setEnabledLineEdit( int state )
{
    if ( state == 0 )
    {
        Increment_lineEdit->setEnabled( true );
        NumberOfLines_lineEdit->setEnabled( true );
    }
    else
    {
        Increment_lineEdit->setEnabled( false );
        NumberOfLines_lineEdit->setEnabled( false );
    }

    StartLine_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void ExtractLinesDialog::clearAll()
{
    StartLine_lineEdit->setText( "1" );
    Increment_lineEdit->setText( "1" );
    NumberOfLines_lineEdit->setText( "1" );

    SkipEmptyLines_checkBox->setChecked( false );
    SkipCommentLines_checkBox->setChecked( false );
    DeleteInputFile_checkBox->setChecked( false );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void ExtractLinesDialog::enableOKButton()
{
    bool b_OK = true;

    if ( ( StartLine_lineEdit->text().isEmpty() == true ) || ( Increment_lineEdit->text().isEmpty() == true ) || ( NumberOfLines_lineEdit->text().isEmpty() == true ) )
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

int MainWindow::doExtractLinesDialog( int &i_StartLine, int &i_Increment, int &i_NumberOfLines, bool &b_ExtractFristLineLastLine, bool &b_SkipEmptyLines, bool &b_SkipCommentLines, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    ExtractLinesDialog dialog( this );

    dialog.StartLine_lineEdit->setText( QString( "%1" ).arg( i_StartLine ) );
    dialog.Increment_lineEdit->setText( QString( "%1" ).arg( i_Increment ) );
    dialog.NumberOfLines_lineEdit->setText( QString( "%1" ).arg( i_NumberOfLines ) );
    dialog.ExtractFirstLastLine_checkBox->setChecked( b_ExtractFristLineLastLine );
    dialog.SkipEmptyLines_checkBox->setChecked( b_SkipEmptyLines );
    dialog.SkipCommentLines_checkBox->setChecked( b_SkipCommentLines );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    if ( gsl_FilenameList.count() > 1 )
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );

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
        i_StartLine          = dialog.StartLine_lineEdit->text().toInt();
        i_Increment          = dialog.Increment_lineEdit->text().toInt();
        i_NumberOfLines      = dialog.NumberOfLines_lineEdit->text().toInt();

        if ( i_StartLine < 1 )     i_StartLine = 1;
        if ( i_Increment < 1 )     i_Increment = 1;
        if ( i_NumberOfLines < 0 ) i_NumberOfLines = 0;

        b_ExtractFristLineLastLine = dialog.ExtractFirstLastLine_checkBox->isChecked();
        b_SkipEmptyLines           = dialog.SkipEmptyLines_checkBox->isChecked();
        b_SkipCommentLines         = dialog.SkipCommentLines_checkBox->isChecked();
        b_DeleteInputFile          = dialog.DeleteInputFile_checkBox->isChecked();

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
