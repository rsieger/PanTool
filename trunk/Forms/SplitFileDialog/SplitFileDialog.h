#ifndef SPLITFILEDIALOG_H
#define SPLITFILEDIALOG_H

#include <QDialog>

#include "ui_splitfiledialog.h"

class SplitFileDialog : public QDialog, public Ui::SplitFileDialog
{
Q_OBJECT

public:
    SplitFileDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();
};

#endif
