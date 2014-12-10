#ifndef CONCATENATEFILESOPTIONSDIALOG_H
#define CONCATENATEFILESOPTIONSDIALOG_H

#include <QDialog>

#include "ui_concatenatefilesoptionsdialog.h"

class ConcatenateFilesOptionsDialog : public QDialog, public Ui::ConcatenateFilesOptionsDialog
{
Q_OBJECT

public:
    ConcatenateFilesOptionsDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
