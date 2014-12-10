// ***********************************************************************************************
// *                                                                                             *
// * ODPSampleLabelDialog.cpp - Ask for columns with OPD sample information                      *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-10-02                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "ODPSampleLabelDialog.h"

ODPSampleLabelDialog::ODPSampleLabelDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    LegColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    SiteColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    HoleColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    CoreColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    TypeColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    SectionColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    TopColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    BottomColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( LegColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( SiteColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( HoleColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( CoreColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( TypeColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( SectionColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( TopColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( BottomColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void ODPSampleLabelDialog::clearAll()
{
    LegColumn_lineEdit->setText( "" );
    SiteColumn_lineEdit->setText( "" );
    HoleColumn_lineEdit->setText( "" );
    CoreColumn_lineEdit->setText( "" );
    TypeColumn_lineEdit->setText( "" );
    SectionColumn_lineEdit->setText( "" );
    TopColumn_lineEdit->setText( "" );
    BottomColumn_lineEdit->setText( "" );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void ODPSampleLabelDialog::enableOKButton()
{
    QList<int>  testList;

    bool b_OK = false;

    testList.append( LegColumn_lineEdit->text().toInt() );         // 0
    testList.append( SiteColumn_lineEdit->text().toInt() );        // 1
    testList.append( HoleColumn_lineEdit->text().toInt() );        // 2
    testList.append( CoreColumn_lineEdit->text().toInt() );        // 3
    testList.append( TypeColumn_lineEdit->text().toInt() );        // 4
    testList.append( SectionColumn_lineEdit->text().toInt() );     // 5
    testList.append( TopColumn_lineEdit->text().toInt() );         // 6
    testList.append( BottomColumn_lineEdit->text().toInt() );      // 7

    if ( testList.at( 0 ) < 1 )  LegColumn_lineEdit->setText( "" );
    if ( testList.at( 1 ) < 1 )  SiteColumn_lineEdit->setText( "" );
    if ( testList.at( 2 ) < 1 )  HoleColumn_lineEdit->setText( "" );
    if ( testList.at( 3 ) < 1 )  CoreColumn_lineEdit->setText( "" );
    if ( testList.at( 4 ) < 1 )  TypeColumn_lineEdit->setText( "" );
    if ( testList.at( 5 ) < 1 )  SectionColumn_lineEdit->setText( "" );
    if ( testList.at( 6 ) < 1 )  TopColumn_lineEdit->setText( "" );
    if ( testList.at( 7 ) < 1 )  BottomColumn_lineEdit->setText( "" );

    for ( int i=0; i<testList.count(); i++ )
        if ( testList.at( i ) > 0 ) b_OK = true;

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

int MainWindow::doODPSampleLabelDialog( int &i_LegColumn, int &i_SiteColumn, int &i_HoleColumn, int &i_CoreColumn, int &i_CoreTypeColumn, int &i_SectionColumn, int &i_TopColumn, int &i_BottomColumn, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    ODPSampleLabelDialog dialog( this );

    dialog.LegColumn_lineEdit->setText( QString( "%1" ).arg( i_LegColumn ) );
    dialog.SiteColumn_lineEdit->setText( QString( "%1" ).arg( i_SiteColumn ) );
    dialog.HoleColumn_lineEdit->setText( QString( "%1" ).arg( i_HoleColumn ) );
    dialog.CoreColumn_lineEdit->setText( QString( "%1" ).arg( i_CoreColumn ) );
    dialog.TypeColumn_lineEdit->setText( QString( "%1" ).arg( i_CoreTypeColumn ) );
    dialog.SectionColumn_lineEdit->setText( QString( "%1" ).arg( i_SectionColumn ) );
    dialog.TopColumn_lineEdit->setText( QString( "%1" ).arg( i_TopColumn ) );
    dialog.BottomColumn_lineEdit->setText( QString( "%1" ).arg( i_BottomColumn ) );

    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

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
        i_LegColumn        = dialog.LegColumn_lineEdit->text().toInt();
        i_SiteColumn       = dialog.SiteColumn_lineEdit->text().toInt();
        i_HoleColumn       = dialog.HoleColumn_lineEdit->text().toInt();
        i_CoreColumn       = dialog.CoreColumn_lineEdit->text().toInt();
        i_CoreTypeColumn   = dialog.TypeColumn_lineEdit->text().toInt();
        i_SectionColumn    = dialog.SectionColumn_lineEdit->text().toInt();
        i_TopColumn        = dialog.TopColumn_lineEdit->text().toInt();
        i_BottomColumn     = dialog.BottomColumn_lineEdit->text().toInt();

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
