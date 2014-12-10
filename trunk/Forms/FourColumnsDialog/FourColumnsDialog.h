#ifndef FOURCOLUMNSDIALOG_H
#define FOURCOLUMNSDIALOG_H

#include <QDialog>

#include "ui_fourcolumnsdialog.h"

class FourColumnsDialog : public QDialog, public Ui::FourColumnsDialog
{
Q_OBJECT

public:
    FourColumnsDialog( QWidget *parent = 0 );
};

#endif
