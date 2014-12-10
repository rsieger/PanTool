// ***********************************************************************************************
// *                                                                                             *
// * DateTimeDialog.cpp - Ask for columns with data/time information                             *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-05-06                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "DateTimeDialog.h"

DateTimeDialog::DateTimeDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    DateColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    YearColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    MonthColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    DayColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );

    TimeColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    HourColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    MinuteColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    SecondColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );

    DateTimeColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    DayOfYearColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    JulianDayColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( DateColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( YearColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( MonthColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( DayColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( TimeColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( HourColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( SecondColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( DateTimeColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( DayOfYearColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( JulianDayColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void DateTimeDialog::clearAll()
{
    DateColumn_lineEdit->setText( "" );
    YearColumn_lineEdit->setText( "" );
    MonthColumn_lineEdit->setText( "" );
    DayColumn_lineEdit->setText( "" );

    TimeColumn_lineEdit->setText( "" );
    HourColumn_lineEdit->setText( "" );
    MinuteColumn_lineEdit->setText( "" );
    SecondColumn_lineEdit->setText( "" );

    DateTimeColumn_lineEdit->setText( "" );
    DayOfYearColumn_lineEdit->setText( "" );
    JulianDayColumn_lineEdit->setText( "" );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void DateTimeDialog::enableOKButton()
{
    QList<int>  testList;

    bool b_OK = false;

    testList.append( DateColumn_lineEdit->text().toInt() );         // 0
    testList.append( YearColumn_lineEdit->text().toInt() );         // 1
    testList.append( MonthColumn_lineEdit->text().toInt() );        // 2
    testList.append( DayColumn_lineEdit->text().toInt() );          // 3
    testList.append( TimeColumn_lineEdit->text().toInt() );         // 4
    testList.append( HourColumn_lineEdit->text().toInt() );         // 5
    testList.append( MinuteColumn_lineEdit->text().toInt() );       // 6
    testList.append( SecondColumn_lineEdit->text().toInt() );       // 7
    testList.append( DateTimeColumn_lineEdit->text().toInt() );     // 8
    testList.append( DayOfYearColumn_lineEdit->text().toInt() );    // 9
    testList.append( JulianDayColumn_lineEdit->text().toInt() );    // 10

    if ( testList.at( 0 ) < 1 )  DateColumn_lineEdit->setText( "" );
    if ( testList.at( 1 ) < 1 )  YearColumn_lineEdit->setText( "" );
    if ( testList.at( 2 ) < 1 )  MonthColumn_lineEdit->setText( "" );
    if ( testList.at( 3 ) < 1 )  DayColumn_lineEdit->setText( "" );
    if ( testList.at( 4 ) < 1 )  TimeColumn_lineEdit->setText( "" );
    if ( testList.at( 5 ) < 1 )  HourColumn_lineEdit->setText( "" );
    if ( testList.at( 6 ) < 1 )  MinuteColumn_lineEdit->setText( "" );
    if ( testList.at( 7 ) < 1 )  SecondColumn_lineEdit->setText( "" );
    if ( testList.at( 8 ) < 1 )  DateTimeColumn_lineEdit->setText( "" );
    if ( testList.at( 9 ) < 1 )  DayOfYearColumn_lineEdit->setText( "" );
    if ( testList.at( 10 ) < 1 ) JulianDayColumn_lineEdit->setText( "" );

    for ( int i=0; i<testList.count(); i++ )
        if ( testList.at( i ) > 0 ) b_OK = true;

    if ( ( testList.at( 9 ) > 0 ) && ( testList.at( 1 ) < 1 ) )  // DayOfYearColumn != 0 and YearColumn == 0
        b_OK = false;

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

int MainWindow::doDateTimeDialog( int &i_DateColumn, int &i_YearColumn, int &i_MonthColumn, int &i_DayColumn, int &i_TimeColumn, int &i_HourColumn, int &i_MinuteColumn, int &i_SecondColumn, int &i_DateTimeColumn, int &i_DayOfYearColumn, int &i_JulianDayColumn, bool &b_WriteDateTimeOnly )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    DateTimeDialog dialog( this );

    dialog.DateColumn_lineEdit->setText( QString( "%1" ).arg( i_DateColumn ) );
    dialog.YearColumn_lineEdit->setText( QString( "%1" ).arg( i_YearColumn ) );
    dialog.MonthColumn_lineEdit->setText( QString( "%1" ).arg( i_MonthColumn ) );
    dialog.DayColumn_lineEdit->setText( QString( "%1" ).arg( i_DayColumn ) );
    dialog.TimeColumn_lineEdit->setText( QString( "%1" ).arg( i_TimeColumn ) );
    dialog.HourColumn_lineEdit->setText( QString( "%1" ).arg( i_HourColumn ) );
    dialog.MinuteColumn_lineEdit->setText( QString( "%1" ).arg( i_MinuteColumn ) );
    dialog.SecondColumn_lineEdit->setText( QString( "%1" ).arg( i_SecondColumn ) );
    dialog.DateTimeColumn_lineEdit->setText( QString( "%1" ).arg( i_DateTimeColumn ) );
    dialog.DayOfYearColumn_lineEdit->setText( QString( "%1" ).arg( i_DayOfYearColumn ) );
    dialog.JulianDayColumn_lineEdit->setText( QString( "%1" ).arg( i_JulianDayColumn ) );

    dialog.WriteDateTimeOnly_checkBox->setChecked( b_WriteDateTimeOnly );

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
        i_DateColumn        = dialog.DateColumn_lineEdit->text().toInt();
        i_YearColumn        = dialog.YearColumn_lineEdit->text().toInt();
        i_MonthColumn       = dialog.MonthColumn_lineEdit->text().toInt();
        i_DayColumn         = dialog.DayColumn_lineEdit->text().toInt();
        i_TimeColumn        = dialog.TimeColumn_lineEdit->text().toInt();
        i_HourColumn        = dialog.HourColumn_lineEdit->text().toInt();
        i_MinuteColumn      = dialog.MinuteColumn_lineEdit->text().toInt();
        i_SecondColumn      = dialog.SecondColumn_lineEdit->text().toInt();
        i_DateTimeColumn    = dialog.DateTimeColumn_lineEdit->text().toInt();
        i_DayOfYearColumn   = dialog.DayOfYearColumn_lineEdit->text().toInt();
        i_JulianDayColumn   = dialog.JulianDayColumn_lineEdit->text().toInt();

        b_WriteDateTimeOnly = dialog.WriteDateTimeOnly_checkBox->isChecked();

        i_DialogResult      = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
