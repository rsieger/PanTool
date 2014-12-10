// ***********************************************************************************************
// *                                                                                             *
// * CharactersAtPositionDialog.cpp - Sets position list                                         *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-08-11                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "CharactersAtPositionDialog.h"

CharactersAtPositionDialog::CharactersAtPositionDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    QRegExp regExp( "[0-9,]{1024}" );
    PositionList_lineEdit->setValidator( new QRegExpValidator( regExp, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( PositionList_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( Text_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void CharactersAtPositionDialog::clearAll()
{
    PositionList_lineEdit->setText( "" );
    Text_lineEdit->setText( "" );

    SkipEmptyLines_checkBox->setChecked( false );
    SkipCommentLines_checkBox->setChecked( false );
    DeleteInputFile_checkBox->setChecked( false );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void CharactersAtPositionDialog::enableOKButton()
{
    bool b_OK = true;

    if ( ( PositionList_lineEdit->text().isEmpty() == true ) || ( Text_lineEdit->text().isEmpty() == true ) )
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

int MainWindow::doCharactersAtPositionDialog( const int mode, QString &s_PositionList, QString &s_Text, bool &b_SkipEmptyLines, bool &b_SkipCommentLines, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    CharactersAtPositionDialog dialog( this );

    switch ( mode )
    {
    case _INSERTCHARS:
        dialog.setWindowTitle( tr( "Insert characters dialog" ) );
        dialog.Text_label->setText( tr( "Insert text:" ) );
        break;
    case _REPLACECHARS:
        dialog.setWindowTitle( tr( "Replace characters dialog" ) );
        dialog.Text_label->setText( tr( "Replace text:" ) );
        break;
    default:
        break;
    }

    dialog.PositionList_lineEdit->setText( s_PositionList );
    dialog.Text_lineEdit->setText( s_Text );
    dialog.SkipEmptyLines_checkBox->setChecked( b_SkipEmptyLines );
    dialog.SkipCommentLines_checkBox->setChecked( b_SkipCommentLines );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    dialog.OK_pushButton->setWhatsThis( tr( "Close dialog" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    if ( gsl_FilenameList.count() > 1 )
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );

    dialog.PositionList_lineEdit->selectAll();

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
        s_PositionList     = dialog.PositionList_lineEdit->text().toLower();
        s_Text             = dialog.Text_lineEdit->text();
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
