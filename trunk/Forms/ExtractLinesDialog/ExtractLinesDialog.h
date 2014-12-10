#ifndef EXTRACTLINESDIALOG_H
#define EXTRACTLINESDIALOG_H

#include <QDialog>

#include "ui_extractlinesdialog.h"

class ExtractLinesDialog : public QDialog, public Ui::ExtractLinesDialog
{
Q_OBJECT

public:
    ExtractLinesDialog( QWidget *parent = 0 );

private slots:
    void setEnabledLineEdit( int state );
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
