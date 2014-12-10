#ifndef ODPSAMPLELABELDIALOG_H
#define ODPSAMPLELABELDIALOG_H

#include <QDialog>

#include "ui_odpsamplelabeldialog.h"

class ODPSampleLabelDialog : public QDialog, public Ui::ODPSampleLabelDialog
{
Q_OBJECT

public:
    ODPSampleLabelDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
