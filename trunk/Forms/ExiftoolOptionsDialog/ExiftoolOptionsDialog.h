#ifndef ExifToolOPTIONDIALOG_H
#define ExifToolOPTIONDIALOG_H

#include <QDialog>

#include "ui_exiftooloptionsdialog.h"

class ExifToolOptionsDialog : public QDialog, public Ui::ExifToolOptionsDialog
{
Q_OBJECT

public:
    ExifToolOptionsDialog( QWidget *parent = 0 );
    QString getDocumentDir();

private slots:
    void browseFilenameDialog();
    void showUtcOffset();
};

#endif
