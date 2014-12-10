#ifndef RECALCDIALOG_H
#define RECALCDIALOG_H

#include <QDialog>

#include "ui_recalcdialog.h"

class RecalcDialog : public QDialog, public Ui::RecalcDialog
{
Q_OBJECT

public:
    RecalcDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
