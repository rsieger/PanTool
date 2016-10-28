#ifndef GOOGLEEARTHOPTIONSDIALOG_H
#define GOOGLEEARTHOPTIONSDIALOG_H

#include <QDialog>

#include "ui_googleearthoptionsdialog.h"

class GoogleEarthOptionsDialog : public QDialog, public Ui::GoogleEarthOptionsDialog
{
Q_OBJECT

public:
    GoogleEarthOptionsDialog( QWidget *parent = 0 );
    QString getProgramDir();
    QString getDocumentDir();

private slots:
    void browseFilenameDialog();
    void browseOutputFilenameDialog();
};

#endif
