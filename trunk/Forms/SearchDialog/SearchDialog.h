#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

#include "ui_searchdialog.h"

class SearchDialog : public QDialog, public Ui::SearchDialog
{
    Q_OBJECT

public:
    SearchDialog( QWidget *parent = 0 );

private:
    QString getHomeDir();
    QString getDocumentDir();

private slots:
    void browseDatabaseNameDialog();
    void clearAll();

public slots:
    void enableOKButton();

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );
};

#endif
