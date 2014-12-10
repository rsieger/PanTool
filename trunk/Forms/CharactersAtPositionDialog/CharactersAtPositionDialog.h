#ifndef CHARACTERSATPOSITIONDIALOG_H
#define CHARACTERSATPOSITIONDIALOG_H

#include <QDialog>

#include "ui_charactersatpositiondialog.h"

class CharactersAtPositionDialog : public QDialog, public Ui::CharactersAtPositionDialog
{
Q_OBJECT

public:
    CharactersAtPositionDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
