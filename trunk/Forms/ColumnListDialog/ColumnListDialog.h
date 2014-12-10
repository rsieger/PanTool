#ifndef COLUMNLISTDIALOG_H
#define COLUMNLISTDIALOG_H

#include <QDialog>

#include "ui_columnlistdialog.h"

class ColumnListDialog : public QDialog, public Ui::ColumnListDialog
{
Q_OBJECT

public:
    ColumnListDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
