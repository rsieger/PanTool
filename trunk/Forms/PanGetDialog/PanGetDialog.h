#ifndef PANGETDIALOG_H
#define PANGETDIALOG_H

#include <QDialog>
#include <QTextEdit>

#include "Webfile.h"
#include "ui_pangetdialog.h"

class QProgressDialog;

class PanGetDialog : public QDialog, public Ui::PanGetDialog
{
    Q_OBJECT
    Q_CLASSINFO( "author", "Rainer Sieger" )
    Q_CLASSINFO( "url", "http://www.pangaea.de" )

public:
    PanGetDialog(QWidget *parent = 0);
    QString s_PrefVersion;

private:
    QString getDocumentDir();
    QTextEdit *textViewer;

private slots:
    void enableBuildButton();
    void browseIDListFileDialog();
    void browseDownloadDirectoryDialog();
    void displayHelp();

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );

};

#endif // PANGETDIALOG_H
