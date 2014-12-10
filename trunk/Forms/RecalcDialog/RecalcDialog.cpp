// ***********************************************************************************************
// *                                                                                             *
// * RecalcDialog.cpp - Recalculation of column                                                  *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-08-20                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "RecalcDialog.h"

RecalcDialog::RecalcDialog( QWidget *parent ) : QDialog( parent )
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

void RecalcDialog::clearAll()
{
    ColumnList_lineEdit->setText( "" );

    DeleteInputFile_checkBox->setChecked( false );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void RecalcDialog::enableOKButton()
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

int MainWindow::doRecalcDialog( QString &s_ColumnList, float &f_MulX, float &f_AddX, int &i_NumOfDigits, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    RecalcDialog dialog( this );

    dialog.ColumnList_lineEdit->setText( s_ColumnList );
    dialog.MulX_lineEdit->setText( QString( "%1" ).arg( f_MulX ) );
    dialog.AddX_lineEdit->setText( QString( "%1" ).arg( f_AddX ) );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );
    dialog.NumOfDigits_spinBox->setValue( i_NumOfDigits );
    dialog.NumOfDigits_spinBox->setRange( 0, 10 );

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
        f_MulX             = dialog.MulX_lineEdit->text().toFloat();
        f_AddX             = dialog.AddX_lineEdit->text().toFloat();
        i_NumOfDigits      = dialog.NumOfDigits_spinBox->value();
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
