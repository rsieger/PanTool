#ifndef SKIPLINESDIALOG_H
#define SKIPLINESDIALOG_H

#include <QDialog>

#include "ui_skiplinesdialog.h"

class SkipLinesDialog : public QDialog, public Ui::SkipLinesDialog
{
Q_OBJECT

public:
    SkipLinesDialog( QWidget *parent = 0 );
};

#endif
