// ***********************************************************************************************
// *                                                                                             *
// * FourColumnsDialog.cpp - Ask for number of geocodes and for skip empty lines and             *
//*                          delete input file                                                   *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-02-03                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "FourColumnsDialog.h"

FourColumnsDialog::FourColumnsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doFourColumnsDialog( const int mode, int &i_NumOfGeocodes, int &i_FieldDelimiter, QString &s_MissingValue, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    FourColumnsDialog dialog( this );

    if ( i_NumOfGeocodes < 2 )
        dialog.one_radioButton->setChecked( true );
    else
        dialog.two_radioButton->setChecked( true );

    switch ( i_FieldDelimiter )
    {
        case _COMMA_:
            dialog.comma_radioButton->setChecked( true );
            break;
        case _SEMICOLON_:
            dialog.semicolon_radioButton->setChecked( true );
            break;
        case _SPACE_:
            dialog.space_radioButton->setChecked( true );
            break;
        default:
            dialog.tabstop_radioButton->setChecked( true );
            break;
    }

    dialog.missingValue_lineEdit->setText( s_MissingValue );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    if ( gsl_FilenameList.count() > 1 )
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );

    switch ( mode )
    {
    case _TABLECOLUMNS:
        dialog.setWindowTitle( tr( "Table to columns dialog" ) );
        dialog.FieldOptions_groupBox->hide();
        break;

    case _COLUMNSTABLE:
        dialog.setWindowTitle( tr( "Columns to table dialog" ) );
        dialog.NumOfGeocodes_groupBox->hide();
        break;

    default:
        break;
    }

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        switch ( mode )
        {
        case _TABLECOLUMNS:
            if ( dialog.one_radioButton->isChecked() == true )
                i_NumOfGeocodes = 1;
            else
                i_NumOfGeocodes = 2;
            break;

        case _COLUMNSTABLE:
            s_MissingValue = dialog.missingValue_lineEdit->text();

            if ( dialog.comma_radioButton->isChecked() == true )
                i_FieldDelimiter = _COMMA_;

            if ( dialog.semicolon_radioButton->isChecked() == true )
                i_FieldDelimiter = _SEMICOLON_;

            if ( dialog.space_radioButton->isChecked() == true )
                i_FieldDelimiter = _SPACE_;

            if ( dialog.tabstop_radioButton->isChecked() == true )
                i_FieldDelimiter = _TABSTOP_;
            break;
        }

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
