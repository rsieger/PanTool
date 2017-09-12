#ifndef RSCRIPTOPTIONSDIALOG_H
#define RSCRIPTOPTIONSDIALOG_H

#include <QDialog>

#include "ui_rscriptoptionsdialog.h"

class RScriptOptionsDialog : public QDialog, public Ui::RScriptOptionsDialog
{
Q_OBJECT

public:
    RScriptOptionsDialog( QWidget *parent = 0 );
    QString getProgramDir();

private slots:
    void browseFilenameDialog();
};

#endif
