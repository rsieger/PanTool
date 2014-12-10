// ***********************************************************************************************
// *                                                                                             *
// * AddDialog.cpp - Ask for a text to insert at line number                                     *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-10-10                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "AddDialog.h"

AddDialog::AddDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    LineNo_lineEdit_1->setValidator( new QIntValidator( 1, 9999999, this ) );
    LineNo_lineEdit_2->setValidator( new QIntValidator( 1, 9999999, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( LineNo_lineEdit_1, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( HeaderText_lineEdit_1, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

    connect( LineNo_lineEdit_2, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( TextBlock_plainTextEdit_2, SIGNAL( textChanged() ), this, SLOT( enableOKButton() ) );

    connect( HeaderText_lineEdit_3, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( ColumnText_lineEdit_3, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

    connect( AddFilename_checkBox, SIGNAL( toggled(bool) ), this, SLOT( enableOKButton() ) );
    connect( AddFullPath_checkBox, SIGNAL( toggled(bool) ), this, SLOT( enableOKButton() ) );
    connect( AddOrdinalNumber_checkBox, SIGNAL( toggled(bool) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void AddDialog::clearAll()
{
    if ( ( groupBox_AddBlock->isHidden() == true ) && ( groupBox_AddColumn->isHidden() == true ) )  // add line
    {
        LineNo_lineEdit_1->setText( "" );
        HeaderText_lineEdit_1->setText( "" );
    }

    if ( ( groupBox_AddLine->isHidden() == true ) && ( groupBox_AddColumn->isHidden() == true ) )   // add block
    {
        LineNo_lineEdit_2->setText( "" );
        TextBlock_plainTextEdit_2->setPlainText( "" );
    }

    if ( ( groupBox_AddLine->isHidden() == true ) && ( groupBox_AddBlock->isHidden() == true ) )    // add column
    {
        HeaderText_lineEdit_3->setText( "" );
        ColumnText_lineEdit_3->setText( "" );
    }

    AddFilename_checkBox->setChecked( false );
    AddFullPath_checkBox->setChecked( false );
    AddOrdinalNumber_checkBox->setChecked( false );

    SkipEmptyLines_checkBox->setChecked( false );
    SkipCommentLines_checkBox->setChecked( false );
    DeleteInputFile_checkBox->setChecked( false );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void AddDialog::enableOKButton()
{
    QList<int>  testList;

    bool b_OK = false;

    testList.append( LineNo_lineEdit_1->text().toInt() );                           // 0
    testList.append( (int) HeaderText_lineEdit_1->text().isEmpty() );               // 1

    testList.append( LineNo_lineEdit_2->text().toInt() );                           // 2
    testList.append( (int) TextBlock_plainTextEdit_2->toPlainText().isEmpty() );    // 3

    testList.append( (int) HeaderText_lineEdit_3->text().isEmpty() );               // 4

    if ( testList.at( 0 ) < 1 )  LineNo_lineEdit_1->setText( "" );
    if ( testList.at( 2 ) < 1 )  LineNo_lineEdit_2->setText( "" );

    if ( ( groupBox_AddBlock->isHidden() == true ) && ( groupBox_AddColumn->isHidden() == true ) )  // add line
        if ( ( testList.at( 0 ) > 0 ) && ( testList.at( 1 ) < 1 ) ) b_OK = true;

    if ( ( groupBox_AddLine->isHidden() == true ) && ( groupBox_AddColumn->isHidden() == true ) )   // add block
        if ( ( testList.at( 2 ) > 0 ) && ( testList.at( 3 ) < 1 ) )  b_OK = true;

    if ( ( groupBox_AddLine->isHidden() == true ) && ( groupBox_AddBlock->isHidden() == true ) )    // add column
        if ( ( testList.at( 4 ) < 1 ) || ( AddFilename_checkBox->isChecked() == true ) || ( AddFullPath_checkBox->isChecked() == true ) || ( AddOrdinalNumber_checkBox->isChecked() == true ) ) b_OK = true;

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

int MainWindow::doAddDialog( const int mode, QString &s_HeaderText, QString &s_ColumnText, int &i_LineNo, bool &b_AddFilename, bool &b_AddFullPath, bool &b_AddOrdinalNumber, bool &b_SkipEmptyLines, bool &b_SkipCommentLines, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    AddDialog dialog( this );

    switch ( mode )
    {
    case _ADDLINE:
        dialog.HeaderText_lineEdit_1->setText( s_HeaderText );
        dialog.LineNo_lineEdit_1->setText( QString( "%1" ).arg( i_LineNo ) );
        dialog.groupBox_AddBlock->hide();
        dialog.groupBox_AddColumn->hide();
        break;

    case _ADDBLOCK:
        s_HeaderText.replace( "^n", "\n" );
        s_HeaderText.replace( "^t", "\t" );

        dialog.TextBlock_plainTextEdit_2->setPlainText( s_HeaderText );
        dialog.LineNo_lineEdit_2->setText( QString( "%1" ).arg( i_LineNo ) );
        dialog.groupBox_AddLine->hide();
        dialog.groupBox_AddColumn->hide();
        break;

    case _ADDCOLUMN:
        dialog.HeaderText_lineEdit_3->setText( s_HeaderText );
        dialog.ColumnText_lineEdit_3->setText( s_ColumnText );
        dialog.groupBox_AddLine->hide();
        dialog.groupBox_AddBlock->hide();
        break;

    default:
        break;
    }

    dialog.AddFilename_checkBox->setChecked( b_AddFilename );
    dialog.AddFullPath_checkBox->setChecked( b_AddFullPath );
    dialog.AddOrdinalNumber_checkBox->setChecked( b_AddOrdinalNumber );
    dialog.SkipEmptyLines_checkBox->setChecked( b_SkipEmptyLines );
    dialog.SkipCommentLines_checkBox->setChecked( b_SkipCommentLines );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    dialog.OK_pushButton->setWhatsThis( tr( "Close dialog" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    if ( gsl_FilenameList.count() > 1 )
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );

    switch ( mode )
    {
    case _ADDCOLUMN:
        dialog.setWindowTitle( tr( "Add column" ) );
        break;

    case _ADDLINE:
        dialog.setWindowTitle( tr( "Add line" ) );
        break;

    case _ADDBLOCK:
        dialog.setWindowTitle( tr( "Add text block" ) );
        break;

    default:
        break;
    }

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
        switch ( mode )
        {
        case _ADDLINE:
            s_HeaderText = dialog.HeaderText_lineEdit_1->text();
            i_LineNo     = dialog.LineNo_lineEdit_1->text().toInt();

            s_HeaderText.replace( "\t", "^t" );
            s_HeaderText.replace( "\n", "^n" );
            break;

        case _ADDBLOCK:
            s_HeaderText = dialog.TextBlock_plainTextEdit_2->toPlainText();
            i_LineNo     = dialog.LineNo_lineEdit_2->text().toInt();

            s_HeaderText.replace( "\t", "^t" );
            s_HeaderText.replace( "\n", "^n" );
            break;

        case _ADDCOLUMN:
            s_HeaderText = dialog.HeaderText_lineEdit_3->text();
            s_ColumnText = dialog.ColumnText_lineEdit_3->text();

            s_HeaderText.replace( "\t", "^t" );
            s_ColumnText.replace( "\t", "^t" );
            break;

        default:
            break;
        }

        b_AddFilename      = dialog.AddFilename_checkBox->isChecked();
        b_AddFullPath      = dialog.AddFullPath_checkBox->isChecked();
        b_AddOrdinalNumber = dialog.AddOrdinalNumber_checkBox->isChecked();
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
