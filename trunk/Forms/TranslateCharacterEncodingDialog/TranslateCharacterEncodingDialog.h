#ifndef TRANSLATECHARACTERENCODINGDIALOG_H
#define TRANSLATECHARACTERENCODINGDIALOG_H

#include <QDialog>

#include "ui_translatecharacterencodingdialog.h"

class TranslateCharacterEncodingDialog : public QDialog, public Ui::TranslateCharacterEncodingDialog
{
Q_OBJECT

public:
    TranslateCharacterEncodingDialog( QWidget *parent = 0 );
};

#endif
