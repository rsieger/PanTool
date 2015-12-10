#ifndef FILELISTOPTIONSDIALOG_H
#define FILELISTOPTIONSDIALOG_H

#include <QDialog>

#include "ui_filelistoptionsdialog.h"

class FileListOptionsDialog : public QDialog, public Ui::FileListOptionsDialog
{
Q_OBJECT

public:
    FileListOptionsDialog( QWidget *parent = 0 );

private slots:
    void clearAll();
};

#endif
