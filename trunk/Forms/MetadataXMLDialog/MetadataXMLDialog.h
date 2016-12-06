#ifndef METADATAXMLDIALOG_H
#define METADATAXMLDIALOG_H

#include <QDialog>

#include "ui_metadataxmldialog.h"

class MetadataXMLDialog : public QDialog, public Ui::MetadataXMLDialog
{
Q_OBJECT

public:
    MetadataXMLDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
