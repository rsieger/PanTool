#ifndef DELETELINESDIALOG_H
#define DELETELINESDIALOG_H

#include <QDialog>

#include "ui_deletelinesdialog.h"

class DeleteLinesDialog : public QDialog, public Ui::DeleteLinesDialog
{
Q_OBJECT

public:
    DeleteLinesDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
