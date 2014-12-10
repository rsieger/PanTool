#ifndef FILENAMEFOLDERDIALOG_H
#define FILENAMEFOLDERDIALOG_H

#include <QDialog>
#include <QTextEdit>

#include "ui_filenamefolderdialog.h"

class QProgressDialog;

class FilenameFolderDialog : public QDialog, public Ui::FilenameFolderDialog
{
    Q_OBJECT

public:
    FilenameFolderDialog(QWidget *parent = 0);

private:
    QString getDocumentDir();

    QTextEdit *textViewer;

private slots:
    void browseFilenameListFileDialog();
    void browseTargetDirectoryDialog();
    void displayHelp();

public slots:
    void enableOKButton();

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );

};

#endif // FILENAMEFOLDERDIALOG_H
