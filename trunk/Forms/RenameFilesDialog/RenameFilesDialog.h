#ifndef RENAMEFILESDIALOG_H
#define RENAMEFILESDIALOG_H

#include <QDialog>

#include "ui_renamefilesdialog.h"

class RenameFilesDialog : public QDialog, public Ui::RenameFilesDialog
{
Q_OBJECT

public:
    RenameFilesDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
