#ifndef TRANSPOSETABLEOPTIONSDIALOG_H
#define TRANSPOSETABLEOPTIONSDIALOG_H

#include <QDialog>

#include "ui_transposetableoptionsdialog.h"

class TransposeTableOptionsDialog : public QDialog, public Ui::TransposeTableOptionsDialog
{
Q_OBJECT

public:
    TransposeTableOptionsDialog( QWidget *parent = 0 );
};

#endif
