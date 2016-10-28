// ***********************************************************************************************
// *                                                                                             *
// * GoogleEarthOptionsDialog.cpp - Select the GoogleEarth program                               *
// *                                                                                             *
// * Dr. Rainer Sieger - 2010-08-18                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "GoogleEarthOptionsDialog.h"

GoogleEarthOptionsDialog::GoogleEarthOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowseGoogleEarthProgram_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
    connect(BrowseGoogleEarthFilename_pushButton, SIGNAL(clicked()), this, SLOT(browseOutputFilenameDialog()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doGoogleEarthOptionsDialog()
{
    int i_DialogResult = QDialog::Rejected;

    QFileInfo fi( gs_ge_FilenameGoogleEarth );
    QDir di( fi.absolutePath() );

    if ( di.exists() == false )
        gs_ge_FilenameGoogleEarth = getDocumentDir() + "/" + fi.baseName() + ".kml";

    GoogleEarthOptionsDialog dialog( this );

    dialog.GoogleEarthFilename_lineEdit->setText( QDir::toNativeSeparators( gs_ge_FilenameGoogleEarth ) );
    dialog.showEventLabel_checkBox->setChecked( gb_ge_displayEventLabel );
    dialog.showDescription_checkBox->setChecked( gb_ge_displayDescription );
    dialog.IconColor_comboBox->setCurrentIndex( gi_ge_IconColor );
    dialog.IconSymbol_comboBox->setCurrentIndex( gi_ge_IconSymbol );
    dialog.IconSize_spinBox->setValue( gi_ge_IconSize );
    dialog.TracklineColor_comboBox->setCurrentIndex( gi_ge_TracklineColor );
    dialog.TracklineWidth_spinBox->setValue( gi_ge_TracklineWidth );

    dialog.CodecInput_ComboBox->setCurrentIndex( gi_CodecInput );

    dialog.StartGoogleEarth_checkBox->setChecked( gb_ge_startGoogleEarth );
    dialog.GoogleEarthProgram_lineEdit->setText( QDir::toNativeSeparators( gs_ge_FilenameGoogleEarthProgram ) );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.BrowseGoogleEarthProgram_pushButton->setWhatsThis( "Browse through the Google Earth program file" );
    dialog.BrowseGoogleEarthFilename_pushButton->setWhatsThis( "Browse through the KML output file" );

#if defined(Q_OS_MAC)
    dialog.StartGoogleEarth_checkBox->setChecked( false );
    dialog.GoogleEarth_groupBox->hide();
#endif

    dialog.GoogleEarthFilename_lineEdit->selectAll();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gs_ge_FilenameGoogleEarthProgram = dialog.GoogleEarthProgram_lineEdit->text();
        gs_ge_FilenameGoogleEarth        = dialog.GoogleEarthFilename_lineEdit->text();
        gb_ge_displayEventLabel          = dialog.showEventLabel_checkBox->isChecked();
        gb_ge_displayDescription         = dialog.showDescription_checkBox->isChecked();
        gi_ge_IconColor                  = dialog.IconColor_comboBox->currentIndex();
        gi_ge_IconSymbol                 = dialog.IconSymbol_comboBox->currentIndex();
        gi_ge_IconSize                   = dialog.IconSize_spinBox->value();
        gi_ge_TracklineColor             = dialog.TracklineColor_comboBox->currentIndex();
        gi_ge_TracklineWidth             = dialog.TracklineWidth_spinBox->value();
        gb_ge_startGoogleEarth           = dialog.StartGoogleEarth_checkBox->isChecked();
        gi_CodecInput                    = dialog.CodecInput_ComboBox->currentIndex();

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

void GoogleEarthOptionsDialog::browseFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->GoogleEarthProgram_lineEdit->text();
    QString fileStart	= getProgramDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    if ( ( file.startsWith( "Browse" ) == false ) && ( file.isEmpty() == false ) )
        fileStart = file;

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the Google Earth program" ), fileStart, tr( "EXE file (*.exe)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the Google Earth program" ), fileStart, tr( "All (*)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getExistingDirectory( this, tr( "Select the Google Earth program" ), fileStart, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == true )
        fn = file;

    this->GoogleEarthProgram_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    GoogleEarthProgram_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void GoogleEarthOptionsDialog::browseOutputFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->GoogleEarthFilename_lineEdit->text();
    QString fileStart	= getDocumentDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    QFileInfo fi( file );
    QDir di( fi.absolutePath() );

    if ( ( file.isEmpty() == false ) && ( di.exists() == true ) )
        fileStart = file;

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.kml)" ), fileStart, tr( "KML file (*.kml)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.kml)" ), fileStart, tr( "KML file (*.kml)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.kml)" ), fileStart, tr( "KML file (*.kml)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == true )
        fn = file;

    this->GoogleEarthFilename_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    GoogleEarthFilename_lineEdit->setFocus();

    if ( fn.contains( " " ) == true )
    {
        QString s_Message = "The output file\n\n    " + QDir::toNativeSeparators( fn ) + "\n\n contains spaces. Google Earth cannot open files in directories with spaces.";
        QMessageBox::warning( this, tr( "Pan2Applic" ), s_Message );
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString GoogleEarthOptionsDialog::getProgramDir()
{
    #if defined(Q_OS_LINUX)
        return( QDir::homePath() );
    #endif

    #if defined(Q_OS_MAC)
        return( "/Applications" );
    #endif

    #if defined(Q_OS_WIN)
        return( QDir::rootPath() + QLatin1String( "Program Files (x86)") );
    #endif
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString GoogleEarthOptionsDialog::getDocumentDir()
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
