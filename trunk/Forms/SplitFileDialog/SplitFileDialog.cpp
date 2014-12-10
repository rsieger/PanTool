// ***********************************************************************************************
// *                                                                                             *
// * SplitFileDialog.cpp - Ask for a number lines or columns                                     *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-10-06                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "SplitFileDialog.h"

SplitFileDialog::SplitFileDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    NumberOfLines_lineEdit_1->setValidator( new QIntValidator( 1, 9999999, this ) );
    NumberOfHeaderLines_lineEdit_1->setValidator( new QIntValidator( 0, 9999999, this ) );

    NumberOfHeaderLines_lineEdit_2->setValidator( new QIntValidator( 0, 9999999, this ) );

    NumberOfColumns_lineEdit_3->setValidator( new QIntValidator( 1, 9999999, this ) );
    NumberOfMetadataColumns_lineEdit_3->setValidator( new QIntValidator( 0, 9999999, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( NumberOfLines_lineEdit_1, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( NumberOfHeaderLines_lineEdit_1, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

    connect( NumberOfHeaderLines_lineEdit_2, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

    connect( NumberOfColumns_lineEdit_3, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( NumberOfMetadataColumns_lineEdit_3, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void SplitFileDialog::clearAll()
{
    int mode = 0;

    if ( groupBox_SplitByLines->isHidden() == false )
        mode = _SPLITBYLINES;

    if ( groupBox_SplitByLargeFiles->isHidden() == false )
        mode = _SPLITLARGE;

    if ( groupBox_SplitByColumns->isHidden() == false )
        mode = _SPLITBYCOLUMNS;

    switch ( mode )
    {
    case _SPLITBYLINES:
        NumberOfLines_lineEdit_1->setText( QString( "%1" ).arg( 0 ) );
        NumberOfHeaderLines_lineEdit_1->setText( QString( "%1" ).arg( 0 ) );
        SkipCommentLines_checkBox->setChecked( false );
        SkipEmptyLines_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    case _SPLITLARGE:
        NumberOfHeaderLines_lineEdit_2->setText( QString( "%1" ).arg( 0 ) );
        SkipCommentLines_checkBox->setChecked( false );
        SkipEmptyLines_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    case _SPLITBYCOLUMNS:
        NumberOfColumns_lineEdit_3->setText( QString( "%1" ).arg( 0 ) );
        NumberOfMetadataColumns_lineEdit_3->setText( QString( "%1" ).arg( 0 ) );
        SkipCommentLines_checkBox->setChecked( false );
        SkipEmptyLines_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    default:
        break;
    }

    enableOKButton();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void SplitFileDialog::enableOKButton()
{
    bool b_OK = true;

    int  mode = 0;

    if ( groupBox_SplitByLines->isHidden() == false )
        mode = _SPLITBYLINES;

    if ( groupBox_SplitByLargeFiles->isHidden() == false )
        mode = _SPLITLARGE;

    if ( groupBox_SplitByColumns->isHidden() == false )
        mode = _SPLITBYCOLUMNS;

    switch ( mode )
    {
    case _SPLITBYLINES:
        if ( NumberOfLines_lineEdit_1->text().toInt() < 1 )
            b_OK = false;
        if ( NumberOfHeaderLines_lineEdit_1->text().toInt() < 0 )
            b_OK = false;
        break;

    case _SPLITLARGE:
        if ( NumberOfHeaderLines_lineEdit_2->text().toInt() < 0 )
            b_OK = false;
        break;

    case _SPLITBYCOLUMNS:
        if ( NumberOfColumns_lineEdit_3->text().toInt() < 1 )
            b_OK = false;
        if ( NumberOfMetadataColumns_lineEdit_3->text().toInt() < 0 )
            b_OK = false;
        break;

    default:
        break;
    }

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

int MainWindow::doSplitFileDialog( const int mode, int &i_NumberOfLines, int &i_NumberOfHeaderLines, int &i_NumberOfColumns, int &i_NumberOfMetadataColumns, bool &b_SkipEmptyLines, bool &b_SkipCommentLines, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    SplitFileDialog dialog( this );

    dialog.NumberOfLines_lineEdit_1->setText( QString( "%1" ).arg( i_NumberOfLines ) );
    dialog.NumberOfHeaderLines_lineEdit_1->setText( QString( "%1" ).arg( i_NumberOfHeaderLines ) );

    dialog.NumberOfHeaderLines_lineEdit_2->setText( QString( "%1" ).arg( i_NumberOfHeaderLines ) );

    dialog.NumberOfColumns_lineEdit_3->setText( QString( "%1" ).arg( i_NumberOfColumns ) );
    dialog.NumberOfMetadataColumns_lineEdit_3->setText( QString( "%1" ).arg( i_NumberOfMetadataColumns ) );

    dialog.SkipEmptyLines_checkBox->setChecked( b_SkipEmptyLines );
    dialog.SkipCommentLines_checkBox->setChecked( b_SkipCommentLines );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    if ( gsl_FilenameList.count() > 1 )
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );

    switch ( mode )
    {
    case _SPLITBYLINES:
        dialog.setWindowTitle( "Split file dialog" );
        dialog.groupBox_SplitByLargeFiles->hide();
        dialog.groupBox_SplitByColumns->hide();
        break;

    case _SPLITLARGE:
        dialog.setWindowTitle( "Split file dialog" );
        dialog.groupBox_SplitByLines->hide();
        dialog.groupBox_SplitByColumns->hide();
        break;

    case _SPLITBYCOLUMNS:
        dialog.setWindowTitle( "Split file dialog" );
        dialog.groupBox_SplitByLines->hide();
        dialog.groupBox_SplitByLargeFiles->hide();
        dialog.SkipEmptyLines_checkBox->setEnabled( false );
        dialog.SkipCommentLines_checkBox->setEnabled( false );
        break;

    default:
        break;
    }

    dialog.OK_pushButton->setWhatsThis( tr( "Close dialog" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    dialog.enableOKButton();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
         switch ( mode )
        {
        case _SPLITBYLINES:
            i_NumberOfLines           = dialog.NumberOfLines_lineEdit_1->text().toInt();
            i_NumberOfHeaderLines     = dialog.NumberOfHeaderLines_lineEdit_1->text().toInt();
            break;
        case _SPLITLARGE:
            i_NumberOfHeaderLines     = dialog.NumberOfHeaderLines_lineEdit_2->text().toInt();
            break;
        case _SPLITBYCOLUMNS:
            i_NumberOfColumns         = dialog.NumberOfColumns_lineEdit_3->text().toInt();
            i_NumberOfMetadataColumns = dialog.NumberOfMetadataColumns_lineEdit_3->text().toInt();
        break;
        }

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
