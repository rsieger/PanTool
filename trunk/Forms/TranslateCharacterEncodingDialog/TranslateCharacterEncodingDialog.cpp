// ***********************************************************************************************
// *                                                                                             *
// * TranslateCharacterEncodingDialog.cpp - sets translate character encoding options            *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-10-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "TranslateCharacterEncodingDialog.h"

TranslateCharacterEncodingDialog::TranslateCharacterEncodingDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::doTranslateCharacterEncodingDialog( int &i_CodecInput, int &i_CodecOutput, int &i_EOL, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    TranslateCharacterEncodingDialog dialog( this );

    dialog.CodecInput_ComboBox->setCurrentIndex( i_CodecInput );
    dialog.CodecOutput_ComboBox->setCurrentIndex( i_CodecOutput );
    dialog.EOL_ComboBox->setCurrentIndex( i_EOL );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    dialog.OK_pushButton->setWhatsThis( tr( "Close dialog" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        i_CodecInput      = dialog.CodecInput_ComboBox->currentIndex();
        i_CodecOutput     = dialog.CodecOutput_ComboBox->currentIndex();
        i_EOL             = dialog.EOL_ComboBox->currentIndex();
        b_DeleteInputFile = dialog.DeleteInputFile_checkBox->isChecked();

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
