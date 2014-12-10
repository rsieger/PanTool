#ifndef DATETIMEDIALOG_H
#define DATETIMEDIALOG_H

#include <QDialog>

#include "ui_datetimedialog.h"

class DateTimeDialog : public QDialog, public Ui::DateTimeDialog
{
Q_OBJECT

public:
    DateTimeDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
