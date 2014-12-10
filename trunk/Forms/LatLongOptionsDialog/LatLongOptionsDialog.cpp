// ***********************************************************************************************
// *                                                                                             *
// * LatLongOptionsDialog.cpp - Latitude/Longitude converter options dialog                      *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-05-16                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "LatLongOptionsDialog.h"

LatLongOptionsDialog::LatLongOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    LatitudeColumn_lineEdit_1->setValidator( new QIntValidator( 1, 9999, this ) );
    LongitudeColumn_lineEdit_1->setValidator( new QIntValidator( 1, 9999, this ) );

    LatitudeColumn_lineEdit_2->setValidator( new QIntValidator( 1, 9999, this ) );
    LongitudeColumn_lineEdit_2->setValidator( new QIntValidator( 1, 9999, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( LatitudeColumn_lineEdit_1, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( LongitudeColumn_lineEdit_1, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );

    connect( LatitudeColumn_lineEdit_2, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( LongitudeColumn_lineEdit_2, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( PressureColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void LatLongOptionsDialog::clearAll()
{
    LatitudeColumn_lineEdit_1->setText( "" );
    LongitudeColumn_lineEdit_1->setText( "" );

    LatitudeColumn_lineEdit_2->setText( "" );
    LongitudeColumn_lineEdit_2->setText( "" );
    PressureColumn_lineEdit->setText( "" );

    NumOfDigitsPosition_spinBox->setValue( 4 );
    NumOfDigitsDepth_spinBox->setValue( 2 );

    Dot_radioButton->setChecked( true );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void LatLongOptionsDialog::enableOKButton()
{
    QList<int>  testList;

    bool b_OK = false;

    if ( PressureToDepthPositions_groupBox->isHidden() == true )
    {
        testList.append( LatitudeColumn_lineEdit_1->text().toInt() );         // 0
        testList.append( LongitudeColumn_lineEdit_1->text().toInt() );        // 1

        if ( testList.at( 0 ) < 1 ) LatitudeColumn_lineEdit_1->setText( "" );
        if ( testList.at( 1 ) < 1 ) LongitudeColumn_lineEdit_1->setText( "" );
    }
    else
    {
        testList.append( LatitudeColumn_lineEdit_2->text().toInt() );         // 0
        testList.append( LongitudeColumn_lineEdit_2->text().toInt() );        // 1
        testList.append( PressureColumn_lineEdit->text().toInt() );           // 2

        if ( testList.at( 0 ) < 1 ) LatitudeColumn_lineEdit_2->setText( "" );
        if ( testList.at( 1 ) < 1 ) LongitudeColumn_lineEdit_2->setText( "" );
        if ( testList.at( 2 ) < 1 ) PressureColumn_lineEdit->setText( "" );
    }

    if ( testList.count() < 3 )
    {
        if ( ( testList.at( 0 ) > 0 ) && ( testList.at( 1 ) > 0 ) )         // Latitude > 0 and Longitude > 0
            b_OK = true;
    }
    else
    {
        if ( ( testList.at( 0 ) > 0 ) && ( testList.at( 1 ) > 0 ) && ( testList.at( 2 ) > 0 ) )         // Latitude > 0 and Longitude > 0 and Pressure > 0
            b_OK = true;
    }

    if ( b_OK == true )
    {
        for ( int i=0; i<testList.count(); i++ )
        {
            if ( testList.at( i ) > 0 )
            {
                for ( int j=0; j<testList.count(); j++ )
                {
                    if ( ( i != j ) && ( testList.at( i ) == testList.at( j ) ) )
                         b_OK = false;
                }
            }
        }
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

int MainWindow::doLatLongOptionsDialog( int &i_LatitudeColumn, int &i_LongitudeColumn, int &i_PressureColumn, int &i_NumOfDigits, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    LatLongOptionsDialog dialog( this );

    dialog.setWindowTitle( trUtf8( "Convert pressure to depth dialog" ) );
    dialog.LatLongPositions_groupBox->hide();
    dialog.PositionResolution_groupBox->hide();
    dialog.DecimalSeparator_groupBox->hide();
    dialog.LatitudeColumn_lineEdit_2->setText( QString( "%1" ).arg( i_LatitudeColumn ) );
    dialog.LongitudeColumn_lineEdit_2->setText( QString( "%1" ).arg( i_LongitudeColumn ) );
    dialog.PressureColumn_lineEdit->setText( QString( "%1" ).arg( i_PressureColumn ) );
    dialog.NumOfDigitsDepth_spinBox->setValue( i_NumOfDigits );
    dialog.NumOfDigitsDepth_spinBox->setRange( 0, 3 );

    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    if ( gsl_FilenameList.count() > 1 )
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );

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
        i_LatitudeColumn  = dialog.LatitudeColumn_lineEdit_2->text().toInt();
        i_LongitudeColumn = dialog.LongitudeColumn_lineEdit_2->text().toInt();
        i_PressureColumn  = dialog.PressureColumn_lineEdit->text().toInt();
        i_NumOfDigits     = dialog.NumOfDigitsDepth_spinBox->value();

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
