// ***********************************************************************************************
// *                                                                                             *
// * SearchDialog.cpp - Sets search string                                                       *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-07-25                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "SearchDialog.h"

SearchDialog::SearchDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    StartAtLine_lineEdit_2->setValidator( new QIntValidator( 1, 9999999, this ) );
    BreakAfterNLines_lineEdit_2->setValidator( new QIntValidator( 0, 9999999, this ) );

    StartAtLine_lineEdit_3->setValidator( new QIntValidator( 1, 9999999, this ) );
    BreakAfterNLines_lineEdit_3->setValidator( new QIntValidator( 0, 9999999, this ) );

    StartAtLine_lineEdit_4->setValidator( new QIntValidator( 1, 9999999, this ) );
    BreakAfterNLines_lineEdit_4->setValidator( new QIntValidator( 0, 9999999, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect( browseDatabaseName_pushButton, SIGNAL( clicked() ), this, SLOT( browseDatabaseNameDialog() ) );
    connect( OK_pushButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel_pushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    connect( SearchString_lineEdit_1, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

    connect( SearchString_lineEdit_2, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( StartAtLine_lineEdit_2, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( BreakAfterNLines_lineEdit_2, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

    connect( SearchString_lineEdit_3, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( ReplaceString_lineEdit_3, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( StartAtLine_lineEdit_3, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( BreakAfterNLines_lineEdit_3, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

    connect( StartAtLine_lineEdit_4, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( BreakAfterNLines_lineEdit_4, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( DatabaseName_lineEdit_4, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void SearchDialog::clearAll()
{
    int mode = 0;

    if ( windowTitle() == tr( "Extract matched columns" ) )
        mode = _EXTRACTCOLUMNS;

    if ( windowTitle() ==  tr( "Extract matched lines" ) )
        mode = _EXTRACTLINES;

    if ( windowTitle() ==  tr( "Delete matched columns" ) )
        mode = _DELETECOLUMNS;

    if ( windowTitle() ==  tr( "Delete matched lines" ) )
        mode = _DELETELINES;

    if ( windowTitle() ==  tr( "Search one string" ) )
        mode = _SEARCHONESTRING;

    if ( windowTitle() ==  tr( "Search and replace one string" ) )
        mode = _SEARCHANDREPLACEONESTRING;

    if ( windowTitle() ==  tr( "Search and replace many strings at once" ) )
        mode = _SEARCHANDREPLACEMANYSTRINGSATONCE;

    switch ( mode )
    {
    case _EXTRACTCOLUMNS:
        SearchString_lineEdit_1->setText( "" );
        SkipCommentLines_checkBox->setChecked( false );
        NoEmptyOutputFile_checkBox->setChecked( false );
        SaveNoMatch_checkBox->setChecked( false );
        SkipEmptyLines_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    case _EXTRACTLINES:
        SearchString_lineEdit_2->setText( "" );
        StartAtLine_lineEdit_2->setText( QString( "%1" ).arg( 1 ) );
        BreakAfterNLines_lineEdit_2->setText( QString( "%1" ).arg( 0 ) );
        SaveFirstLine_checkBox->setChecked( false );
        SkipCommentLines_checkBox->setChecked( false );
        NoEmptyOutputFile_checkBox->setChecked( false );
        SaveNoMatch_checkBox->setChecked( false );
        SkipEmptyLines_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    case _DELETECOLUMNS:
        SearchString_lineEdit_1->setText( "" );
        SaveFirstLine_checkBox->setChecked( false );
        SaveNoMatch_checkBox->setChecked( false );
        SkipCommentLines_checkBox->setChecked( false );
        SkipEmptyLines_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    case _DELETELINES:
        SearchString_lineEdit_2->setText( "" );
        StartAtLine_lineEdit_2->setText( QString( "%1" ).arg( 1 ) );
        BreakAfterNLines_lineEdit_2->setText( QString( "%1" ).arg( 0 ) );
        SaveFirstLine_checkBox->setChecked( false );
        SkipCommentLines_checkBox->setChecked( false );
        SkipEmptyLines_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    case _SEARCHONESTRING:
        SearchString_lineEdit_2->setText( "" );
        StartAtLine_lineEdit_2->setText( QString( "%1" ).arg( 1 ) );
        BreakAfterNLines_lineEdit_2->setText( QString( "%1" ).arg( 0 ) );
        SkipCommentLines_checkBox->setChecked( false );
        NoEmptyOutputFile_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    case _SEARCHANDREPLACEONESTRING:
        SearchString_lineEdit_3->setText( "" );
        ReplaceString_lineEdit_3->setText( "" );
        StartAtLine_lineEdit_3->setText( QString( "%1" ).arg( 1 ) );
        BreakAfterNLines_lineEdit_3->setText( QString( "%1" ).arg( 0 ) );
        SkipCommentLines_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    case _SEARCHANDREPLACEMANYSTRINGSATONCE:
        DatabaseName_lineEdit_4->setText( tr( "Choose the \"search and replace database\" with \"Browse ...\"" ) );
        DatabaseName_lineEdit_4->setEnabled( false );
        StartAtLine_lineEdit_4->setText( QString( "%1" ).arg( 1 ) );
        BreakAfterNLines_lineEdit_4->setText( QString( "%1" ).arg( 0 ) );
        SkipCommentLines_checkBox->setChecked( false );
        SkipEmptyLines_checkBox->setChecked( false );
        DeleteInputFile_checkBox->setChecked( false );
        break;

    default:
        break;
    }

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void SearchDialog::browseDatabaseNameDialog()
{
    QString	fn   = "";
    QString file = DatabaseName_lineEdit_4->text();

    QFileInfo fi( file );

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        file = getDocumentDir();

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getOpenFileName( this, tr( "Select a search and replace database (*.rdb)" ), file, tr( "RDB file (*.rdb)\nText (*.txt *.csv)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getOpenFileName( this, tr( "Select a search and replace database (*.rdb)" ), file, tr( "RDB file (*.rdb)\nText (*.txt *.csv)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getOpenFileName( this, tr( "Select a search and replace database (*.rdb)" ), file, tr( "RDB file (*.rdb)\nText (*.txt *.csv)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == false )
        fi.setFile( fn );
    else
        fn = file;

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        DatabaseName_lineEdit_4->clear();
    else
        DatabaseName_lineEdit_4->setText( QDir::toNativeSeparators( fn ) );

    DatabaseName_lineEdit_4->setFocus();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString SearchDialog::getHomeDir()
{
    return( QDir::homePath() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString SearchDialog::getDocumentDir()
{
    #if defined(Q_OS_LINUX)
        return( QDir::homePath() );
    #endif

    #if defined(Q_OS_MAC)
        return( QDir::homePath() );
    #endif

    #if defined(Q_OS_WIN)
        return( QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation ) );
    #endif
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void SearchDialog::enableOKButton()
{
    bool b_OK = true;

    int  mode = 0;

    QFileInfo fi( DatabaseName_lineEdit_4->text() );

    if ( windowTitle() == tr( "Extract matched columns" ) )
        mode = _EXTRACTCOLUMNS;

    if ( windowTitle() ==  tr( "Extract matched lines" ) )
        mode = _EXTRACTLINES;

    if ( windowTitle() ==  tr( "Delete matched columns" ) )
        mode = _DELETECOLUMNS;

    if ( windowTitle() ==  tr( "Delete matched lines" ) )
        mode = _DELETELINES;

    if ( windowTitle() ==  tr( "Search one string" ) )
        mode = _SEARCHONESTRING;

    if ( windowTitle() ==  tr( "Search and replace one string" ) )
        mode = _SEARCHANDREPLACEONESTRING;

    if ( windowTitle() ==  tr( "Search and replace many strings at once" ) )
        mode = _SEARCHANDREPLACEMANYSTRINGSATONCE;

    switch ( mode )
    {
    case _EXTRACTCOLUMNS:
        if ( SearchString_lineEdit_1->text().isEmpty() == true )
            b_OK = false;
        break;

    case _EXTRACTLINES:
        if ( SearchString_lineEdit_2->text().isEmpty() == true )
            b_OK = false;
        break;

    case _DELETECOLUMNS:
        if ( SearchString_lineEdit_1->text().isEmpty() == true )
            b_OK = false;
        break;

    case _DELETELINES:
        if ( SearchString_lineEdit_2->text().isEmpty() == true )
            b_OK = false;
        break;

    case _SEARCHONESTRING:
        if ( SearchString_lineEdit_2->text().isEmpty() == true )
            b_OK = false;
        break;

    case _SEARCHANDREPLACEONESTRING:
        if ( SearchString_lineEdit_3->text().isEmpty() == true )
            b_OK = false;
        break;

    case _SEARCHANDREPLACEMANYSTRINGSATONCE:
        if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
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

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void SearchDialog::dragEnterEvent( QDragEnterEvent *event )
{
    if ( event->mimeData()->hasFormat( "text/uri-list" ) )
        event->acceptProposedAction();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void SearchDialog::dropEvent( QDropEvent *event )
{
    QList<QUrl> urls = event->mimeData()->urls();

    if ( urls.isEmpty() == true )
        return;

    QString s_fileName = urls.first().toLocalFile();

    if ( s_fileName.isEmpty() == true )
        return;

    QFileInfo fi( s_fileName );

    if ( fi.isFile() == true )
    {
        if ( fi.suffix().toLower() == "rdb" )
            DatabaseName_lineEdit_4->setText( QDir::toNativeSeparators( s_fileName ) );
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doSearchDialog( const int mode, QString &s_SearchString, QString &s_ReplaceString, QString &s_DatabaseName, int &i_StartLine, int &i_BreakAfterNLines, int &i_SearchAndReplaceMode, bool &b_SaveFirstLine, bool &b_SaveNoMatch, bool &b_SkipEmptyLines, bool &b_SkipCommentLines, bool &b_DeleteEmptyOutputFile, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    QFileInfo fi( s_DatabaseName );

    if ( mode == _SEARCHANDREPLACEMANYSTRINGSATONCE )
    {
        if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
            s_DatabaseName = tr( "Choose the \"search and replace database\" with \"Browse ...\"" );
    }
    else
    {
        s_DatabaseName = "not used in this mode";
    }

// ***********************************************************************************************************************

    SearchDialog dialog( this );

    switch ( mode )
    {
    case _EXTRACTCOLUMNS:
        dialog.setWindowTitle( tr( "Extract matched columns" ) );
        dialog.groupBox_SearchAndReplace_2->hide();
        dialog.groupBox_SearchAndReplace_3->hide();
        dialog.groupBox_SearchAndReplace_4->hide();
        dialog.groupBox_SearchAndReplaceMode->hide();
        dialog.SearchString_lineEdit_1->setText( s_SearchString );
        dialog.SaveFirstLine_checkBox->setEnabled( false );
        break;

    case _EXTRACTLINES:
        dialog.setWindowTitle( tr( "Extract matched lines" ) );
        dialog.groupBox_SearchAndReplace_1->hide();
        dialog.groupBox_SearchAndReplace_3->hide();
        dialog.groupBox_SearchAndReplace_4->hide();
        dialog.groupBox_SearchAndReplaceMode->hide();
        dialog.SearchString_lineEdit_2->setText( s_SearchString );
        dialog.StartAtLine_lineEdit_2->setText( QString( "%1" ).arg( i_StartLine ) );
        dialog.BreakAfterNLines_lineEdit_2->setText( QString( "%1" ).arg( i_BreakAfterNLines ) );
        break;

    case _DELETECOLUMNS:
        dialog.setWindowTitle( tr( "Delete matched columns" ) );
        dialog.groupBox_SearchAndReplace_2->hide();
        dialog.groupBox_SearchAndReplace_3->hide();
        dialog.groupBox_SearchAndReplace_4->hide();
        dialog.groupBox_SearchAndReplaceMode->hide();
        dialog.SearchString_lineEdit_1->setText( s_SearchString );
        dialog.SaveFirstLine_checkBox->setEnabled( false );
        dialog.NoEmptyOutputFile_checkBox->setEnabled( false );
        dialog.SaveNoMatch_checkBox->setEnabled( false );
        break;

    case _DELETELINES:
        dialog.setWindowTitle( tr( "Delete matched lines" ) );
        dialog.groupBox_SearchAndReplace_1->hide();
        dialog.groupBox_SearchAndReplace_3->hide();
        dialog.groupBox_SearchAndReplace_4->hide();
        dialog.groupBox_SearchAndReplaceMode->hide();
        dialog.SearchString_lineEdit_2->setText( s_SearchString );
        dialog.StartAtLine_lineEdit_2->setText( QString( "%1" ).arg( i_StartLine ) );
        dialog.BreakAfterNLines_lineEdit_2->setText( QString( "%1" ).arg( i_BreakAfterNLines ) );
        dialog.NoEmptyOutputFile_checkBox->setEnabled( false );
        dialog.SaveNoMatch_checkBox->setEnabled( false );
        break;

    case _SEARCHONESTRING:
        dialog.setWindowTitle( tr( "Search one string" ) );
        dialog.groupBox_SearchAndReplace_1->hide();
        dialog.groupBox_SearchAndReplace_3->hide();
        dialog.groupBox_SearchAndReplace_4->hide();
        dialog.groupBox_SearchAndReplaceMode->hide();
        dialog.SearchString_lineEdit_2->setText( s_SearchString );
        dialog.StartAtLine_lineEdit_2->setText( QString( "%1" ).arg( i_StartLine ) );
        dialog.BreakAfterNLines_lineEdit_2->setText( QString( "%1" ).arg( i_BreakAfterNLines ) );
        dialog.SkipEmptyLines_checkBox->setEnabled( false );
        dialog.SaveFirstLine_checkBox->setEnabled( false );
        dialog.SaveNoMatch_checkBox->setEnabled( false );
        break;

    case _SEARCHANDREPLACEONESTRING:
        dialog.setWindowTitle( tr( "Search and replace one string" ) );
        dialog.groupBox_SearchAndReplace_1->hide();
        dialog.groupBox_SearchAndReplace_2->hide();
        dialog.groupBox_SearchAndReplace_4->hide();
        dialog.groupBox_SearchAndReplaceMode->hide();
        dialog.SearchString_lineEdit_3->setText( s_SearchString );
        dialog.ReplaceString_lineEdit_3->setText( s_ReplaceString );
        dialog.StartAtLine_lineEdit_3->setText( QString( "%1" ).arg( i_StartLine ) );
        dialog.BreakAfterNLines_lineEdit_3->setText( QString( "%1" ).arg( i_BreakAfterNLines ) );
        dialog.SkipEmptyLines_checkBox->setEnabled( false );
        dialog.SaveFirstLine_checkBox->setEnabled( false );
        dialog.NoEmptyOutputFile_checkBox->setEnabled( false );
        dialog.SaveNoMatch_checkBox->setEnabled( false );
        break;

    case _SEARCHANDREPLACEMANYSTRINGSATONCE:
        dialog.setWindowTitle( tr( "Search and replace many strings at once" ) );
        dialog.groupBox_SearchAndReplace_1->hide();
        dialog.groupBox_SearchAndReplace_2->hide();
        dialog.groupBox_SearchAndReplace_3->hide();
        dialog.DatabaseName_lineEdit_4->setText( s_DatabaseName );
        dialog.DatabaseName_lineEdit_4->setEnabled( false );
        dialog.StartAtLine_lineEdit_4->setText( QString( "%1" ).arg( i_StartLine ) );
        dialog.BreakAfterNLines_lineEdit_4->setText( QString( "%1" ).arg( i_BreakAfterNLines ) );
        dialog.SkipEmptyLines_checkBox->setEnabled( false );
        dialog.SaveFirstLine_checkBox->setEnabled( false );
        dialog.NoEmptyOutputFile_checkBox->setEnabled( false );
        dialog.SaveNoMatch_checkBox->setEnabled( false );
        if ( i_SearchAndReplaceMode == _LINE ) dialog.Line_radioButton->setChecked( true );
        if ( i_SearchAndReplaceMode == _CELL ) dialog.Cell_radioButton->setChecked( true );
        break;

    default:
        dialog.groupBox_SearchAndReplaceMode->hide();
        break;
    }

// Options
    dialog.SaveFirstLine_checkBox->setChecked( b_SaveFirstLine );
    dialog.SkipEmptyLines_checkBox->setChecked( b_SkipEmptyLines );
    dialog.SkipCommentLines_checkBox->setChecked( b_SkipCommentLines );
    dialog.NoEmptyOutputFile_checkBox->setChecked( b_DeleteEmptyOutputFile );
    dialog.SaveNoMatch_checkBox->setChecked( b_SaveNoMatch );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    if ( gsl_FilenameList.count() > 1 )
    {
        dialog.NoEmptyOutputFile_checkBox->setText( tr( "Don't create empty output files" ) );
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );
    }

    dialog.SearchString_lineEdit_1->selectAll();

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
        case _EXTRACTCOLUMNS:
            s_SearchString      = dialog.SearchString_lineEdit_1->text();
            break;

        case _EXTRACTLINES:
            s_SearchString      = dialog.SearchString_lineEdit_2->text();
            i_StartLine         = dialog.StartAtLine_lineEdit_2->text().toInt();
            i_BreakAfterNLines  = dialog.BreakAfterNLines_lineEdit_2->text().toInt();
            break;

        case _DELETECOLUMNS:
            s_SearchString      = dialog.SearchString_lineEdit_1->text();
            break;

        case _DELETELINES:
            s_SearchString      = dialog.SearchString_lineEdit_2->text();
            i_StartLine         = dialog.StartAtLine_lineEdit_2->text().toInt();
            i_BreakAfterNLines  = dialog.BreakAfterNLines_lineEdit_2->text().toInt();
            break;

        case _SEARCHONESTRING:
            s_SearchString      = dialog.SearchString_lineEdit_2->text();
            i_StartLine         = dialog.StartAtLine_lineEdit_2->text().toInt();
            i_BreakAfterNLines  = dialog.BreakAfterNLines_lineEdit_2->text().toInt();
            break;

        case _SEARCHANDREPLACEONESTRING:
            s_SearchString      = dialog.SearchString_lineEdit_3->text();
            s_ReplaceString     = dialog.ReplaceString_lineEdit_3->text();
            i_StartLine         = dialog.StartAtLine_lineEdit_3->text().toInt();
            i_BreakAfterNLines  = dialog.BreakAfterNLines_lineEdit_3->text().toInt();
            break;

        case _SEARCHANDREPLACEMANYSTRINGSATONCE:
            s_DatabaseName      = dialog.DatabaseName_lineEdit_4->text();
            i_StartLine         = dialog.StartAtLine_lineEdit_4->text().toInt();
            i_BreakAfterNLines  = dialog.BreakAfterNLines_lineEdit_4->text().toInt();

            if ( dialog.Line_radioButton->isChecked() == true )
                i_SearchAndReplaceMode = _LINE;

            if ( dialog.Cell_radioButton->isChecked() == true )
                i_SearchAndReplaceMode = _CELL;
            break;

        default:
            break;
        }

        if ( i_StartLine < 1 )
            i_StartLine = 1;

        if ( i_BreakAfterNLines < 0 )
            i_BreakAfterNLines = 0;

        b_SaveFirstLine         = dialog.SaveFirstLine_checkBox->isChecked();
        b_SkipEmptyLines        = dialog.SkipEmptyLines_checkBox->isChecked();
        b_SkipCommentLines      = dialog.SkipCommentLines_checkBox->isChecked();
        b_DeleteEmptyOutputFile = dialog.NoEmptyOutputFile_checkBox->isChecked();
        b_SaveNoMatch           = dialog.SaveNoMatch_checkBox->isChecked();
        b_DeleteInputFile       = dialog.DeleteInputFile_checkBox->isChecked();

        i_DialogResult          = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
