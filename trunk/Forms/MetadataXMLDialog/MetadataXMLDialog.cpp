// ***********************************************************************************************
// *                                                                                             *
// * RecalcDialog.cpp - Recalculation of column                                                  *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-08-20                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "MetadataXMLDialog.h"

MetadataXMLDialog::MetadataXMLDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect(Citation_checkBox, SIGNAL(clicked()), this, SLOT(enableOKButton()));
    connect(Authors_checkBox, SIGNAL(clicked()), this, SLOT(enableOKButton()));
    connect(References_checkBox, SIGNAL(clicked()), this, SLOT(enableOKButton()));
    connect(Projects_checkBox, SIGNAL(clicked()), this, SLOT(enableOKButton()));
    connect(Events_checkBox, SIGNAL(clicked()), this, SLOT(enableOKButton()));
    connect(Parameters_checkBox, SIGNAL(clicked()), this, SLOT(enableOKButton()));
    connect(Coverage_checkBox, SIGNAL(clicked()), this, SLOT(enableOKButton()));
    connect(Keywords_checkBox, SIGNAL(clicked()), this, SLOT(enableOKButton()));
    connect(TechnicalInfo_checkBox, SIGNAL(clicked()), this, SLOT(enableOKButton()));

}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MetadataXMLDialog::clearAll()
{
    Citation_checkBox->setChecked( false );
    Authors_checkBox->setChecked( false );
    References_checkBox->setChecked( false );
    Projects_checkBox->setChecked( false );
    Events_checkBox->setChecked( false );
    Parameters_checkBox->setChecked( false );
    Coverage_checkBox->setChecked( false );
    Keywords_checkBox->setChecked( false );
    TechnicalInfo_checkBox->setChecked( false );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MetadataXMLDialog::enableOKButton()
{
    bool b_OK = false;

    if ( Citation_checkBox->isChecked() == true ) b_OK = true;
    if ( Authors_checkBox->isChecked() == true ) b_OK = true;
    if ( References_checkBox->isChecked() == true ) b_OK = true;
    if ( Projects_checkBox->isChecked() == true ) b_OK = true;
    if ( Events_checkBox->isChecked() == true ) b_OK = true;
    if ( Parameters_checkBox->isChecked() == true ) b_OK = true;
    if ( Coverage_checkBox->isChecked() == true ) b_OK = true;
    if ( Keywords_checkBox->isChecked() == true ) b_OK = true;
    if ( TechnicalInfo_checkBox->isChecked() == true ) b_OK = true;

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

int MainWindow::doMetadataXMLDialog( bool &b_Citation, bool &b_Authors, bool &b_References, bool &b_Projects, bool &b_Events,
                                     bool &b_Parameters, bool &b_Coverage, bool &b_Keywords, bool &b_TechnicalInfo )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    MetadataXMLDialog dialog( this );

    dialog.Citation_checkBox->setChecked( b_Citation );
    dialog.Authors_checkBox->setChecked( b_Authors );
    dialog.References_checkBox->setChecked( b_References );
    dialog.Projects_checkBox->setChecked( b_Projects );
    dialog.Events_checkBox->setChecked( b_Events );
    dialog.Parameters_checkBox->setChecked( b_Parameters );
    dialog.Coverage_checkBox->setChecked( b_Coverage );
    dialog.Keywords_checkBox->setChecked( b_Keywords );
    dialog.TechnicalInfo_checkBox->setChecked( b_TechnicalInfo );

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
        b_Citation      = dialog.Citation_checkBox->isChecked();
        b_Authors       = dialog.Authors_checkBox->isChecked();
        b_References    = dialog.References_checkBox->isChecked();
        b_Projects      = dialog.Projects_checkBox->isChecked();
        b_Events        = dialog.Events_checkBox->isChecked();
        b_Parameters    = dialog.Parameters_checkBox->isChecked();
        b_Coverage      = dialog.Coverage_checkBox->isChecked();
        b_Keywords      = dialog.Keywords_checkBox->isChecked();
        b_TechnicalInfo = dialog.TechnicalInfo_checkBox->isChecked();

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
