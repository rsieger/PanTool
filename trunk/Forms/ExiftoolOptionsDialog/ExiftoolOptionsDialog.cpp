// ***********************************************************************************************
// *                                                                                             *
// * ExifToolOptionsDialog.cpp - Set the options for the ExifTool                                *
// *                                                                                             *
// * Dr. Rainer Sieger - 2016-08-18                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "ExiftoolOptionsDialog.h"

ExifToolOptionsDialog::ExifToolOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowseFilenameExifOut_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
    connect(UtcOffsetHours_spinBox, SIGNAL(valueChanged(int)), this, SLOT(showUtcOffset()));
    connect(UtcOffsetMinutes_spinBox, SIGNAL(valueChanged(int)), this, SLOT(showUtcOffset()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doExifToolOptionsDialog()
{
    int i_DialogResult = QDialog::Rejected;

    QFileInfo fi( gs_et_FilenameOut );
    QTime time( 0, 0, 0 );

// ***********************************************************************************************************************

    if ( gi_et_UtcOffset >= 0 )
        time = time.addSecs( gi_et_UtcOffset );
    else
        time = time.addSecs( -gi_et_UtcOffset );

    if ( fi.exists() == false )
        gs_et_FilenameOut = getDocumentDir() + "/" + fi.baseName() + "_position.txt";

    ExifToolOptionsDialog dialog( this );

    switch ( gi_et_OutputFormatDateTime )
    {
        case _BUILDDATE:
            dialog.Day_radioButton->setChecked( true );
            break;
        case _BUILDDATETIME:
            dialog.ISO_radioButton->setChecked( true );
            break;
        default:
            dialog.realISO_radioButton->setChecked( true );
            break;
    }

    if ( gi_et_UtcOffset >= 0 )
        dialog.UtcOffsetHours_spinBox->setValue( time.hour() );
    else
        dialog.UtcOffsetHours_spinBox->setValue( -time.hour() );

    dialog.UtcOffsetMinutes_spinBox->setValue( time.minute() );

    dialog.CreateKmlFile_checkBox->setChecked( gb_et_CreateKmlFile );

    dialog.FilenameExifOut_lineEdit->setText( QDir::toNativeSeparators( gs_et_FilenameOut ) );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.BrowseFilenameExifOut_pushButton->setWhatsThis( "Browse for the text file" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gs_et_FilenameOut = dialog.FilenameExifOut_lineEdit->text();

        if ( dialog.Day_radioButton->isChecked() == true )
            gi_et_OutputFormatDateTime = _BUILDDATE;

        if ( dialog.ISO_radioButton->isChecked() == true )
            gi_et_OutputFormatDateTime = _BUILDDATETIME;

        if ( dialog.realISO_radioButton->isChecked() == true )
            gi_et_OutputFormatDateTime = _BUILDISODATETIME;

        gb_et_CreateKmlFile = dialog.CreateKmlFile_checkBox->isChecked();

        if ( dialog.UtcOffsetHours_spinBox->value() >= 0 )
            gi_et_UtcOffset = dialog.UtcOffsetHours_spinBox->value()*3600 + dialog.UtcOffsetMinutes_spinBox->value()*60;
        else
            gi_et_UtcOffset = dialog.UtcOffsetHours_spinBox->value()*3600 - dialog.UtcOffsetMinutes_spinBox->value()*60;

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

void ExifToolOptionsDialog::showUtcOffset()
{
    int i_UtcOffset = 0;

    QDateTime dt( QDateTime::currentDateTime().toUTC() );

// ***********************************************************************************************************************

    DateTimeUTC_label->setText( dt.toString( "yyyy-MM-dd hh:mm" ) );

    if ( UtcOffsetHours_spinBox->value() >= 0 )
        i_UtcOffset = UtcOffsetHours_spinBox->value()*3600 + UtcOffsetMinutes_spinBox->value()*60;
    else
        i_UtcOffset = UtcOffsetHours_spinBox->value()*3600 - UtcOffsetMinutes_spinBox->value()*60;

    if ( ( i_UtcOffset >= -43200 ) && ( i_UtcOffset <= 50400 ) )  // UTC-12h to UTC+14h
    {
        DateTimeLocal_label->setText( dt.toOffsetFromUtc( i_UtcOffset ).toString( "yyyy-MM-dd hh:mm" ) );
        OK_pushButton->setEnabled( true );
    }
    else
    {
        DateTimeLocal_label->setText( "ERROR" );
        OK_pushButton->setEnabled( false );
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void ExifToolOptionsDialog::browseFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->FilenameExifOut_lineEdit->text();
    QString fileStart	= getDocumentDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    QFileInfo fi( file );
    QDir di( fi.absolutePath() );

    if ( ( file.isEmpty() == false ) && ( di.exists() == true ) )
        fileStart = file;

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "Text file (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "Text file (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getSaveFileName( this, tr( "Select the output file (*.txt)" ), fileStart, tr( "Text file (*.txt)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == true )
        fn = file;

    this->FilenameExifOut_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    FilenameExifOut_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString ExifToolOptionsDialog::getDocumentDir()
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
