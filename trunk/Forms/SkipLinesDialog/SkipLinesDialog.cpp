// ***********************************************************************************************
// *                                                                                             *
// * SkipLinesDialog.cpp - Ask for skip empty lines and delete input file                        *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-07-21                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "SkipLinesDialog.h"

SkipLinesDialog::SkipLinesDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doSkipLinesDialog( const int mode, bool &b_SaveHeader, bool &b_SkipEmptyLines, bool &b_SkipCommentLines, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    SkipLinesDialog dialog( this );

    dialog.SaveHeader_checkBox->setChecked( b_SaveHeader );
    dialog.SkipEmptyLines_checkBox->setChecked( b_SkipEmptyLines );
    dialog.SkipCommentLines_checkBox->setChecked( b_SkipCommentLines );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    if ( gsl_FilenameList.count() > 1 )
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );

    switch ( mode )
    {
    case _DELETECOMMENTBLOCKS:
        dialog.setWindowTitle( tr( "Delete comment blocks dialog" ) );
        dialog.SaveHeader_checkBox->hide();
        break;

    case _DELETEDOUBLELINES:
        dialog.setWindowTitle( tr( "Delete double lines dialog" ) );
        dialog.SaveHeader_checkBox->hide();
        break;

    case _EXTRACT10MINLINES:
        dialog.setWindowTitle( tr( "Extract 10 min lines dialog" ) );
        dialog.SkipEmptyLines_checkBox->hide();
        dialog.SkipCommentLines_checkBox->hide();
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
        b_SaveHeader       = dialog.SaveHeader_checkBox->isChecked();
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
