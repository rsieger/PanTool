// ***********************************************************************************************
// *                                                                                             *
// * RScripOptionsDialog.cpp - Select the RScript program                                        *
// *                                                                                             *
// * Dr. Rainer Sieger - 2017-09-11                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "RScriptOptionsDialog.h"

RScriptOptionsDialog::RScriptOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowseRScriptProgram_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doRScriptOptionsDialog()
{
    int i_DialogResult = QDialog::Rejected;

    RScriptOptionsDialog dialog( this );

    dialog.RScriptProgram_lineEdit->setText( QDir::toNativeSeparators( gs_rs_FilenameRScriptProgram ) );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.BrowseRScriptProgram_pushButton->setWhatsThis( "Browse through the Rscript program file" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gs_rs_FilenameRScriptProgram = dialog.RScriptProgram_lineEdit->text();

        i_DialogResult = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        i_DialogResult = QDialog::Rejected;
        break;

    default:
        i_DialogResult = QDialog::Rejected;
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************
// 2017-09-11

void RScriptOptionsDialog::browseFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->RScriptProgram_lineEdit->text();
    QString fileStart	= getProgramDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    QFileInfo fi( file );

    if ( fi.exists() == true )
        fileStart = file;

    OK_pushButton->setEnabled( true );

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the Rscript program" ), fileStart, tr( "EXE file (*.exe)" ), 0, QFileDialog::DontUseNativeDialog );
        if ( fn.toLower().endsWith( "rscript.exe") == false )
            OK_pushButton->setEnabled( false );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the Rscript program" ), fileStart, tr( "All (*)" ), 0, QFileDialog::DontUseNativeDialog );
        if ( fn.endsWith( "/Rscript") == false )
            OK_pushButton->setEnabled( false );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the Rscript program" ), fileStart, tr( "All (*)" ), 0, QFileDialog::DontUseNativeDialog );
        if ( fn.endsWith( "/Rscript") == false )
            OK_pushButton->setEnabled( false );
    #endif

    if ( fn.isEmpty() == true )
        fn = file;

    this->RScriptProgram_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    RScriptProgram_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString RScriptOptionsDialog::getProgramDir()
{
    #if defined(Q_OS_LINUX)
        return( "/usr/bin" );
    #endif

    #if defined(Q_OS_MAC)
        return( "/usr/local/bin" );
    #endif

    #if defined(Q_OS_WIN)
        return( QDir::rootPath() + QLatin1String( "Program Files/R" ) );
    #endif
}
